/* 
 * This file is part of the wfs distribution (https://github.com/lauch788/wfs).
 * Copyright (c) 2023 Adrien Ricciardi.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdio.h>

#include <grapheme.h>

#include <wfs/dom.h>
#include <wfs/dom_core.h>
#include <wfs/html_parse.h>

#include <wfs/infra_string.h>

#include "unicode.h"

struct tokenizer;
struct treebuilder;

enum token_type {
  TOKEN_START_TAG,
  TOKEN_END_TAG,

  TOKEN_DOCTYPE,

  TOKEN_COMMENT,

  TOKEN_CHARACTER,
  TOKEN_WHITESPACE, /* extensions */

  TOKEN_EOF,
};

struct tag {
  InfraString *tagname;
  uint16_t localname;
  void *attrs;

  bool self_closing_fl;
  bool ack_self_closing_fl;
};

struct attr {
  InfraString *name;
  InfraString *value;
};

struct doctype {
  InfraString *name;
  InfraString *public_id;
  InfraString *system_id;
  bool name_missing;
  bool system_id_missing;
  bool public_id_missing;
  bool force_quirks;
};

union token_data {
  struct tag      tag;
  struct doctype  doctype;
  InfraString *   comment;
  uint32_t        c;
};

enum tokenizer_state {
  DATA_STATE,
  RCDATA_STATE,
  RAWTEXT_STATE,
  SCRIPT_STATE,
  PLAINTEXT_STATE,
  TAG_OPEN_STATE,
  END_TAG_OPEN_STATE,
  TAG_NAME_STATE,
  RCDATA_LT_STATE,
  RCDATA_END_TAG_OPEN_STATE,
  RCDATA_END_TAG_NAME_STATE,
  RAWTEXT_LT_STATE,
  RAWTEXT_END_TAG_OPEN_STATE,
  RAWTEXT_END_TAG_NAME_STATE,
  SCRIPT_LT_STATE,
  SCRIPT_END_TAG_OPEN_STATE,
  SCRIPT_END_TAG_NAME_STATE,
  SCRIPT_ESCAPE_START_STATE,
  SCRIPT_ESCAPE_START_DASH_STATE,
  SCRIPT_ESCAPED_STATE,
  SCRIPT_ESCAPED_DASH_STATE,
  SCRIPT_ESCAPED_DASH_DASH_STATE,
  SCRIPT_ESCAPED_LT_STATE,
  SCRIPT_ESCAPED_END_TAG_OPEN_STATE,
  SCRIPT_ESCAPED_END_TAG_NAME_STATE,
  SCRIPT_DOUBLE_ESCAPE_START_STATE,
  SCRIPT_DOUBLE_ESCAPED_STATE,
  SCRIPT_DOUBLE_ESCAPED_DASH_STATE,
  SCRIPT_DOUBLE_ESCAPED_DASH_DASH_STATE,
  SCRIPT_DOUBLE_ESCAPED_LT_STATE,
  SCRIPT_DOUBLE_ESCAPE_END_STATE,
  BEFORE_ATTR_NAME_STATE,
  ATTR_NAME_STATE,
  AFTER_ATTR_NAME_STATE,
  BEFORE_ATTR_VALUE_STATE,
  ATTR_VALUE_DOUBLE_QUOTED_STATE,
  ATTR_VALUE_SINGLE_QUOTED_STATE,
  ATTR_VALUE_UNQUOTED_STATE,
  AFTER_ATTR_VALUE_QUOTED_STATE,
  AFTER_ATTR_VALUE_UNQUOTED_STATE,
  SELF_CLOSING_START_TAG_STATE,
  BOGUS_COMMENT_STATE,
  MARKUP_DECL_OPEN_STATE,
  COMMENT_START_STATE,
  COMMENT_START_DASH_STATE,
  COMMENT_STATE,
  COMMENT_LT_STATE,
  COMMENT_LT_BANG_STATE,
  COMMENT_LT_BANG_DASH_STATE,
  COMMENT_LT_BANG_DASH_DASH_STATE,
  COMMENT_END_DASH_STATE,
  COMMENT_END_STATE,
  COMMENT_END_BANG_STATE,
  DOCTYPE_STATE,
  BEFORE_DOCTYPE_NAME_STATE,
  DOCTYPE_NAME_STATE,
  AFTER_DOCTYPE_NAME_STATE,
  AFTER_DOCTYPE_PUBLIC_KEYWORD_STATE,
  BEFORE_DOCTYPE_PUBLIC_ID_STATE,
  DOCTYPE_PUBLIC_ID_DOUBLE_QUOTED_STATE,
  DOCTYPE_PUBLIC_ID_SINGLE_QUOTED_STATE,
  AFTER_DOCTYPE_PUBLIC_ID_STATE,
  BETWEEN_DOCTYPE_PUBLIC_SYSTEM_IDS_STATE,
  AFTER_DOCTYPE_SYSTEM_KEYWORD_STATE,
  BEFORE_DOCTYPE_SYSTEM_ID_STATE,
  DOCTYPE_SYSTEM_ID_DOUBLE_QUOTED_STATE,
  DOCTYPE_SYSTEM_ID_SINGLE_QUOTED_STATE,
  AFTER_DOCTYPE_SYSTEM_ID_STATE,
  BOGUS_DOCTYPE_STATE,
  CDATA_SECTION_STATE,
  CDATA_SECTION_BRACKET_STATE,
  CDATA_SECTION_END_STATE,
  CHAR_REF_STATE,
  NAMED_CHAR_REF_STATE,
  AMBIGUOUS_AMPERSAND_STATE,
  NUMERIC_CHAR_REF_STATE,
  HEX_CHAR_REF_START_STATE,
  DEC_CHAR_REF_START_STATE,
  HEX_CHAR_REF_STATE,
  DEC_CHAR_REF_STATE,
  NUMERIC_CHAR_REF_END_STATE,

  NUM_STATES
};

struct tokenizer {
  struct {
    const char *p;
    const char *end;
  } input;

  struct treebuilder *treebuilder;

  struct tag *tag;
  struct attr *attr;
  InfraString *comment;
  struct doctype doctype;
  enum token_type tag_type;

  enum tokenizer_state state;
  enum tokenizer_state ret_state;
};

enum tokenizer_status {
  TOKENIZER_STATUS_RECONSUME = 0,
  TOKENIZER_STATUS_OK,
  TOKENIZER_STATUS_IGNORE,
  TOKENIZER_STATUS_EOF,
};

typedef enum tokenizer_status (*tokenizer_state_handler) (struct tokenizer *tokenizer, int32_t c);

enum treebuilder_mode {
  INITIAL_MODE,
  BEFORE_HTML_MODE,
  BEFORE_HEAD_MODE,
  IN_HEAD_MODE,
  IN_HEAD_NOSCRIPT_MODE,
  AFTER_HEAD_MODE,
  IN_BODY_MODE,
  TEXT_MODE,
  IN_TABLE_MODE,
  IN_TABLE_TEXT_MODE,
  IN_CAPTION_MODE,
  IN_COLUMN_GROUP_MODE,
  IN_TABLE_BODY_MODE,
  IN_ROW_MODE,
  IN_CELL_MODE,
  IN_SELECT_MODE,
  IN_SELECT_IN_TABLE_MODE,
  IN_TEMPLATE_MODE,
  AFTER_BODY_MODE,
  IN_FRAMESET_MODE,
  AFTER_AFTER_BODY_MODE,
  AFTER_AFTER_FRAMESET_MODE,
  // IN_FOREIGN_CONTENT_MODE,

  NUM_MODES
};

struct treebuilder {
  struct tokenizer *tokenizer;

  int script_nesting;

  enum treebuilder_mode mode;
  enum treebuilder_mode original_mode;
};

enum treebuilder_status {
  TREEBUILDER_STATUS_REPROCESS = 0,
  TREEBUILDER_STATUS_OK,
  TREEBUILDER_STATUS_IGNORE,
  TREEBUILDER_STATUS_EOF,
};

// typedef enum treebuilder_status (*insertion_mode_handler) (struct treebuilder *treebuilder,

static void tokenizer_error(struct tokenizer *tokenizer, const char *msg);
static void tokenizer_mainloop(struct tokenizer *tokenizer);
static int_least32_t tokenizer_getc(struct tokenizer *tokenizer);

static int tokenizer_cmp_consume(struct tokenizer *tokenizer,
                                  int (*cmp) (const char *, const char *, size_t),
                                  const char *s, size_t slen);
static int tokenizer_match(struct tokenizer *tokenizer, const char *s, size_t slen);
static int tokenizer_matchcase(struct tokenizer *tokenizer, const char *s, size_t slen);

static void create_doctype(struct tokenizer *tokenizer);
static void destroy_tag(struct tag *tag);
static void create_tag(struct tokenizer *tokenizer, enum token_type type);
static void create_start_tag(struct tokenizer *tokenizer);
static void create_end_tag(struct tokenizer *tokenizer);
static void create_attr(struct tokenizer *tokenizer);
static void create_comment(struct tokenizer *tokenizer);

static void emit_token(struct tokenizer *tokenizer,
                       union token_data *token_data,
                       enum token_type token_type);
static void emit_tag(struct tokenizer *tokenizer);
static void emit_doctype(struct tokenizer *tokenizer);
static void emit_comment(struct tokenizer *tokenizer);
static inline enum token_type char_to_type(uint32_t c);
static void emit_character(struct tokenizer *tokenizer, uint32_t c);
static enum tokenizer_status emit_eof(struct tokenizer *tokenizer);

static void create_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder);
static void free_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder);

static enum tokenizer_status data_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status tag_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status end_tag_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status tag_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status before_attr_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status attr_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status markup_decl_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status doctype_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status before_doctype_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status doctype_name_state(struct tokenizer *tokenizer, int32_t c);

/* globals */
static const tokenizer_state_handler k_tokenizer_states[NUM_STATES] = {
  [DATA_STATE] = data_state,
  /* ... */
  [TAG_OPEN_STATE] = tag_open_state,
  [END_TAG_OPEN_STATE] = end_tag_open_state,
  [TAG_NAME_STATE] = tag_name_state,
  /* ... */
  [BEFORE_ATTR_NAME_STATE] = before_attr_name_state,
  [ATTR_NAME_STATE] = attr_name_state,
  /* ... */
  [MARKUP_DECL_OPEN_STATE] = markup_decl_open_state,
  /* ... */
  [DOCTYPE_STATE] = doctype_state,
  [BEFORE_DOCTYPE_NAME_STATE] = before_doctype_name_state,
  [DOCTYPE_NAME_STATE] = doctype_name_state,
  /* ... */
};


static void
tokenizer_error(struct tokenizer *tokenizer, const char *msg)
{
  (void) tokenizer;
  printf("%s\n", msg);
}

static void
tokenizer_mainloop(struct tokenizer *tokenizer)
{
  enum tokenizer_status rc = TOKENIZER_STATUS_OK;

  while (rc != TOKENIZER_STATUS_EOF) {
    int32_t c;

    switch (tokenizer->state) {
      case MARKUP_DECL_OPEN_STATE:
        c = (int32_t) { 0 };
        break;

      default:
        c = tokenizer_getc(tokenizer);
        break;
    }

    do { rc = k_tokenizer_states[tokenizer->state](tokenizer, c); }
      while (rc == TOKENIZER_STATUS_RECONSUME);
  }

}

static int_least32_t
tokenizer_getc(struct tokenizer *tokenizer)
{
  size_t left = tokenizer->input.end - tokenizer->input.p;
  size_t read;
  uint_least32_t c = { 0 };

  if (left > 0 && *tokenizer->input.p == '\0')
    /* grapheme doesn't handle this */
    return *tokenizer->input.p++;

  if (left >= 2
   && tokenizer->input.p[0] == '\r'
   && tokenizer->input.p[1] == '\n') {
    tokenizer->input.p += 2;
    return '\n';
  }

  if (left >= 1
   && tokenizer->input.p[0] == '\r') {
    tokenizer->input.p += 1;
    return '\n';
  }

  if (!left || !(read = grapheme_decode_utf8(tokenizer->input.p, left, &c)))
    return -1;

  tokenizer->input.p += read;
  return c;
}

static int
tokenizer_cmp_consume(struct tokenizer *tokenizer,
                      int (*cmp) (const char *, const char *, size_t),
                      const char *s, size_t slen)
{
  size_t left = tokenizer->input.end - tokenizer->input.p;

  if (left < slen)
    return 0;

  if (!cmp(s, tokenizer->input.p, slen)) {
    tokenizer->input.p += slen;
    return 1;
  }

  return 0;
}

static int
tokenizer_match(struct tokenizer *tokenizer,
                const char *s, size_t slen)
{
  return tokenizer_cmp_consume(tokenizer,
           (int (*) (const char *, const char *, size_t)) memcmp,
           s, slen);
}

static inline char
my_tolower(char c)
{
  return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c;
}

static int
my_strncasecmp(const char *s1, const char *s2, size_t len)
{
  for (size_t i = 0; i < len; i++)
    if (my_tolower(s1[i]) != my_tolower(s2[i]))
      return -1;

  return 0;
}

static int
tokenizer_matchcase(struct tokenizer *tokenizer,
                    const char *s, size_t slen)
{
  return tokenizer_cmp_consume(tokenizer, my_strncasecmp, s, slen);
}

static void
create_doctype(struct tokenizer *tokenizer)
{
  infra_string_unref(tokenizer->doctype.name);
  infra_string_unref(tokenizer->doctype.public_id);
  infra_string_unref(tokenizer->doctype.system_id);

  memset(&tokenizer->doctype, 0, sizeof (tokenizer->doctype));
  tokenizer->doctype.name = infra_string_create();
  tokenizer->doctype.public_id = infra_string_create();
  tokenizer->doctype.system_id = infra_string_create();
}

static void
destroy_tag(struct tag *tag)
{
  infra_string_unref(tag->tagname);
  free(tag);
}

static void
create_tag(struct tokenizer *tokenizer, enum token_type type)
{
  if (tokenizer->tag != NULL)
    destroy_tag(tokenizer->tag);

  tokenizer->tag = malloc(sizeof (struct tag));
  memset(tokenizer->tag, 0, sizeof (*tokenizer->tag));

  tokenizer->tag->tagname = infra_string_create();

  tokenizer->tag_type = type;
}

static void
create_start_tag(struct tokenizer *tokenizer)
{
  create_tag(tokenizer, TOKEN_START_TAG);
}

static void
create_end_tag(struct tokenizer *tokenizer)
{
  create_tag(tokenizer, TOKEN_END_TAG);
}

static void
create_attr(struct tokenizer *tokenizer)
{
  (void) tokenizer;
}

static void
create_comment(struct tokenizer *tokenizer)
{
  infra_string_unref(tokenizer->comment);
  tokenizer->comment = infra_string_create();
}

static void
emit_token(struct tokenizer *tokenizer, union token_data *token_data,
           enum token_type token_type)
{
  (void) tokenizer;
  (void) token_data;
  (void) token_type;
}

static void
emit_tag(struct tokenizer *tokenizer)
{
  emit_token(tokenizer, (union token_data *) tokenizer->tag, tokenizer->tag_type);
}

static inline enum token_type
char_to_type(uint32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      return TOKEN_WHITESPACE;
    default:
      return TOKEN_CHARACTER;
  }
}

static void
emit_doctype(struct tokenizer *tokenizer)
{
  emit_token(tokenizer, (union token_data *) &tokenizer->doctype, TOKEN_DOCTYPE);
}

static void
emit_comment(struct tokenizer *tokenizer)
{
  emit_token(tokenizer, (union token_data *) &tokenizer->comment, TOKEN_COMMENT);
}

static void
emit_character(struct tokenizer *tokenizer, uint32_t c)
{
  emit_token(tokenizer, (union token_data *) &c, char_to_type(c));
}


static enum tokenizer_status
emit_eof(struct tokenizer *tokenizer)
{
  emit_token(tokenizer, NULL, TOKEN_EOF);
  return TOKENIZER_STATUS_EOF;
}

#include "html_tokenizer_states.c"

static void
create_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder)
{
  (void) tokenizer;
  (void) treebuilder;
}

static void
free_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder)
{
  (void) treebuilder;

  infra_string_unref(tokenizer->doctype.name);
  infra_string_unref(tokenizer->doctype.public_id);
  infra_string_unref(tokenizer->doctype.system_id);

  if (tokenizer->tag)
    destroy_tag(tokenizer->tag);
}

void
html_parse(struct dom_document *document,
           const char *input, size_t input_len)
{
  struct tokenizer tokenizer = { 0 };
  struct treebuilder treebuilder = { 0 };

  create_parser(&tokenizer, &treebuilder);

  tokenizer.input.p   = input;
  tokenizer.input.end = &input[input_len];

  tokenizer.state = DATA_STATE;

  tokenizer_mainloop(&tokenizer);

  free_parser(&tokenizer, &treebuilder);
}

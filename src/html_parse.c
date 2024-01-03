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
#include <string.h>
#include <grapheme.h>

#include <wfs/dom.h>
#include <wfs/dom_core.h>
#include <wfs/html_tags.h>
#include <wfs/html.h>

#include <wfs/infra_string.h>
#include <wfs/infra_stack.h>
#include <wfs/infra_namespace.h>

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
  InfraStack *attrs;

  uint16_t localname;

  bool self_closing_fl;
  bool ack_self_closing_fl;
};

enum {
  FOREIGN_TAG_MATH = NUM_HTML_TAG,
  FOREIGN_TAG_SVG,
};

struct attr {
  InfraString *name;
  InfraString *value;
};

struct doctype {
  InfraString *name;
  InfraString *public_id;
  InfraString *system_id;
  bool name_missing : 1;
  bool system_id_missing : 1;
  bool public_id_missing : 1;
  bool force_quirks : 1;
};

union token_data {
  struct tag      tag;
  struct doctype  doctype;
  InfraString *   comment;
  uint32_t        c;
};

struct insertion_location {
  struct dom_node *parent; // phantom reference
  struct dom_node *child; // phantom reference
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

  InfraString *tmpbuf;

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

  struct dom_element *context; // strong reference
  struct dom_document *document; // strong reference

  struct dom_html_head_element *head; // strong reference
  struct dom_html_form_element *form; // strong reference

  InfraStack *open_elements; // -> strong references

  int script_nesting;

  enum treebuilder_mode mode;
  enum treebuilder_mode original_mode;

  bool scripting;
  bool frameset_ok;
  bool foster_parenting;
};

enum treebuilder_status {
  TREEBUILDER_STATUS_REPROCESS = 0,
  TREEBUILDER_STATUS_OK,
  TREEBUILDER_STATUS_IGNORE,
  TREEBUILDER_STATUS_STOP,
};

typedef enum treebuilder_status (*treebuilder_mode_handler) (struct treebuilder *treebuilder,
                                                             union token_data *token_data,
                                                             enum token_type token_type);

static void tokenizer_error(struct tokenizer *tokenizer, const char *msg);
static void treebuilder_error(struct treebuilder *treebuilder);
static void tokenizer_mainloop(struct tokenizer *tokenizer);
static int_least32_t tokenizer_getc(struct tokenizer *tokenizer);

static int tokenizer_cmp_consume(struct tokenizer *tokenizer,
                                  int (*cmp) (const char *, const char *, size_t),
                                  const char *s, size_t slen);
static int tokenizer_match(struct tokenizer *tokenizer, const char *s, size_t slen);
static int tokenizer_matchcase(struct tokenizer *tokenizer, const char *s, size_t slen);

static void push_open_element(struct treebuilder *treebuilder, struct dom_element *elem);
static const struct dom_element *pop_open_element(struct treebuilder *treebuilder);
static inline struct dom_element *current_node(struct treebuilder *treebuilder);
static inline struct dom_element *adjusted_current_node(struct treebuilder *treebuilder);

static void acknowledge_self_closing_fl(struct tag *tag);
static int appropriate_end_tag(struct tokenizer *tokenizer);
static inline int char_ref_in_attr(struct tokenizer *tokenizer);

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

static enum treebuilder_status tree_construction_dispatcher(struct treebuilder *treebuilder,
                                                            union token_data *token_data,
                                                            enum token_type token_type);
/* XXX MathML, SVG */

static struct insertion_location appropriate_place(struct treebuilder *treebuilder,
                                                   struct dom_node *override_target);
static struct dom_element *create_element_for_token(struct treebuilder *treebuilder,
                                                    struct tag *tag,
                                                    enum InfraNamespace namespace,
                                                    struct dom_node *intended_parent);
static void insert_element_at_adj_location(struct treebuilder *treebuilder,
                                                          struct dom_element *element);
static struct dom_element *insert_foreign_element(struct treebuilder *treebuilder,
                                                  struct tag *tag,
                                                  enum InfraNamespace namespace,
                                                  bool only_add_to_element_stack);
static struct dom_html_element *insert_html_element(struct treebuilder *treebuilder,
                                                    struct tag *tag);

static void insert_character(struct treebuilder *treebuilder, uint32_t c);
static void insert_comment(struct treebuilder *treebuilder, InfraString *data,
                           struct insertion_location position);

static enum treebuilder_status generic_raw_text_parse(struct treebuilder *treebuilder,
                                                      struct tag *tag);
static enum treebuilder_status generic_rcdata_parse(struct treebuilder *treebuilder,
                                                    struct tag *tag);

static void create_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder,
                          struct dom_document *document, const char *input,
                          size_t input_len);
static void free_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder);

static enum tokenizer_status data_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status rcdata_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status rawtext_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status tag_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status end_tag_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status tag_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status rcdata_lt_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status rcdata_end_tag_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status rcdata_end_tag_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status before_attr_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status attr_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status after_attr_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status before_attr_value_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status attr_value_double_quoted_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status attr_value_single_quoted_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status attr_value_unquoted_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status after_attr_value_quoted_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status self_closing_start_tag_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status bogus_comment_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status markup_decl_open_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_start_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_start_dash_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_lt_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_lt_bang_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_lt_bang_dash_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_lt_bang_dash_dash_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_end_dash_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_end_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status comment_end_bang_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status doctype_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status before_doctype_name_state(struct tokenizer *tokenizer, int32_t c);
static enum tokenizer_status doctype_name_state(struct tokenizer *tokenizer, int32_t c);

static enum treebuilder_status initial_mode(struct treebuilder *treebuilder,
                                            union token_data *token_data,
                                            enum token_type token_type);
static enum treebuilder_status before_html_mode(struct treebuilder *treebuilder,
                                                union token_data *token_data,
                                                enum token_type token_type);
static enum treebuilder_status before_head_mode(struct treebuilder *treebuilder,
                                                union token_data *token_data,
                                                enum token_type token_type);
static enum treebuilder_status in_head_mode(struct treebuilder *treebuilder,
                                            union token_data *token_data,
                                            enum token_type token_type);
static enum treebuilder_status after_head_mode(struct treebuilder *treebuilder,
                                               union token_data *token_data,
                                               enum token_type token_type);
static enum treebuilder_status in_body_mode(struct treebuilder *treebuilder,
                                            union token_data *token_data,
                                            enum token_type token_type);
static enum treebuilder_status after_body_mode(struct treebuilder *treebuilder,
                                               union token_data *token_data,
                                               enum token_type token_type);
static enum treebuilder_status after_after_body_mode(struct treebuilder *treebuilder,
                                                     union token_data *token_data,
                                                     enum token_type token_type);

/* globals */
static const tokenizer_state_handler k_tokenizer_states[NUM_STATES] = {
  [DATA_STATE] = data_state,
  [RCDATA_STATE] = rcdata_state,
  [RAWTEXT_STATE] = rawtext_state,
  /* ... */
  [TAG_OPEN_STATE] = tag_open_state,
  [END_TAG_OPEN_STATE] = end_tag_open_state,
  [TAG_NAME_STATE] = tag_name_state,
  [RCDATA_LT_STATE] = rcdata_lt_state,
  [RCDATA_END_TAG_OPEN_STATE] = rcdata_end_tag_open_state,
  [RCDATA_END_TAG_NAME_STATE] = rcdata_end_tag_name_state,
  /* ... */
  [BEFORE_ATTR_NAME_STATE] = before_attr_name_state,
  [ATTR_NAME_STATE] = attr_name_state,
  [AFTER_ATTR_NAME_STATE] = after_attr_name_state,
  [BEFORE_ATTR_VALUE_STATE] = before_attr_value_state,
  [ATTR_VALUE_DOUBLE_QUOTED_STATE] = attr_value_double_quoted_state,
  [ATTR_VALUE_SINGLE_QUOTED_STATE] = attr_value_single_quoted_state,
  [ATTR_VALUE_UNQUOTED_STATE] = attr_value_unquoted_state,
  [AFTER_ATTR_VALUE_QUOTED_STATE] = after_attr_value_quoted_state,
  [SELF_CLOSING_START_TAG_STATE] = self_closing_start_tag_state,
  [BOGUS_COMMENT_STATE] = bogus_comment_state,
  [MARKUP_DECL_OPEN_STATE] = markup_decl_open_state,
  [COMMENT_START_STATE] = comment_start_state,
  [COMMENT_START_DASH_STATE] = comment_start_dash_state,
  [COMMENT_STATE] = comment_state,
  [COMMENT_LT_STATE] = comment_lt_state,
  [COMMENT_LT_BANG_STATE] = comment_lt_bang_state,
  [COMMENT_LT_BANG_DASH_STATE] = comment_lt_bang_dash_state,
  [COMMENT_LT_BANG_DASH_DASH_STATE] = comment_lt_bang_dash_dash_state,
  [COMMENT_END_DASH_STATE] = comment_end_dash_state,
  [COMMENT_END_STATE] = comment_end_state,
  [COMMENT_END_BANG_STATE] = comment_end_bang_state,
  [DOCTYPE_STATE] = doctype_state,
  [BEFORE_DOCTYPE_NAME_STATE] = before_doctype_name_state,
  [DOCTYPE_NAME_STATE] = doctype_name_state,
  /* ... */
};

static const treebuilder_mode_handler k_treebuilder_modes[NUM_MODES] = {
  [INITIAL_MODE]          = initial_mode,
  [BEFORE_HTML_MODE]      = before_html_mode,
  [BEFORE_HEAD_MODE]      = before_head_mode,
  [IN_HEAD_MODE]          = in_head_mode,
  /* ... */
  [AFTER_HEAD_MODE]       = after_head_mode,
  [IN_BODY_MODE]          = in_body_mode,
  /* ... */
  [AFTER_BODY_MODE]       = after_body_mode,
  [AFTER_AFTER_BODY_MODE] = after_after_body_mode,
  /* ... */
};

static void
tokenizer_error(struct tokenizer *tokenizer, const char *msg)
{
  (void) tokenizer;
  printf("%s\n", msg);
}

static void
treebuilder_error(struct treebuilder *treebuilder)
{
  (void) treebuilder;
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
push_open_element(struct treebuilder *treebuilder, struct dom_element *elem)
{
  infra_stack_push(treebuilder->open_elements,
    dom_strong_ref_object(elem));
}

static const struct dom_element *
pop_open_element(struct treebuilder *treebuilder)
{
  struct dom_element *popped = infra_stack_pop(treebuilder->open_elements);

  dom_strong_unref_object(popped);

  return popped;
}

static inline struct dom_element *
current_node(struct treebuilder *treebuilder)
{
  return infra_stack_peek(treebuilder->open_elements);
}

static inline struct dom_element *
adjusted_current_node(struct treebuilder *treebuilder)
{
  if (treebuilder->context != NULL
   && treebuilder->open_elements->size == 1)
    return treebuilder->context;

  return current_node(treebuilder);
}

static void
acknowledge_self_closing_fl(struct tag *tag)
{
  tag->ack_self_closing_fl = true;
}

static int
appropriate_end_tag(struct tokenizer *tokenizer)
{
  return 1;
}

static inline int
char_ref_in_attr(struct tokenizer *tokenizer)
{
  return (tokenizer->ret_state == ATTR_VALUE_DOUBLE_QUOTED_STATE
       || tokenizer->ret_state == ATTR_VALUE_SINGLE_QUOTED_STATE
       || tokenizer->ret_state == ATTR_VALUE_UNQUOTED_STATE);
}

static void
create_doctype(struct tokenizer *tokenizer)
{
  infra_string_unref(tokenizer->doctype.name);
  infra_string_unref(tokenizer->doctype.public_id);
  infra_string_unref(tokenizer->doctype.system_id);

  memset(&tokenizer->doctype, 0, sizeof (tokenizer->doctype));

  tokenizer->doctype.name      = infra_string_create();
  tokenizer->doctype.public_id = infra_string_create();
  tokenizer->doctype.system_id = infra_string_create();
}

static void
destroy_tag(struct tag *tag)
{
  infra_string_unref(tag->tagname);

  INFRA_STACK_FOREACH(tag->attrs, i) {
    struct attr *attr = tag->attrs->items[i];

    infra_string_unref(attr->name);
    infra_string_unref(attr->value);

    free(attr);
  }

  infra_stack_free(tag->attrs);

  free(tag);
}

static void
create_tag(struct tokenizer *tokenizer, enum token_type type)
{
  if (tokenizer->tag != NULL) {
    destroy_tag(tokenizer->tag);
    tokenizer->tag = NULL;
  }

  tokenizer->tag = malloc(sizeof (struct tag));
  memset(tokenizer->tag, 0, sizeof (*tokenizer->tag));

  tokenizer->tag->tagname = infra_string_create();
  tokenizer->tag->attrs = infra_stack_create();

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
  struct attr *attr = malloc(sizeof (*attr));
  memset(attr, 0, sizeof (*attr));

  attr->name = infra_string_create();
  attr->value = infra_string_create();

  infra_stack_push(tokenizer->tag->attrs, attr);
  tokenizer->attr = attr;
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
  struct treebuilder *treebuilder = tokenizer->treebuilder;
  enum treebuilder_status rc = TREEBUILDER_STATUS_OK;

  do { rc = tree_construction_dispatcher(treebuilder, token_data, token_type); }
    while (rc == TREEBUILDER_STATUS_REPROCESS);
}

static void
emit_tag(struct tokenizer *tokenizer)
{
  /* XXX Support other namespaces */
  for (uint16_t i = HTML_TAG_HTML; i < NUM_HTML_TAG; i++)
  {
    if (!strcmp(k_html_tag_names[i], tokenizer->tag->tagname->data))
    {
      tokenizer->tag->localname = i;
      break;
    }
  }

  if (tokenizer->tag->localname == 0)
  {
    if (!strcmp("math", tokenizer->tag->tagname->data)) {
      tokenizer->tag->localname = FOREIGN_TAG_MATH;
    } else if (!strcmp("svg", tokenizer->tag->tagname->data)) {
      tokenizer->tag->localname = FOREIGN_TAG_SVG;
    } else if (strcmp("image", tokenizer->tag->tagname->data) == 0
            && tokenizer->treebuilder->mode == IN_BODY_MODE) {
      tokenizer->tag->localname = HTML_TAG_IMG;
    }
  }

  emit_token(tokenizer, (union token_data *) tokenizer->tag, tokenizer->tag_type);
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


static enum treebuilder_status
tree_construction_dispatcher(struct treebuilder *treebuilder,
                             union token_data *token_data,
                             enum token_type token_type)
{
  /* XXX Foreign content!!! */
  return k_treebuilder_modes[treebuilder->mode](treebuilder, token_data, token_type);
}

static struct insertion_location
appropriate_place(struct treebuilder *treebuilder, struct dom_node *override_target)
{
  struct insertion_location location = { 0 };
  struct dom_node *target = NULL;
  struct dom_element *elem = (struct dom_element *) target;

  target = override_target != NULL
         ? override_target
         : (struct dom_node *) current_node(treebuilder);

  if (treebuilder->foster_parenting
   && DOM_IMPLEMENTS(target, html_element)
   && (elem->namespace == INFRA_NAMESPACE_HTML)
   && (elem->local_name == HTML_TAG_TABLE
    || elem->local_name == HTML_TAG_TBODY
    || elem->local_name == HTML_TAG_TFOOT
    || elem->local_name == HTML_TAG_THEAD)) {
    struct dom_html_template_element *template = NULL;
    struct dom_html_table_element *table = NULL;
    int template_ind = -1;
    int table_ind = -1;

    INFRA_STACK_FOREACH(treebuilder->open_elements, i) {
      struct dom_element *elem = treebuilder->open_elements->items[i];

      if (elem->namespace == INFRA_NAMESPACE_HTML
       && elem->local_name == HTML_TAG_TEMPLATE) {
        template = (struct dom_html_template_element *) elem;
        template_ind = i;
        continue;
      }

      if (elem->namespace == INFRA_NAMESPACE_HTML
       && elem->local_name == HTML_TAG_TABLE) {
        table = (struct dom_html_table_element *) elem;
        table_ind = i;
        continue;
      }
    }

    if (template != NULL && (table == NULL || template_ind < table_ind)) {
      location.parent = (struct dom_node *) template->template_contents;
      location.child  = NULL;
      goto done;
    }

    if (table == NULL) {
      /* fragment case */
      location.parent = treebuilder->open_elements->items[0];
      location.child  = NULL;
      goto done;
    }

    if (((struct dom_node *) table)->parent != NULL) {
      location.parent = ((struct dom_node *) table)->parent;
      location.child  = (struct dom_node *) table;
      goto done;
    }

    location.parent = treebuilder->open_elements->items[table_ind - 1];
    location.child  = NULL;
  } else {
    location.parent = (struct dom_node *) target;
    location.child  = NULL;
  }

done:
  if (DOM_IMPLEMENTS(location.parent, html_template_element)) {
    location.parent = (struct dom_node *)
     ((struct dom_html_template_element *) target)->template_contents;
    location.child = NULL;
  }

  return location;
}

static struct dom_element *
create_element_for_token(struct treebuilder *treebuilder, struct tag *tag,
                         enum InfraNamespace namespace,
                         struct dom_node *intended_parent)
{
  dom_strong_ref_object(intended_parent);

  struct dom_document *document =
    dom_strong_ref_object(intended_parent->node_document);
  uint16_t local_name = tag->localname;
  struct dom_element *element = NULL;
  /*
   * XXX: uninterned local names (mostly valid custom element names)
   * XXX: stub for custom elements; create dummy definition and generate
   * internal element index
   */

  bool exec_script = false;

  element = dom_create_element_interned(document, local_name, namespace,
    NULL, NULL, exec_script);

  INFRA_STACK_FOREACH(tag->attrs, i) {
    struct attr *on_token = tag->attrs->items[i];

    // if
  }

  dom_strong_unref_object(document);
  dom_strong_unref_object(intended_parent);

  return element;
}

static void
insert_element_at_adj_location(struct treebuilder *treebuilder,
                               struct dom_element *element)
{
  struct insertion_location location = appropriate_place(treebuilder, NULL);

  /* XXX Check if possible */
  dom_insert_node(location.parent, (struct dom_node *) element, location.child, false);
}

static struct dom_element *
insert_foreign_element(struct treebuilder *treebuilder,
                       struct tag *tag,
                       enum InfraNamespace namespace,
                       bool only_add_to_element_stack)
{
  struct insertion_location location = appropriate_place(treebuilder, NULL);
  struct dom_element *element = create_element_for_token(treebuilder, tag,
                                 namespace, location.parent);

  if (!only_add_to_element_stack)
    insert_element_at_adj_location(treebuilder, element);

  push_open_element(treebuilder, element);

  return element;
}

static struct dom_html_element *
insert_html_element(struct treebuilder *treebuilder,
                    struct tag *tag)
{
  return (struct dom_html_element *)
    insert_foreign_element(treebuilder, tag, INFRA_NAMESPACE_HTML, false);
}

static void
insert_character(struct treebuilder *treebuilder, uint32_t c)
{
  (void) treebuilder;
  (void) c;
}

static void
insert_comment(struct treebuilder *treebuilder, InfraString *data,
               struct insertion_location position)
{
  if (position.parent == NULL)
    position = appropriate_place(treebuilder, NULL);

  struct dom_comment *comment = DOM_NEW_OBJECT( comment );

  ((struct dom_character_data *) comment)->data = infra_string_ref(data);

  ((struct dom_node *) comment)->node_document =
    dom_weak_ref_object(position.parent->node_document);

  dom_insert_node(position.parent, (struct dom_node *) comment,
   position.child, false);
}

static enum treebuilder_status
generic_raw_text_parse(struct treebuilder *treebuilder,
                       struct tag *tag)
{
  insert_html_element(treebuilder, tag);
  treebuilder->tokenizer->state = RAWTEXT_STATE;
  treebuilder->original_mode = treebuilder->mode;
  treebuilder->mode = TEXT_MODE;

  return TREEBUILDER_STATUS_OK;
}

static enum treebuilder_status
generic_rcdata_parse(struct treebuilder *treebuilder,
                     struct tag *tag)
{
  insert_html_element(treebuilder, tag);
  treebuilder->tokenizer->state = RCDATA_STATE;
  treebuilder->original_mode = treebuilder->mode;
  treebuilder->mode = TEXT_MODE;

  return TREEBUILDER_STATUS_OK;
}

static void
create_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder,
              struct dom_document *document, const char *input, size_t input_len)
{
  tokenizer->treebuilder = treebuilder;
  treebuilder->tokenizer = tokenizer;

  tokenizer->input.p   = input;
  tokenizer->input.end = &input[input_len];

  treebuilder->document = dom_strong_ref_object(document);
  treebuilder->open_elements = infra_stack_create();
}

static void
free_parser(struct tokenizer *tokenizer, struct treebuilder *treebuilder)
{
  (void) treebuilder;

  infra_string_unref(tokenizer->doctype.name);
  infra_string_unref(tokenizer->doctype.public_id);
  infra_string_unref(tokenizer->doctype.system_id);

  infra_string_unref(tokenizer->comment);

  if (tokenizer->tag != NULL)
    destroy_tag(tokenizer->tag);

  while (treebuilder->open_elements->size > 0)
    pop_open_element(treebuilder);
  infra_stack_free(treebuilder->open_elements);

  dom_strong_unref_object(treebuilder->head);
  dom_strong_unref_object(treebuilder->form);

  dom_strong_unref_object(treebuilder->document);
}

#include "html_tokenizer_states.c"

#include "html_treebuilder_modes.c"

void
html_parse(struct dom_document *document,
           const char *input, size_t input_len)
{
  struct tokenizer tokenizer = { 0 };
  struct treebuilder treebuilder = { 0 };

  create_parser(&tokenizer, &treebuilder,
                document,
                input, input_len);

  tokenizer.state  = DATA_STATE;
  treebuilder.mode = INITIAL_MODE;

  tokenizer_mainloop(&tokenizer);

  free_parser(&tokenizer, &treebuilder);
}

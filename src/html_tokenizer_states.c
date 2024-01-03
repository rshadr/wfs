static enum tokenizer_status
data_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '&':
      tokenizer->ret_state = DATA_STATE;
      tokenizer->state = CHAR_REF_STATE;
      return TOKENIZER_STATUS_OK;

    case '<':
      tokenizer->state = TAG_OPEN_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      emit_character(tokenizer, c);
      return TOKENIZER_STATUS_OK;

    case -1:
      return TOKENIZER_STATUS_EOF;

    default:
      emit_character(tokenizer, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
rcdata_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '&':
      tokenizer->ret_state = RCDATA_STATE;
      tokenizer->state = CHAR_REF_STATE;
      return TOKENIZER_STATUS_OK;

    case '<':
      tokenizer->state = RCDATA_LT_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      emit_character(tokenizer, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      return emit_eof(tokenizer);

    default:
      emit_character(tokenizer, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
rawtext_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '<':
      tokenizer->state = RAWTEXT_LT_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      emit_character(tokenizer, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      return emit_eof(tokenizer);

    default:
      emit_character(tokenizer, c);
      return TOKENIZER_STATUS_OK;
  }
}

/* ... */

static enum tokenizer_status
tag_open_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_alpha(c)) {
    create_start_tag(tokenizer);
    tokenizer->state = TAG_NAME_STATE;
    return TOKENIZER_STATUS_RECONSUME;
  }

  switch (c) {
    case '!':
      tokenizer->state = MARKUP_DECL_OPEN_STATE;
      return TOKENIZER_STATUS_OK;

    case '/':
      tokenizer->state = END_TAG_OPEN_STATE;
      return TOKENIZER_STATUS_OK;

    case '?':
      tokenizer_error(tokenizer, "unexpected-question-mark-instead-of-tag-name");
      create_comment(tokenizer);
      tokenizer->state = BOGUS_COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;

    case -1:
      tokenizer_error(tokenizer, "eof-before-tag-name");
      emit_character(tokenizer, '<');
      return emit_eof(tokenizer);

    default:
      tokenizer_error(tokenizer, "invalid-first-character-of-tag-name");
      tokenizer->state = DATA_STATE;
      emit_character(tokenizer, '<');
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
end_tag_open_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_alpha(c)) {
    create_end_tag(tokenizer);
    tokenizer->state = TAG_NAME_STATE;
    return TOKENIZER_STATUS_RECONSUME;
  }

  switch (c) {
    case '>':
      tokenizer_error(tokenizer, "missing-end-tag-name");
      tokenizer->state = DATA_STATE;
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-before-tag-name");
      emit_character(tokenizer, '<');
      emit_character(tokenizer, '/');
      return emit_eof(tokenizer);

    default:
      tokenizer_error(tokenizer, "invalid-first-character-of-tag-name");
      create_comment(tokenizer);
      tokenizer->state = BOGUS_COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
tag_name_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_upper_alpha(c)) {
    infra_string_put_char(tokenizer->tag->tagname, c | 0x20);
    return TOKENIZER_STATUS_OK;
  }

  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      tokenizer->state = BEFORE_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    case '/':
      tokenizer->state = SELF_CLOSING_START_TAG_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = DATA_STATE;
      emit_tag(tokenizer);
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->tag->tagname, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      infra_string_put_codepoint(tokenizer->tag->tagname, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
rcdata_lt_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '/':
      infra_string_zero(tokenizer->tmpbuf);
      tokenizer->state = RCDATA_END_TAG_OPEN_STATE;
      return TOKENIZER_STATUS_OK;

    default:
      emit_character(tokenizer, '<');
      tokenizer->state = RCDATA_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
rcdata_end_tag_open_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_alpha(c)) {
    create_end_tag(tokenizer);
    tokenizer->state = RCDATA_END_TAG_NAME_STATE;
    return TOKENIZER_STATUS_RECONSUME;
  }

  switch (c) {
    default:
      emit_character(tokenizer, '<');
      emit_character(tokenizer, '/');
      tokenizer->state = RCDATA_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
rcdata_end_tag_name_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_upper_alpha(c)) {
    infra_string_put_char(tokenizer->tag->tagname, c | 0x20);
    infra_string_put_char(tokenizer->tmpbuf, c);
    return TOKENIZER_STATUS_OK;
  }

  if (ascii_is_lower_alpha(c)) {
    infra_string_put_char(tokenizer->tag->tagname, c);
    infra_string_put_char(tokenizer->tmpbuf, c);
    return TOKENIZER_STATUS_OK;
  }

  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      if (appropriate_end_tag(tokenizer)) {
        tokenizer->state = BEFORE_ATTR_NAME_STATE;
        return TOKENIZER_STATUS_OK;
      }
      goto anything_else;

    case '/':
      if (appropriate_end_tag(tokenizer)) {
        tokenizer->state = SELF_CLOSING_START_TAG_STATE;
        return TOKENIZER_STATUS_OK;
      }
      goto anything_else;

      case '>':
        if (appropriate_end_tag(tokenizer)) {
          tokenizer->state = DATA_STATE;
          emit_tag(tokenizer);
          return TOKENIZER_STATUS_OK;
        }
        goto anything_else;

anything_else:
      default:
        emit_character(tokenizer, '<');
        emit_character(tokenizer, '/');
        /* XXX foreach in tmpbuf */
        tokenizer->state = RCDATA_STATE;
        return TOKENIZER_STATUS_RECONSUME;
  }
}

/* ... */

static enum tokenizer_status
before_attr_name_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      return TOKENIZER_STATUS_IGNORE;

    case '/': case '>': case -1:
      tokenizer->state = AFTER_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;

    case '=':
      tokenizer_error(tokenizer, "unexpected-equals-sign-before-attribute-name");
      create_attr(tokenizer);
      infra_string_put_char(tokenizer->attr->value, c);
      tokenizer->state = ATTR_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    default:
      create_attr(tokenizer);
      tokenizer->state = ATTR_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
attr_name_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_upper_alpha(c)) {
    infra_string_put_char(tokenizer->attr->name, c | 0x20);
    return TOKENIZER_STATUS_OK;
  }

  switch (c) {
    case '\t': case '\n': case '\f': case ' ': case '/': case '>': case -1:
      tokenizer->state = AFTER_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;

    case '=':
      tokenizer->state = BEFORE_ATTR_VALUE_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->attr->name, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case '\"': case '\'': case '<':
      tokenizer_error(tokenizer, "unexpected-character-in-attribute-name");
      goto anything_else;

anything_else:
    default:
      infra_string_put_codepoint(tokenizer->attr->name, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
after_attr_name_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      return TOKENIZER_STATUS_IGNORE;

    case '/':
      tokenizer->state = SELF_CLOSING_START_TAG_STATE;
      return TOKENIZER_STATUS_OK;

    case '=':
      tokenizer->state = BEFORE_ATTR_VALUE_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = DATA_STATE;
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      create_attr(tokenizer);
      tokenizer->state = ATTR_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
before_attr_value_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      return TOKENIZER_STATUS_IGNORE;

    case '\"':
      tokenizer->state = ATTR_VALUE_DOUBLE_QUOTED_STATE;
      return TOKENIZER_STATUS_OK;

    case '\'':
      tokenizer->state = ATTR_VALUE_SINGLE_QUOTED_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer_error(tokenizer, "missing-attribute-value");
      tokenizer->state = DATA_STATE;
      emit_tag(tokenizer);
      return TOKENIZER_STATUS_OK;

    default:
      tokenizer->state = ATTR_VALUE_UNQUOTED_STATE;
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
attr_value_double_quoted_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\"':
      tokenizer->state = AFTER_ATTR_VALUE_QUOTED_STATE;
      return TOKENIZER_STATUS_OK;

    case '&':
      tokenizer->ret_state = ATTR_VALUE_DOUBLE_QUOTED_STATE;
      tokenizer->state = CHAR_REF_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->attr->value, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      infra_string_put_codepoint(tokenizer->attr->value, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
attr_value_single_quoted_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\'':
      tokenizer->state = AFTER_ATTR_VALUE_QUOTED_STATE;
      return TOKENIZER_STATUS_OK;

    case '&':
      tokenizer->ret_state = ATTR_VALUE_SINGLE_QUOTED_STATE;
      tokenizer->state = CHAR_REF_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->attr->value, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      infra_string_put_codepoint(tokenizer->attr->value, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
attr_value_unquoted_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      tokenizer->state = BEFORE_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    case '&':
      tokenizer->ret_state = ATTR_VALUE_UNQUOTED_STATE;
      tokenizer->state = CHAR_REF_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = DATA_STATE;
      emit_tag(tokenizer);
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->attr->value, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case '\"': case '\'': case '<': case '=': case '`':
      tokenizer_error(tokenizer, "unexpected-character-in-unquoted-attribute-value");
      goto anything_else;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

anything_else:
    default:
      infra_string_put_codepoint(tokenizer->attr->value, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
after_attr_value_quoted_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      tokenizer->state = BEFORE_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_OK;
  
    case '/':
      tokenizer->state = SELF_CLOSING_START_TAG_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = DATA_STATE;
      emit_tag(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      tokenizer_error(tokenizer, "missing-whitespace-between-attributes");
      tokenizer->state = BEFORE_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
self_closing_start_tag_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '>':
      tokenizer->tag->self_closing_fl = true;
      tokenizer->state = DATA_STATE;
      emit_tag(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-tag");
      return emit_eof(tokenizer);

    default:
      tokenizer_error(tokenizer, "unexpected-solidus-in-tag");
      tokenizer->state = BEFORE_ATTR_NAME_STATE;
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
bogus_comment_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '>':
      tokenizer->state = DATA_STATE;
      emit_comment(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->comment, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    default:
      infra_string_put_codepoint(tokenizer->comment, c);
      return TOKENIZER_STATUS_OK;
  }
}

#define S(cstring) (cstring), sizeof ((cstring)) - 1

static enum tokenizer_status
markup_decl_open_state(struct tokenizer *tokenizer, int32_t c)
{
  (void) c;

  if (tokenizer_match(tokenizer, S("--"))) {
    create_comment(tokenizer);
    tokenizer->state = COMMENT_START_STATE;
    return TOKENIZER_STATUS_OK;
  }

  if (tokenizer_matchcase(tokenizer, S("DOCTYPE"))) {
    tokenizer->state = DOCTYPE_STATE;
    return TOKENIZER_STATUS_OK;
  }

  if (tokenizer_match(tokenizer, S("[CDATA["))) {
    /* XXX */
    return TOKENIZER_STATUS_OK;
  }

  tokenizer_error(tokenizer, "incorrectly-opened-comment");
  create_comment(tokenizer);
  tokenizer->state = BOGUS_COMMENT_STATE;
  return TOKENIZER_STATUS_OK;
}

#undef S

static enum tokenizer_status
comment_start_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      tokenizer->state = COMMENT_START_DASH_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer_error(tokenizer, "abrupt-closing-of-empty-comment");
      tokenizer->state = DATA_STATE;
      emit_comment(tokenizer);
      return TOKENIZER_STATUS_OK;

    default:
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_start_dash_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      tokenizer->state = COMMENT_END_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer_error(tokenizer, "abrupt-closing-of-empty-comment");
      tokenizer->state = DATA_STATE;
      emit_comment(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-comment");
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_char(tokenizer->comment, '-');
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '<':
      infra_string_put_char(tokenizer->comment, c);
      tokenizer->state = COMMENT_LT_STATE;
      return TOKENIZER_STATUS_OK;

    case '-':
      tokenizer->state = COMMENT_END_DASH_STATE;
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->comment, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-comment");
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_codepoint(tokenizer->comment, c);
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
comment_lt_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '!':
      infra_string_put_char(tokenizer->comment, c);
      tokenizer->state = COMMENT_LT_BANG_STATE;
      return TOKENIZER_STATUS_OK;

    case '<':
      infra_string_put_codepoint(tokenizer->comment, c);
      return TOKENIZER_STATUS_OK;

    default:
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_lt_bang_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      tokenizer->state = COMMENT_LT_BANG_DASH_STATE;
      return TOKENIZER_STATUS_OK;

    default:
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_lt_bang_dash_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      tokenizer->state = COMMENT_LT_BANG_DASH_DASH_STATE;
      return TOKENIZER_STATUS_OK;

    default:
      tokenizer->state = COMMENT_END_DASH_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_lt_bang_dash_dash_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '>': case -1:
      tokenizer->state = COMMENT_END_STATE;
      return TOKENIZER_STATUS_RECONSUME;

    default:
      tokenizer_error(tokenizer, "nested-comment");
      tokenizer->state = COMMENT_END_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_end_dash_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      tokenizer->state = COMMENT_END_STATE;
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-comment");
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_char(tokenizer->comment, '-');
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_end_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '>':
      tokenizer->state = DATA_STATE;
      emit_comment(tokenizer);
      return TOKENIZER_STATUS_OK;

    case '!':
      tokenizer->state = COMMENT_END_BANG_STATE;
      return TOKENIZER_STATUS_OK;

    case '-':
      infra_string_put_char(tokenizer->comment, '-');
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-comment");
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_char(tokenizer->comment, '-');
      infra_string_put_char(tokenizer->comment, '-');
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
comment_end_bang_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '-':
      infra_string_put_char(tokenizer->comment, '-');
      infra_string_put_char(tokenizer->comment, '-');
      infra_string_put_char(tokenizer->comment, '!');
      tokenizer->state = COMMENT_END_DASH_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer_error(tokenizer, "incorrectly-closed-comment");
      tokenizer->state = DATA_STATE;
      emit_comment(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-comment");
      emit_comment(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_char(tokenizer->comment, '-');
      infra_string_put_char(tokenizer->comment, '-');
      infra_string_put_char(tokenizer->comment, '!');
      tokenizer->state = COMMENT_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
doctype_state(struct tokenizer *tokenizer, int32_t c)
{
  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      tokenizer->state = BEFORE_DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = BEFORE_DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;

    case -1:
      tokenizer_error(tokenizer, "eof-in-doctype");
      create_doctype(tokenizer);
      tokenizer->doctype.force_quirks = true;
      emit_doctype(tokenizer);
      return emit_eof(tokenizer);

    default:
      tokenizer_error(tokenizer, "missing-whitespace-before-doctype-name");
      tokenizer->state = BEFORE_DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_RECONSUME;
  }
}

static enum tokenizer_status
before_doctype_name_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_upper_alpha(c)) {
    create_doctype(tokenizer);
    infra_string_put_char(tokenizer->doctype.name, c | 0x20);
    tokenizer->state = DOCTYPE_NAME_STATE;
    return TOKENIZER_STATUS_OK;
  }

  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      return TOKENIZER_STATUS_IGNORE;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      create_doctype(tokenizer);
      infra_string_put_codepoint(tokenizer->doctype.name, 0xFFFD);
      tokenizer->state = DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer_error(tokenizer, "missing-doctype-name");
      create_doctype(tokenizer);
      tokenizer->doctype.force_quirks = true;
      tokenizer->state = DATA_STATE;
      emit_doctype(tokenizer);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-doctype");
      create_doctype(tokenizer);
      tokenizer->doctype.force_quirks = true;
      emit_doctype(tokenizer);
      return emit_eof(tokenizer);

    default:
      create_doctype(tokenizer);
      infra_string_put_codepoint(tokenizer->doctype.name, c);
      tokenizer->state = DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_OK;
  }
}

static enum tokenizer_status
doctype_name_state(struct tokenizer *tokenizer, int32_t c)
{
  if (ascii_is_upper_alpha(c)) {
    infra_string_put_char(tokenizer->doctype.name, c | 0x20);
    return TOKENIZER_STATUS_OK;
  }

  switch (c) {
    case '\t': case '\n': case '\f': case ' ':
      tokenizer->state = AFTER_DOCTYPE_NAME_STATE;
      return TOKENIZER_STATUS_OK;

    case '>':
      tokenizer->state = DATA_STATE;
      emit_doctype(tokenizer);
      return TOKENIZER_STATUS_OK;

    case '\0':
      tokenizer_error(tokenizer, "unexpected-null-character");
      infra_string_put_codepoint(tokenizer->doctype.name, 0xFFFD);
      return TOKENIZER_STATUS_OK;

    case -1:
      tokenizer_error(tokenizer, "eof-in-doctype");
      tokenizer->doctype.force_quirks = true;
      emit_doctype(tokenizer);
      return emit_eof(tokenizer);

    default:
      infra_string_put_codepoint(tokenizer->doctype.name, c);
      return TOKENIZER_STATUS_OK;
  }
}

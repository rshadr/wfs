static enum treebuilder_status
initial_mode(struct treebuilder *treebuilder,
             union token_data *token_data,
             enum token_type token_type)
{
  if (token_type == TOKEN_WHITESPACE)
  {
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location) { (struct dom_node *) treebuilder->document, NULL });
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_DOCTYPE)
  {
    if (strcmp("html", token_data->doctype.name->data)
     || !token_data->doctype.public_id_missing
     || !token_data->doctype.system_id_missing
     || strcmp("about:legacy-compat", token_data->doctype.system_id->data))
      treebuilder_error(treebuilder);

    struct doctype *token = &token_data->doctype;
    struct dom_document_type *doctype = dom_alloc_object( DOM_INTERFACE(document_type) );

    dom_append_node((struct dom_node *) treebuilder->document, (struct dom_node *) doctype);

    doctype->name      = infra_string_ref(token->name);
    doctype->public_id = infra_string_ref(token->public_id);
    doctype->system_id = infra_string_ref(token->system_id);

    /* XXX Quirky DOCTYPES */

    treebuilder->mode = BEFORE_HTML_MODE;
    return TREEBUILDER_STATUS_OK;
  }

  /* XXX parse error */
  treebuilder->mode = BEFORE_HTML_MODE;
  return TREEBUILDER_STATUS_REPROCESS;
}

static enum treebuilder_status
before_html_mode(struct treebuilder *treebuilder,
                 union token_data *token_data,
                 enum token_type token_type)
{
  printf("Ah?\n");
  if (token_type == TOKEN_DOCTYPE)
  {
    treebuilder_error(treebuilder);
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location){(struct dom_node *) treebuilder->document, NULL});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_WHITESPACE)
  {
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_START_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HTML: {
        /* XXX create element */
        printf("got it\n");
        treebuilder->mode = BEFORE_HEAD_MODE;
        return TREEBUILDER_STATUS_OK;
      }

      default:
        printf("Ceh\n");
        goto anything_else;
    }
  }

  if (token_type == TOKEN_END_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HEAD: case HTML_TAG_BODY: case HTML_TAG_HTML:
      case HTML_TAG_BR:
        goto anything_else;

      default:
        treebuilder_error(treebuilder);
        return TREEBUILDER_STATUS_IGNORE;
    }
  }

anything_else: {
    /* XXX create element */
    treebuilder->mode = BEFORE_HEAD_MODE;
    return TREEBUILDER_STATUS_REPROCESS;
  }
}

static enum treebuilder_status
before_head_mode(struct treebuilder *treebuilder,
                 union token_data *token_data,
                 enum token_type token_type)
{
  if (token_type == TOKEN_WHITESPACE)
  {
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment, (struct insertion_location){0});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_DOCTYPE)
  {
    treebuilder_error(treebuilder);
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_START_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HTML:
        /* XXX process in body */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_HEAD: {
        struct dom_html_head_element *head = NULL;
        /* XXX insert, head ptr */
        treebuilder->mode = IN_HEAD_MODE;
        return TREEBUILDER_STATUS_OK;
      }

      default:
        goto anything_else;
    }
  }

  if (token_type == TOKEN_END_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HEAD: case HTML_TAG_BODY: case HTML_TAG_HTML:
      case HTML_TAG_BR:
        goto anything_else;

      default:
        treebuilder_error(treebuilder);
        return TREEBUILDER_STATUS_IGNORE;
    }
  }

anything_else:
  {
    struct dom_html_head_element *head = (struct dom_html_head_element *)
      insert_html_element(treebuilder, &(struct tag){.localname = HTML_TAG_HEAD});

    treebuilder->head = dom_strong_ref_object(head);

    treebuilder->mode = IN_HEAD_MODE;
    return TREEBUILDER_STATUS_REPROCESS;
  }
}

static enum treebuilder_status
in_head_mode(struct treebuilder *treebuilder,
             union token_data *token_data,
             enum token_type token_type)
{
  if (token_type == TOKEN_WHITESPACE)
  {
    insert_character(treebuilder, token_data->c);
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location){0});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_DOCTYPE)
  {
    treebuilder_error(treebuilder);
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_START_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HTML:
        /* XXX in body mode */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_BASE: case HTML_TAG_BASEFONT: case HTML_TAG_BGSOUND:
      case HTML_TAG_LINK:
        /* XXX insert and pop */
        return TREEBUILDER_STATUS_OK;


      case HTML_TAG_META:
        /* XXX */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_TITLE:
        /* XXX generic rcdata algorithm */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_NOSCRIPT:
        if (treebuilder->scripting) {
        /* fallthrough */
      case HTML_TAG_NOFRAMES: case HTML_TAG_STYLE:
          /* XXX generic raw text parse */
          return TREEBUILDER_STATUS_OK;
        } else {
          /* HTML_TAG_SCRIPT; treebuilder->scripting == false */
          insert_html_element(treebuilder, &token_data->tag);
          treebuilder->mode = IN_HEAD_NOSCRIPT_MODE;
          return TREEBUILDER_STATUS_OK;
        }

      case HTML_TAG_SCRIPT:
        /* XXX */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_TEMPLATE:
        /* XXX */
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_HEAD:
        treebuilder_error(treebuilder);
        return TREEBUILDER_STATUS_IGNORE;

      default:
        goto anything_else;
    }
  }

  if (token_type == TOKEN_END_TAG)
  {
    /* XXX */
  }

anything_else: {
    pop_open_element(treebuilder);
    treebuilder->mode = AFTER_HEAD_MODE;
    return TREEBUILDER_STATUS_REPROCESS;
  }
}

static enum treebuilder_status
after_head_mode(struct treebuilder *treebuilder,
                union token_data *token_data,
                enum token_type token_type)
{
  if (token_type == TOKEN_WHITESPACE)
  {
    insert_character(treebuilder, token_data->c);
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location){0});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_START_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HTML:
        /* XXX in body */
        return TREEBUILDER_STATUS_OK;

      default:
        goto anything_else;
    }
  }

  /* XXX */

anything_else: {
    insert_html_element(treebuilder, &(struct tag){.localname = HTML_TAG_BODY});
    treebuilder->mode = IN_BODY_MODE;
    printf("a\n");
    return TREEBUILDER_STATUS_OK;
  }

}

static enum treebuilder_status
in_body_mode(struct treebuilder *treebuilder,
             union token_data *token_data,
             enum token_type token_type)
{
  if (token_type == TOKEN_CHARACTER)
  {
    if (token_data->c == '\0') {
      treebuilder_error(treebuilder);
      return TREEBUILDER_STATUS_IGNORE;
    }

    /* XXX reconstruct active formatting */
    insert_character(treebuilder, token_data->c);
    treebuilder->frameset_ok = false;
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_WHITESPACE)
  {
    /* XXX reconstruct active formatting */
    insert_character(treebuilder, token_data->c);
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location){0});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_DOCTYPE)
  {
    treebuilder_error(treebuilder);
    return TREEBUILDER_STATUS_IGNORE;
  }

  /* ... */

  if (token_type == TOKEN_END_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_TEMPLATE:
        return in_head_mode(treebuilder, token_data, token_type);

      case HTML_TAG_BODY:
        /* XXX */
        treebuilder->mode = AFTER_BODY_MODE;
        return TREEBUILDER_STATUS_OK;

      case HTML_TAG_HTML:
        /* XXX */
        treebuilder->mode = AFTER_BODY_MODE;
        return TREEBUILDER_STATUS_REPROCESS;

      /* ... */

      default:
        /* XXX */
        return TREEBUILDER_STATUS_OK;
    }
  }

/* anything_else: */ {
    /* XXX UNHANDLED! */
    return TREEBUILDER_STATUS_OK;
  }
}

/* ... */

static enum treebuilder_status
after_body_mode(struct treebuilder *treebuilder,
                union token_data *token_data,
                enum token_type token_type)
{
  if (token_type == TOKEN_WHITESPACE)
  {
    return in_body_mode(treebuilder, token_data, token_type);
  }

  if (token_type == TOKEN_COMMENT)
  {
    insert_comment(treebuilder, token_data->comment,
      (struct insertion_location){treebuilder->open_elements->items[0], NULL});
    return TREEBUILDER_STATUS_OK;
  }

  if (token_type == TOKEN_DOCTYPE)
  {
    treebuilder_error(treebuilder);
    return TREEBUILDER_STATUS_IGNORE;
  }

  if (token_type == TOKEN_END_TAG)
  {
    switch (token_data->tag.localname)
    {
      case HTML_TAG_HTML:
        if (treebuilder->context != NULL) {
          treebuilder_error(treebuilder);
          return TREEBUILDER_STATUS_IGNORE;
        }

        treebuilder->mode = AFTER_AFTER_BODY_MODE;
        return TREEBUILDER_STATUS_OK;

      default:
        goto anything_else;
    }
  }

  /* ... */

anything_else: {
    treebuilder_error(treebuilder);
    treebuilder->mode = IN_BODY_MODE;
    return TREEBUILDER_STATUS_REPROCESS;
  }
}

/* ... */

static enum treebuilder_status
after_after_body_mode(struct treebuilder *treebuilder,
                      union token_data *token_data,
                      enum token_type token_type)
{
  /* ... */

  if (token_type == TOKEN_EOF)
  {
    return TREEBUILDER_STATUS_STOP;
  }

  /* anything_else: */ {
    treebuilder_error(treebuilder);
    abort();
    return TREEBUILDER_STATUS_REPROCESS;
  }
}

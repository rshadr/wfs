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

    doctype->name = infra_string_ref(token->name);
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

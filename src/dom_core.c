#include <wfs/dom_core.h>
#include <stdlib.h>

/* START INTERFACES */

DOM_DEFINE_INTERFACE(event_target) {
  .name = "EventTarget",
  .impl_size = sizeof (struct dom_event_target),
};

static void
node_finalizer(DOMObject *obj)
{
  struct dom_node *node = (DOMAny *) obj;

  dom_weak_unref_object(node->node_document);
  dom_weak_unref_object(node->parent);

  if (node->children != NULL) {
    INFRA_STACK_FOREACH(node->children, i)
      dom_strong_unref_object(node->children->items[i]);

    infra_stack_free(node->children);
  }
}

DOM_DEFINE_INTERFACE(node) {
  .name = "Node",
  .parent_interface = DOM_INTERFACE(event_target),
  .impl_size = sizeof (struct dom_node),
  .finalizer = node_finalizer,
};

DOM_DEFINE_INTERFACE(document) {
  .name = "Document",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_document),
};

static void
document_type_finalizer(DOMObject *obj)
{
  struct dom_document_type *doctype = (DOMAny *) obj;

  infra_string_unref(doctype->name);
  infra_string_unref(doctype->public_id);
  infra_string_unref(doctype->system_id);
}

DOM_DEFINE_INTERFACE(document_type) {
  .name = "DocumentType",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_document_type),
  .finalizer = document_type_finalizer,
};

static void
document_fragment_finalizer(DOMObject *obj)
{
  struct dom_document_fragment *fragment = (DOMAny *) obj;

  dom_strong_unref_object(fragment->host);
}

DOM_DEFINE_INTERFACE(document_fragment) {
  .name = "DocumentFragment",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_document_fragment),
  .finalizer = document_fragment_finalizer,
};

static void
element_finalizer(DOMObject *obj)
{
  struct dom_element *elem = (DOMAny *) obj;

  if (elem->attrs != NULL)
  {
    INFRA_STACK_FOREACH(elem->attrs, i)
      dom_strong_unref_object(elem->attrs->items[i]);
  }
}

DOM_DEFINE_INTERFACE(element) {
  .name = "Element",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_element),
  .finalizer = element_finalizer,
};

static void
attr_finalizer(DOMObject *obj)
{
  struct dom_attr *attr = (DOMAny *) obj;

  dom_weak_unref_object(attr->element);
  infra_string_unref(attr->local_name);
  infra_string_unref(attr->value);
}

DOM_DEFINE_INTERFACE(attr) {
  .name = "Attr",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_attr),
  .finalizer = attr_finalizer,
};

static void
character_data_finalizer(DOMObject *obj)
{
  struct dom_character_data *cd = (DOMAny *) obj;

  infra_string_unref(cd->data);
}

DOM_DEFINE_INTERFACE(character_data) {
  .name = "CharacterData",
  .parent_interface = DOM_INTERFACE(node),
  .impl_size = sizeof (struct dom_character_data),
  .finalizer = character_data_finalizer,
};

DOM_DEFINE_INTERFACE(text) {
  .name = "Text",
  .parent_interface = DOM_INTERFACE(character_data),
  .impl_size = sizeof (struct dom_text),
};

DOM_DEFINE_INTERFACE(comment) {
  .name = "Comment",
  .parent_interface = DOM_INTERFACE(character_data),
  .impl_size = sizeof (struct dom_comment),
};

/* END INTERFACES */

/* START ALGORITHMS */
struct dom_node *
dom_pre_insert_node(struct dom_node *parent,
                    struct dom_node *node,
                    struct dom_node *child)
{
  /* XXX Do some safety checks */
  dom_insert_node(parent, node, child, false);
  return node;
}

void
dom_insert_node(struct dom_node *parent,
                struct dom_node *node,
                struct dom_node *child,
                bool suppress_observers)
{
  /* XXX handle document fragments */
  /* XXX adopt node */
  (void) suppress_observers;

  if (child == NULL) {
    node->parent = dom_weak_ref_object(parent);
    infra_stack_push(parent->children,
      dom_strong_ref_object(node));
    return;
  }

  abort();
  /* ... */
  /* Yikes! */
}

void
dom_append_node(struct dom_node *parent, struct dom_node *node)
{
  dom_pre_insert_node(parent, node, NULL);
}

/* XXX Add uninterned version */
struct dom_element *
dom_create_element_interned(struct dom_document *document, uint16_t local_name,
                            enum InfraNamespace namespace,
                            const void *prefix, const void *is,
                            bool sync_custom_elements)
{
  struct dom_element *result = NULL;

  (void) prefix;
  (void) is;
  (void) sync_custom_elements;

  /* XXX custom elements */
  /* XXX non-HTML elements */

  /*
   * XXX: currently, this section only involves the third case in the spec,
   * where there is no custom element definition for the tag name.
   */

  /* otherwise: */ {
    /* XXX: CIRCULAR DEPENDENCY */
    extern const DOMInterface *k_html_element_interfaces[];

    const DOMInterface *interface = k_html_element_interfaces[local_name];

    result = dom_alloc_object( interface );

    result->namespace  = namespace;
    result->local_name = local_name;

    ((struct dom_node *) result)->node_document = dom_weak_ref_object(document);
    ((struct dom_node *) result)->children = infra_stack_create();
  }

  return result;
}

/* END ALGORITHMS */

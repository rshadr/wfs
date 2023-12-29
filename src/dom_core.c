#include <wfs/dom_core.h>

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
  .parent_interface = DOM_INTERFACE(document_type),
  .impl_size = sizeof (struct dom_document_type),
  .finalizer = document_type_finalizer,
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
    infra_stack_push(parent->children, dom_strong_ref_object(node));
    return;
  }

  /* ... */
  /* Yikes! */
}

void
dom_append_node(struct dom_node *parent, struct dom_node *node)
{
  dom_pre_insert_node(parent, node, NULL);
}
/* END ALGORITHMS */

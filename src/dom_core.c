#include <wfs/dom_core.h>

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

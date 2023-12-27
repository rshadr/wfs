#include <wfs/dom_core.h>

DOM_DEFINE_INTERFACE(event_target) {
  .name = "EventTarget",
  .impl_size = sizeof (struct dom_event_target),
};

static void
node_finalizer(DOMObject *obj)
{
  struct dom_node *node = (DOMAny *) obj;

  dom_strong_unref_object(node->node_document);
  dom_strong_unref_object(node->parent);
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

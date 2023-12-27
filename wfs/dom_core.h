#ifndef _LIBWFS_DOM_CORE_H
#define _LIBWFS_DOM_CORE_H

#include <wfs/dom.h>

struct dom_event_target;
struct dom_node;
struct dom_document;

DOM_DECLARE_INTERFACE(event_target);
struct dom_event_target {
  DOMHeader _header;
};

/* ... */

DOM_DECLARE_INTERFACE(node);
struct dom_node {
  struct dom_event_target _base;

  struct dom_document *node_document; // weak reference
  struct dom_node *parent; // weak reference

  void *children;
};

DOM_DECLARE_INTERFACE(document);
struct dom_document {
  struct dom_node _base;
};

#endif /*  _LIBWFS_DOM_CORE_H */

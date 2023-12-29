#ifndef _LIBWFS_DOM_CORE_H
#define _LIBWFS_DOM_CORE_H

#include <stdbool.h>

#include <wfs/dom.h>
#include <wfs/infra_string.h>
#include <wfs/infra_stack.h>

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

  InfraStack *children; // -> strong references
};

DOM_DECLARE_INTERFACE(document);
struct dom_document {
  struct dom_node _base;
};

DOM_DECLARE_INTERFACE(document_type);
struct dom_document_type {
  struct dom_node _base;

  InfraString *name;
  InfraString *public_id;
  InfraString *system_id;
};

DOM_DECLARE_INTERFACE(character_data);
struct dom_character_data {
  struct dom_node _base;

  InfraString *data;
};

DOM_DECLARE_INTERFACE(text);
struct dom_text {
  struct dom_character_data _base;
};

DOM_DECLARE_INTERFACE(comment);
struct dom_comment {
  struct dom_character_data _base;
};

struct dom_node *dom_pre_insert_node(struct dom_node *parent,
                                     struct dom_node *node,
                                     struct dom_node *child);

void dom_insert_node(struct dom_node *parent,
                     struct dom_node *node,
                     struct dom_node *child,
                     bool suppress_observers);

void dom_append_node(struct dom_node *parent,
                     struct dom_node *node);

#endif /*  _LIBWFS_DOM_CORE_H */

#ifndef _LIBWFS_DOM_CORE_H
#define _LIBWFS_DOM_CORE_H

#include <stdbool.h>

#include <wfs/dom.h>
#include <wfs/infra_string.h>
#include <wfs/infra_stack.h>
#include <wfs/infra_namespace.h>

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

DOM_DECLARE_INTERFACE(element);
struct dom_element {
  struct dom_node _base;

  InfraStack *attrs; // -> strong references

  uint16_t local_name;
  enum InfraNamespace namespace;

  uint8_t ce_state;
};

DOM_DECLARE_INTERFACE(attr);
struct dom_attr {
  struct dom_node _base;

  struct dom_element *element; // weak reference
  InfraString *local_name;
  InfraString *value;
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
struct dom_element *dom_create_element_interned(struct dom_document *document,
                                                uint16_t local_name,
                                                enum InfraNamespace namespace,
                                                const void *prefix,
                                                const void *is,
                                                bool sync_custom_elements);
#endif /* _LIBWFS_DOM_CORE_H */

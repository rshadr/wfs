#ifndef _LIBWFS_DOM_HTML_H
#define _LIBWFS_DOM_HTML_H

#include <wfs/dom_core.h>

/* 3.2.2 Elements in the DOM */
DOM_DECLARE_INTERFACE(html_element);
struct dom_html_element {
  struct dom_element _base;
};

DOM_DECLARE_INTERFACE(html_unknown_element);
struct dom_html_unknown_element {
  struct dom_html_element _base;
};

/* 4.1 The document element */
DOM_DECLARE_INTERFACE(html_html_element);
struct dom_html_html_element {
  struct dom_html_element _base;
};

/* 4.2 Document metadata */
DOM_DECLARE_INTERFACE(html_head_element);
struct dom_html_head_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_title_element);
struct dom_html_title_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_base_element);
struct dom_html_base_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_link_element);
struct dom_html_link_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_meta_element);
struct dom_html_meta_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_style_element);
struct dom_html_style_element {
  struct dom_html_element _base;
};

/* 4.3 Sections */
DOM_DECLARE_INTERFACE(html_body_element);
struct dom_html_body_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_heading_element);
struct dom_html_heading_element {
  struct dom_html_element _base;
};

/* 4.4 Grouping content */
DOM_DECLARE_INTERFACE(html_paragraph_element);
struct dom_html_paragraph_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_hr_element);
struct dom_html_hr_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_pre_element);
struct dom_html_pre_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_quote_element);
struct dom_html_quote_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_olist_element);
struct dom_html_olist_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_ulist_element);
struct dom_html_ulist_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_menu_element);
struct dom_html_menu_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_li_element);
struct dom_html_li_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_dlist_element);
struct dom_html_dlist_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_div_element);
struct dom_html_div_element {
  struct dom_html_element _base;
};

/* 4.5 Text-level semantics */
DOM_DECLARE_INTERFACE(html_anchor_element);
struct dom_html_anchor_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(html_span_element);
struct dom_html_span_element {
  struct dom_html_element _base;
};

/* 4.7 Edits */
/* ... */

/* 4.8 Embedded content */
/* ... */

/* 4.9 Tabular data */
DOM_DECLARE_INTERFACE(html_table_element);
struct dom_html_table_element {
  struct dom_html_element _base;
};

/* 4.10 Forms */
DOM_DECLARE_INTERFACE(html_form_element);
struct dom_html_form_element {
  struct dom_html_element _base;
};

/* 4.11 Interactive elements */
/* N/A */

/* 4.12 Scripting */
DOM_DECLARE_INTERFACE(html_script_element);
struct dom_html_script_element {
  struct dom_html_element _base;

  struct dom_document *parser_document; // weak reference
  struct dom_document *preptime_document; // weak reference

  bool force_async : 1;
  bool from_external_file : 1;
  bool ready_parser_exec : 1;
  bool already_started : 1;
  bool delaying_load_ev : 1;
};

DOM_DECLARE_INTERFACE(html_template_element);
struct dom_html_template_element {
  struct dom_html_element _base;
};

DOM_DECLARE_INTERFACE(custom_element_registry);
struct dom_custom_element_registry {
  DOMHeader _header;

  InfraStack *definitions;
  /* XXX when-defined promise map */
  bool elem_def_running;
};


/* 16 Obsolete features */
/* ... */

#endif /* _LIBWFS_DOM_HTML_H */

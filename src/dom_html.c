#include <wfs/dom_html.h>

/* 3.2.2 Elements in the DOM */
DOM_DEFINE_INTERFACE(html_element) {
  .name = "HTMLElement",
  .parent_interface = DOM_INTERFACE(element),
  .impl_size = sizeof (struct dom_html_element),
};

DOM_DEFINE_INTERFACE(html_unknown_element) {
  .name = "HTMLUnknownElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_unknown_element),
};

/* 4.1 The document element */
DOM_DEFINE_INTERFACE(html_html_element) {
  .name = "HTMLHtmlElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_html_element),
};

/* 4.2 Document metadata */
DOM_DEFINE_INTERFACE(html_head_element) {
  .name = "HTMLHeadElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_head_element),
};

DOM_DEFINE_INTERFACE(html_title_element) {
  .name = "HTMLTitleElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_title_element),
};

DOM_DEFINE_INTERFACE(html_base_element) {
  .name = "HTMLBaseElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_base_element),
};

DOM_DEFINE_INTERFACE(html_link_element) {
  .name = "HTMLLinkElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_link_element),
};

DOM_DEFINE_INTERFACE(html_meta_element) {
  .name = "HTMLMetaElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_meta_element),
};

DOM_DEFINE_INTERFACE(html_style_element) {
  .name = "HTMLStyleElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_style_element),
};

/* 4.3 Sections */
DOM_DEFINE_INTERFACE(html_body_element) {
  .name = "HTMLBodyElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_body_element),
};

DOM_DEFINE_INTERFACE(html_heading_element) {
  .name = "HTMLHeadingElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_heading_element),
};

/* 4.4 Grouping content */
DOM_DEFINE_INTERFACE(html_paragraph_element) {
  .name = "HTMLParagraphElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_paragraph_element),
};

DOM_DEFINE_INTERFACE(html_hr_element) {
  .name = "HTMLHRElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_hr_element),
};

DOM_DEFINE_INTERFACE(html_pre_element) {
  .name = "HTMLPreElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_pre_element),
};

DOM_DEFINE_INTERFACE(html_quote_element) {
  .name = "HTMLQuoteElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_quote_element),
};

DOM_DEFINE_INTERFACE(html_olist_element) {
  .name = "HTMLOListElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_olist_element),
};

DOM_DEFINE_INTERFACE(html_ulist_element) {
  .name = "HTMLUListElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_ulist_element),
};

DOM_DEFINE_INTERFACE(html_menu_element) {
  .name = "HTMLMenuElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_menu_element),
};

DOM_DEFINE_INTERFACE(html_li_element) {
  .name = "HTMLLIElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof(struct dom_html_li_element),
};

DOM_DEFINE_INTERFACE(html_dlist_element) {
  .name = "HTMLDListElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_dlist_element),
};

DOM_DEFINE_INTERFACE(html_div_element) {
  .name = "HTMLDivElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_div_element),
};

/* 4.5 Text-level semantics */
DOM_DEFINE_INTERFACE(html_anchor_element) {
  .name = "HTMLAnchorElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_anchor_element),
};

DOM_DEFINE_INTERFACE(html_span_element) {
  .name = "HTMLSpanElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_span_element),
};

/* 4.7 Edits */
/* ... */

/* 4.8 Embedded content */
/* ... */

/* 4.9 Tabular data */
DOM_DEFINE_INTERFACE(html_table_element) {
  .name = "HTMLTableElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_table_element),
};

/* 4.10 Forms */
DOM_DEFINE_INTERFACE(html_form_element) {
  .name = "HTMLFormElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_form_element),
};

/* 4.11 Interactive elements */
/* N/A */

/* 4.12 Scripting */
DOM_DEFINE_INTERFACE(html_script_element) {
  .name = "HTMLScriptElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_script_element),
};

DOM_DEFINE_INTERFACE(html_template_element) {
  .name = "HTMLTemplateElement",
  .parent_interface = DOM_INTERFACE(html_element),
  .impl_size = sizeof (struct dom_html_template_element),
};

/* 16 Obsolete features */
/* ... */

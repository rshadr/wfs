/* 
 * This file is part of the wfs distribution (https://github.com/lauch788/wfs).
 * Copyright (c) 2023 Adrien Ricciardi.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stddef.h>

#include <wfs/dom_html.h>
#include <wfs/html_tags.h>

const char *k_html_tag_names[NUM_HTML_TAG] = {

  /* 4.1 The document element */
  [HTML_TAG_HTML] = "html",

  /* 4.2 Document metadata */
  [HTML_TAG_HEAD]  = "head",
  [HTML_TAG_TITLE] = "title",
  [HTML_TAG_BASE]  = "base",
  [HTML_TAG_LINK]  = "link",
  [HTML_TAG_META]  = "meta",
  [HTML_TAG_STYLE] = "style",

  /* 4.3 Sections */
  [HTML_TAG_BODY]    = "body",
  [HTML_TAG_ARTICLE] = "article",
  [HTML_TAG_SECTION] = "section",
  [HTML_TAG_NAV]     = "nav",
  [HTML_TAG_ASIDE]   = "aside",
  [HTML_TAG_H1]      = "h1",
  [HTML_TAG_H2]      = "h2",
  [HTML_TAG_H3]      = "h3",
  [HTML_TAG_H4]      = "h4",
  [HTML_TAG_H5]      = "h5",
  [HTML_TAG_H6]      = "h6",
  [HTML_TAG_HGROUP]  = "hgroup",
  [HTML_TAG_HEADER]  = "header",
  [HTML_TAG_FOOTER]  = "footer",
  [HTML_TAG_ADDRESS] = "address",

  /* 4.4 Grouping content */
  [HTML_TAG_P]          = "p",
  [HTML_TAG_HR]         = "hr",
  [HTML_TAG_PRE]        = "pre",
  [HTML_TAG_BLOCKQUOTE] = "blockquote",
  [HTML_TAG_OL]         = "ol",
  [HTML_TAG_UL]         = "ul",
  [HTML_TAG_MENU]       = "menu",
  [HTML_TAG_LI]         = "li",
  [HTML_TAG_DL]         = "dl",
  [HTML_TAG_DT]         = "dt",
  [HTML_TAG_DD]         = "dd",
  [HTML_TAG_FIGURE]     = "figure",
  [HTML_TAG_FIGCAPTION] = "figcaption",
  [HTML_TAG_MAIN]       = "main",
  [HTML_TAG_SEARCH]     = "search",
  [HTML_TAG_DIV]        = "div",

  /* 4.5 Text-level semantics */
  [HTML_TAG_A]      = "a",
  [HTML_TAG_EM]     = "em",
  [HTML_TAG_STRONG] = "strong",
  [HTML_TAG_SMALL]  = "small",
  [HTML_TAG_S]      = "s",
  [HTML_TAG_CITE]   = "cite",
  [HTML_TAG_Q]      = "q",
  [HTML_TAG_DFN]    = "dfn",
  [HTML_TAG_ABBR]   = "abbr",
  [HTML_TAG_RUBY]   = "ruby",
  [HTML_TAG_RT]     = "rt",
  [HTML_TAG_RP]     = "rp",
  [HTML_TAG_DATA]   = "data",
  [HTML_TAG_TIME]   = "time",
  [HTML_TAG_CODE]   = "code",
  [HTML_TAG_VAR]    = "var",
  [HTML_TAG_SAMP]   = "samp",
  [HTML_TAG_KBD]    = "kbd",
  [HTML_TAG_SUB]    = "sub",
  [HTML_TAG_SUP]    = "sup",
  [HTML_TAG_I]      = "i",
  [HTML_TAG_B]      = "b",
  [HTML_TAG_U]      = "u",
  [HTML_TAG_MARK]   = "mark",
  [HTML_TAG_BDI]    = "bdi",
  [HTML_TAG_BDO]    = "bdo",
  [HTML_TAG_SPAN]   = "span",
  [HTML_TAG_BR]     = "br",
  [HTML_TAG_WBR]    = "wbr",

  /* 4.7 Edits */
  [HTML_TAG_INS] = "ins",
  [HTML_TAG_DEL] = "del",

  /* 4.8 Embedded content */
  [HTML_TAG_PICTURE] = "picture",
  [HTML_TAG_SOURCE]  = "source",
  [HTML_TAG_IMG]     = "img",
  [HTML_TAG_IFRAME]  = "iframe",
  [HTML_TAG_EMBED]   = "embed",
  [HTML_TAG_OBJECT]  = "object",
  [HTML_TAG_VIDEO]   = "video",
  [HTML_TAG_AUDIO]   = "audio",
  [HTML_TAG_TRACK]   = "track",
  [HTML_TAG_MAP]     = "map",
  [HTML_TAG_AREA]    = "area",

  /* 4.9 Tabular data */
  [HTML_TAG_TABLE]    = "table",
  [HTML_TAG_CAPTION]  = "caption",
  [HTML_TAG_COLGROUP] = "colgroup",
  [HTML_TAG_COL]      = "col",
  [HTML_TAG_TBODY]    = "tbody",
  [HTML_TAG_THEAD]    = "thead",
  [HTML_TAG_TFOOT]    = "tfoot",
  [HTML_TAG_TR]       = "tr",
  [HTML_TAG_TD]       = "td",
  [HTML_TAG_TH]       = "th",

  /* 4.10 Forms */
  [HTML_TAG_FORM]     = "form",
  [HTML_TAG_LABEL]    = "label",
  [HTML_TAG_INPUT]    = "input",
  [HTML_TAG_BUTTON]   = "button",
  [HTML_TAG_SELECT]   = "select",
  [HTML_TAG_DATALIST] = "datalist",
  [HTML_TAG_OPTGROUP] = "optgroup",
  [HTML_TAG_OPTION]   = "option",
  [HTML_TAG_TEXTAREA] = "textarea",
  [HTML_TAG_OUTPUT]   = "output",
  [HTML_TAG_PROGRESS] = "progress",
  [HTML_TAG_METER]    = "meter",
  [HTML_TAG_FIELDSET] = "fieldset",
  [HTML_TAG_LEGEND]   = "legend",

  /* 4.11 Interactive elements */
  [HTML_TAG_DETAILS] = "details",
  [HTML_TAG_SUMMARY] = "summary",
  [HTML_TAG_DIALOG]  = "dialog",

  /* 4.12 Scripting */
  [HTML_TAG_SCRIPT]   = "script",
  [HTML_TAG_NOSCRIPT] = "noscript",
  [HTML_TAG_TEMPLATE] = "template",
  [HTML_TAG_SLOT]     = "slot",
  [HTML_TAG_CANVAS]   = "canvas",

  /** 16 Obsolete features **/
  [HTML_TAG_APPLET]    = "applet",
  [HTML_TAG_ACRONYM]   = "acronym",
  [HTML_TAG_BGSOUND]   = "bgsound",
  [HTML_TAG_DIR]       = "dir",
  [HTML_TAG_FRAME]     = "frame",
  [HTML_TAG_FRAMESET]  = "frameset",
  [HTML_TAG_NOFRAMES]  = "noframes",
  [HTML_TAG_ISINDEX]   = "isindex",
  [HTML_TAG_KEYGEN]    = "keygen",
  [HTML_TAG_LISTING]   = "listing",
  [HTML_TAG_MENUITEM]  = "menuitem",
  [HTML_TAG_NEXTID]    = "nextid",
  [HTML_TAG_NOEMBED]   = "noembed",
  [HTML_TAG_PARAM]     = "param",
  [HTML_TAG_PLAINTEXT] = "plaintext",
  [HTML_TAG_RB]        = "rb",
  [HTML_TAG_RTC]       = "rtc",
  [HTML_TAG_STRIKE]    = "strike",
  [HTML_TAG_XMP]       = "xmp",
  [HTML_TAG_BASEFONT]  = "basefont",
  [HTML_TAG_BIG]       = "big",
  [HTML_TAG_BLINK]     = "blink",
  [HTML_TAG_CENTER]    = "center",
  [HTML_TAG_FONT]      = "font",
  [HTML_TAG_MARQUEE]   = "marquee",
  [HTML_TAG_MULTICOL]  = "multicol",
  [HTML_TAG_NOBR]      = "nobr",
  [HTML_TAG_SPACER]    = "spacer",
  [HTML_TAG_TT]        = "tt",
};

const DOMInterface *k_html_element_interfaces[NUM_HTML_TAG] = {

  /* 4.1 The document element */
  [HTML_TAG_HTML] = DOM_INTERFACE(html_html_element),

  /* 4.2 Document metadata */
  [HTML_TAG_HEAD]  = DOM_INTERFACE(html_head_element),
  [HTML_TAG_TITLE] = DOM_INTERFACE(html_title_element),
  [HTML_TAG_BASE]  = DOM_INTERFACE(html_base_element),
  [HTML_TAG_LINK]  = DOM_INTERFACE(html_link_element),
  [HTML_TAG_META]  = DOM_INTERFACE(html_meta_element),
  [HTML_TAG_STYLE] = DOM_INTERFACE(html_style_element),

  /* 4.3 Sections */
  [HTML_TAG_BODY]    = DOM_INTERFACE(html_body_element),
  [HTML_TAG_ARTICLE] = DOM_INTERFACE(html_element),
  [HTML_TAG_SECTION] = DOM_INTERFACE(html_element),
  [HTML_TAG_NAV]     = DOM_INTERFACE(html_element),
  [HTML_TAG_ASIDE]   = DOM_INTERFACE(html_element),
  [HTML_TAG_H1]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_H2]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_H3]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_H4]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_H5]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_H6]      = DOM_INTERFACE(html_heading_element),
  [HTML_TAG_HGROUP]  = DOM_INTERFACE(html_element),
  [HTML_TAG_HEADER]  = DOM_INTERFACE(html_element),
  [HTML_TAG_FOOTER]  = DOM_INTERFACE(html_element),
  [HTML_TAG_ADDRESS] = DOM_INTERFACE(html_element),

  /* 4.4 Grouping content */
  [HTML_TAG_P]          = DOM_INTERFACE(html_paragraph_element),
  [HTML_TAG_HR]         = DOM_INTERFACE(html_hr_element),
  [HTML_TAG_PRE]        = DOM_INTERFACE(html_pre_element),
  [HTML_TAG_BLOCKQUOTE] = DOM_INTERFACE(html_quote_element),
  [HTML_TAG_OL]         = DOM_INTERFACE(html_olist_element),
  [HTML_TAG_UL]         = DOM_INTERFACE(html_ulist_element),
  [HTML_TAG_MENU]       = DOM_INTERFACE(html_menu_element),
  [HTML_TAG_LI]         = DOM_INTERFACE(html_li_element),
  [HTML_TAG_DL]         = DOM_INTERFACE(html_dlist_element),
  [HTML_TAG_DT]         = DOM_INTERFACE(html_element),
  [HTML_TAG_DD]         = DOM_INTERFACE(html_element),
  [HTML_TAG_FIGURE]     = DOM_INTERFACE(html_element),
  [HTML_TAG_FIGCAPTION] = DOM_INTERFACE(html_element),
  [HTML_TAG_MAIN]       = DOM_INTERFACE(html_element),
  [HTML_TAG_SEARCH]     = DOM_INTERFACE(html_element),
  [HTML_TAG_DIV]        = DOM_INTERFACE(html_div_element),

  /* 4.5 Text-level semantics */
  [HTML_TAG_A]      = DOM_INTERFACE(html_anchor_element),
  [HTML_TAG_EM]     = DOM_INTERFACE(html_element),
  [HTML_TAG_STRONG] = DOM_INTERFACE(html_element),
  [HTML_TAG_SMALL]  = DOM_INTERFACE(html_element),
  [HTML_TAG_S]      = DOM_INTERFACE(html_element),
  [HTML_TAG_CITE]   = DOM_INTERFACE(html_element),
  [HTML_TAG_Q]      = DOM_INTERFACE(html_element),
  [HTML_TAG_DFN]    = DOM_INTERFACE(html_element),
  [HTML_TAG_ABBR]   = DOM_INTERFACE(html_element),
  [HTML_TAG_RUBY]   = DOM_INTERFACE(html_element),
  [HTML_TAG_RT]     = DOM_INTERFACE(html_element),
  [HTML_TAG_RP]     = DOM_INTERFACE(html_element),
  [HTML_TAG_DATA]   = DOM_INTERFACE(html_element),
  [HTML_TAG_TIME]   = DOM_INTERFACE(html_element),
  [HTML_TAG_CODE]   = DOM_INTERFACE(html_element),
  [HTML_TAG_VAR]    = DOM_INTERFACE(html_element),
  [HTML_TAG_SAMP]   = DOM_INTERFACE(html_element),
  [HTML_TAG_KBD]    = DOM_INTERFACE(html_element),
  [HTML_TAG_SUB]    = DOM_INTERFACE(html_element),
  [HTML_TAG_SUP]    = DOM_INTERFACE(html_element),
  [HTML_TAG_I]      = DOM_INTERFACE(html_element),
  [HTML_TAG_B]      = DOM_INTERFACE(html_element),
  [HTML_TAG_U]      = DOM_INTERFACE(html_element),
  [HTML_TAG_MARK]   = DOM_INTERFACE(html_element),
  [HTML_TAG_BDI]    = DOM_INTERFACE(html_element),
  [HTML_TAG_BDO]    = DOM_INTERFACE(html_element),
  [HTML_TAG_SPAN]   = DOM_INTERFACE(html_span_element),
  [HTML_TAG_BR]     = DOM_INTERFACE(html_element),
  [HTML_TAG_WBR]    = DOM_INTERFACE(html_element),

  /* 4.7 Edits */
  [HTML_TAG_INS] = DOM_INTERFACE(html_element),
  [HTML_TAG_DEL] = DOM_INTERFACE(html_element),

  /* 4.8 Embedded content */
  [HTML_TAG_PICTURE] = DOM_INTERFACE(html_element),
  [HTML_TAG_SOURCE]  = DOM_INTERFACE(html_element),
  [HTML_TAG_IMG]     = DOM_INTERFACE(html_element),
  [HTML_TAG_IFRAME]  = DOM_INTERFACE(html_element),
  [HTML_TAG_EMBED]   = DOM_INTERFACE(html_element),
  [HTML_TAG_OBJECT]  = DOM_INTERFACE(html_element),
  [HTML_TAG_VIDEO]   = DOM_INTERFACE(html_element),
  [HTML_TAG_AUDIO]   = DOM_INTERFACE(html_element),
  [HTML_TAG_TRACK]   = DOM_INTERFACE(html_element),
  [HTML_TAG_MAP]     = DOM_INTERFACE(html_element),
  [HTML_TAG_AREA]    = DOM_INTERFACE(html_element),

  /* 4.9 Tabular data */
  [HTML_TAG_TABLE]    = DOM_INTERFACE(html_table_element),
  [HTML_TAG_CAPTION]  = DOM_INTERFACE(html_element),
  [HTML_TAG_COLGROUP] = DOM_INTERFACE(html_element),
  [HTML_TAG_COL]      = DOM_INTERFACE(html_element),
  [HTML_TAG_TBODY]    = DOM_INTERFACE(html_element),
  [HTML_TAG_THEAD]    = DOM_INTERFACE(html_element),
  [HTML_TAG_TFOOT]    = DOM_INTERFACE(html_element),
  [HTML_TAG_TR]       = DOM_INTERFACE(html_element),
  [HTML_TAG_TD]       = DOM_INTERFACE(html_element),
  [HTML_TAG_TH]       = DOM_INTERFACE(html_element),

  /* 4.10 Forms */
  [HTML_TAG_FORM]     = DOM_INTERFACE(html_form_element),
  [HTML_TAG_LABEL]    = DOM_INTERFACE(html_element),
  [HTML_TAG_INPUT]    = DOM_INTERFACE(html_element),
  [HTML_TAG_BUTTON]   = DOM_INTERFACE(html_element),
  [HTML_TAG_SELECT]   = DOM_INTERFACE(html_element),
  [HTML_TAG_DATALIST] = DOM_INTERFACE(html_element),
  [HTML_TAG_OPTGROUP] = DOM_INTERFACE(html_element),
  [HTML_TAG_OPTION]   = DOM_INTERFACE(html_element),
  [HTML_TAG_TEXTAREA] = DOM_INTERFACE(html_element),
  [HTML_TAG_OUTPUT]   = DOM_INTERFACE(html_element),
  [HTML_TAG_PROGRESS] = DOM_INTERFACE(html_element),
  [HTML_TAG_METER]    = DOM_INTERFACE(html_element),
  [HTML_TAG_FIELDSET] = DOM_INTERFACE(html_element),
  [HTML_TAG_LEGEND]   = DOM_INTERFACE(html_element),

  /* 4.11 Interactive elements */
  [HTML_TAG_DETAILS] = DOM_INTERFACE(html_element),
  [HTML_TAG_SUMMARY] = DOM_INTERFACE(html_element),
  [HTML_TAG_DIALOG]  = DOM_INTERFACE(html_element),

  /* 4.12 Scripting */
  [HTML_TAG_SCRIPT]   = DOM_INTERFACE(html_script_element),
  [HTML_TAG_NOSCRIPT] = DOM_INTERFACE(html_element),
  [HTML_TAG_TEMPLATE] = DOM_INTERFACE(html_template_element),
  [HTML_TAG_SLOT]     = DOM_INTERFACE(html_element),
  [HTML_TAG_CANVAS]   = DOM_INTERFACE(html_element),

  /** 16 Obsolete features **/
  [HTML_TAG_APPLET]    = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_ACRONYM]   = DOM_INTERFACE(html_element),
  [HTML_TAG_BGSOUND]   = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_DIR]       = DOM_INTERFACE(html_element),
  [HTML_TAG_FRAME]     = DOM_INTERFACE(html_element),
  [HTML_TAG_FRAMESET]  = DOM_INTERFACE(html_element),
  [HTML_TAG_NOFRAMES]  = DOM_INTERFACE(html_element),
  [HTML_TAG_ISINDEX]   = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_KEYGEN]    = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_LISTING]   = DOM_INTERFACE(html_pre_element),
  [HTML_TAG_MENUITEM]  = DOM_INTERFACE(html_element),
  [HTML_TAG_NEXTID]    = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_NOEMBED]   = DOM_INTERFACE(html_element),
  [HTML_TAG_PARAM]     = DOM_INTERFACE(html_element),
  [HTML_TAG_PLAINTEXT] = DOM_INTERFACE(html_element),
  [HTML_TAG_RB]        = DOM_INTERFACE(html_element),
  [HTML_TAG_RTC]       = DOM_INTERFACE(html_element),
  [HTML_TAG_STRIKE]    = DOM_INTERFACE(html_element),
  [HTML_TAG_XMP]       = DOM_INTERFACE(html_pre_element),
  [HTML_TAG_BASEFONT]  = DOM_INTERFACE(html_element),
  [HTML_TAG_BIG]       = DOM_INTERFACE(html_element),
  [HTML_TAG_BLINK]     = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_CENTER]    = DOM_INTERFACE(html_element),
  [HTML_TAG_FONT]      = DOM_INTERFACE(html_element),
  [HTML_TAG_MARQUEE]   = DOM_INTERFACE(html_element),
  [HTML_TAG_MULTICOL]  = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_NOBR]      = DOM_INTERFACE(html_element),
  [HTML_TAG_SPACER]    = DOM_INTERFACE(html_unknown_element),
  [HTML_TAG_TT]        = DOM_INTERFACE(html_element),
};

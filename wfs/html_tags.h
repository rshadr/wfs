#ifndef _LIBWFS_HTML_TAGS_H
#define _LIBWFS_HTML_TAGS_H

#include <wfs/dom.h>

enum HTMLTag : uint16_t {
  _HTML_TAG_NONE = 0,
  _HTML_TAG_FIRST,

  /* 4.1 The document element */
  HTML_TAG_HTML = _HTML_TAG_FIRST,

  /* 4.2 Document metadata */
  HTML_TAG_HEAD,
  HTML_TAG_TITLE,
  HTML_TAG_BASE,
  HTML_TAG_LINK,
  HTML_TAG_META,
  HTML_TAG_STYLE,

  /* 4.3 Sections */
  HTML_TAG_BODY,
  HTML_TAG_ARTICLE,
  HTML_TAG_SECTION,
  HTML_TAG_NAV,
  HTML_TAG_ASIDE,
  HTML_TAG_H1,
  HTML_TAG_H2,
  HTML_TAG_H3,
  HTML_TAG_H4,
  HTML_TAG_H5,
  HTML_TAG_H6,
  HTML_TAG_HGROUP,
  HTML_TAG_HEADER,
  HTML_TAG_FOOTER,
  HTML_TAG_ADDRESS,

  /* 4.4 Grouping content */
  HTML_TAG_P,
  HTML_TAG_HR,
  HTML_TAG_PRE,
  HTML_TAG_BLOCKQUOTE,
  HTML_TAG_OL,
  HTML_TAG_UL,
  HTML_TAG_MENU,
  HTML_TAG_LI,
  HTML_TAG_DL,
  HTML_TAG_DT,
  HTML_TAG_DD,
  HTML_TAG_FIGURE,
  HTML_TAG_FIGCAPTION,
  HTML_TAG_MAIN,
  HTML_TAG_SEARCH,
  HTML_TAG_DIV,

  /* 4.5 Text-level semantics */
  HTML_TAG_A,
  HTML_TAG_EM,
  HTML_TAG_STRONG,
  HTML_TAG_SMALL,
  HTML_TAG_S,
  HTML_TAG_CITE,
  HTML_TAG_Q,
  HTML_TAG_DFN,
  HTML_TAG_ABBR,
  HTML_TAG_RUBY,
  HTML_TAG_RT,
  HTML_TAG_RP,
  HTML_TAG_DATA,
  HTML_TAG_TIME,
  HTML_TAG_CODE,
  HTML_TAG_VAR,
  HTML_TAG_SAMP,
  HTML_TAG_KBD,
  HTML_TAG_SUB,
  HTML_TAG_SUP,
  HTML_TAG_I,
  HTML_TAG_B,
  HTML_TAG_U,
  HTML_TAG_MARK,
  HTML_TAG_BDI,
  HTML_TAG_BDO,
  HTML_TAG_SPAN,
  HTML_TAG_BR,
  HTML_TAG_WBR,

  /* 4.7 Edits */
  HTML_TAG_INS,
  HTML_TAG_DEL,

  /* 4.8 Embedded content */
  HTML_TAG_PICTURE,
  HTML_TAG_SOURCE,
  HTML_TAG_IMG,
  HTML_TAG_IFRAME,
  HTML_TAG_EMBED,
  HTML_TAG_OBJECT,
  HTML_TAG_VIDEO,
  HTML_TAG_AUDIO,
  HTML_TAG_TRACK,
  HTML_TAG_MAP,
  HTML_TAG_AREA,

  /* 4.9 Tabular data */
  HTML_TAG_TABLE,
  HTML_TAG_CAPTION,
  HTML_TAG_COLGROUP,
  HTML_TAG_COL,
  HTML_TAG_TBODY,
  HTML_TAG_THEAD,
  HTML_TAG_TFOOT,
  HTML_TAG_TR,
  HTML_TAG_TD,
  HTML_TAG_TH,

  /* 4.10 Forms */
  HTML_TAG_FORM,
  HTML_TAG_LABEL,
  HTML_TAG_INPUT,
  HTML_TAG_BUTTON,
  HTML_TAG_SELECT,
  HTML_TAG_DATALIST,
  HTML_TAG_OPTGROUP,
  HTML_TAG_OPTION,
  HTML_TAG_TEXTAREA,
  HTML_TAG_OUTPUT,
  HTML_TAG_PROGRESS,
  HTML_TAG_METER,
  HTML_TAG_FIELDSET,
  HTML_TAG_LEGEND,

  /* 4.11 Interactive elements */
  HTML_TAG_DETAILS,
  HTML_TAG_SUMMARY,
  HTML_TAG_DIALOG,

  /* 4.12 Scripting */
  HTML_TAG_SCRIPT,
  HTML_TAG_NOSCRIPT,
  HTML_TAG_TEMPLATE,
  HTML_TAG_SLOT,
  HTML_TAG_CANVAS,

  /* 16 Obsolete features */
  HTML_TAG_APPLET,
  HTML_TAG_ACRONYM,
  HTML_TAG_BGSOUND,
  HTML_TAG_DIR,
  HTML_TAG_FRAME,
  HTML_TAG_FRAMESET,
  HTML_TAG_NOFRAMES,
  HTML_TAG_ISINDEX,
  HTML_TAG_KEYGEN,
  HTML_TAG_LISTING,
  HTML_TAG_MENUITEM,
  HTML_TAG_NEXTID,
  HTML_TAG_NOEMBED,
  HTML_TAG_PARAM,
  HTML_TAG_PLAINTEXT,
  HTML_TAG_RB,
  HTML_TAG_RTC,
  HTML_TAG_STRIKE,
  HTML_TAG_XMP,
  HTML_TAG_BASEFONT,
  HTML_TAG_BIG,
  HTML_TAG_BLINK,
  HTML_TAG_CENTER,
  HTML_TAG_FONT,
  HTML_TAG_MARQUEE,
  HTML_TAG_MULTICOL,
  HTML_TAG_NOBR,
  HTML_TAG_SPACER,
  HTML_TAG_TT,

  NUM_HTML_TAG
};

extern const char *k_html_tag_names[NUM_HTML_TAG];
extern const DOMInterface *k_html_element_interfaces[NUM_HTML_TAG];

#endif /* _LIBWFS_HTML_TAGS_H */
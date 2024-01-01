#ifndef _LIBWFS_HTML_H
#define _LIBWFS_HTML_H

#include <stddef.h>
#include <wfs/dom_core.h>
#include <wfs/dom_html.h>

#include <wfs/html_tags.h>

typedef struct HTMLCustomElemDef_s {
  InfraString *name;
  void *ctor;

  bool form_associated;
  bool disable_internals;
  bool disable_shadow;

} HTMLCustomElemDef;

void html_parse(struct dom_document *document, const char *input, size_t input_len);

#endif /* _LIBWFS_HTML_H */

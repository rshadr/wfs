#ifndef _LIBWFS_HTML_PARSE_H
#define _LIBWFS_HTML_PARSE_H

#include <stddef.h>
#include <wfs/dom_core.h>

void html_parse(struct dom_document *document, const char *input, size_t input_len);

#endif /* _LIBWFS_HTML_PARSE_H */

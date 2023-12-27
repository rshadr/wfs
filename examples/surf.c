#include <stdio.h>

#include <wfs/dom.h>
#include <wfs/dom_core.h>

int
main(int argc, char *argv[])
{
  struct dom_document *doc = dom_strong_ref_object(dom_alloc_object( DOM_INTERFACE(document) ));

  dom_strong_unref_object(doc);

  return 0;
}

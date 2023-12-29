#include <stdio.h>

#include <wfs/dom.h>
#include <wfs/dom_core.h>
#include <wfs/html_parse.h>

const char *myfile = "<!DOCTYPE html>\n<html lang='fr' yeet scott=free>TTTTTTTTTTT</html>";

int
main(int argc, char *argv[])
{
  struct dom_document *doc = dom_strong_ref_object(dom_alloc_object( DOM_INTERFACE(document) ));
  ((struct dom_node *) doc)->children = infra_stack_create();

  html_parse(doc, myfile, strlen(myfile));

  dom_strong_unref_object(doc);

  return 0;
}

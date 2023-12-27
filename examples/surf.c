#include <stdio.h>

#include <wfs/dom.h>
#include <wfs/dom_core.h>
#include <wfs/html_parse.h>

const char *myfile = "<!DOCTYPE html>\n<html>";

int
main(int argc, char *argv[])
{
  struct dom_document *doc = dom_strong_ref_object(dom_alloc_object( DOM_INTERFACE(document) ));

  html_parse(doc, myfile, strlen(myfile));

  dom_strong_unref_object(doc);

  return 0;
}

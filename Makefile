.POSIX:
.SUFFIXES:

include config.mk

CFLAGS += -I.

WFS_HEADERS =\
	wfs/dom.h\
	wfs/dom_core.h\
	wfs/infra_stack.h\
	wfs/infra_string.h\

all: examples/surf libwfs.a

SRCS =\
	src/dom_core\
	src/dom_html\
	src/html_parse\
	src/html_tags\
	src/infra_stack\
	src/infra_string\

src/dom_core.o: src/dom_core.c wfs/dom_core.h wfs/dom.h
src/dom_html.o: src/dom_html.c wfs/dom_html.h wfs/dom_core.h wfs/dom.h
src/html_parse.o: src/html_parse.c src/html_tokenizer_states.c \
	src/html_treebuilder_modes.c wfs/dom_core.h wfs/dom.h \
	src/unicode.h wfs/infra_string.h wfs/infra_stack.h
src/html_tags.o: src/html_tags.c wfs/html_tags.h wfs/dom.h
src/infra_stack.o: src/infra_stack.c wfs/infra_stack.h
src/infra_string.o: src/infra_string.c wfs/infra_string.h

$(SRCS:=.o): Makefile config.mk

libwfs.a: $(SRCS:=.o)
	$(AR) -r $@ $^
	$(RANLIB) $@

$(SRCS:=.o):
	$(CC) -c -o $@ $(CFLAGS) $(@:.o=.c)

examples/surf: examples/surf.c libwfs.a $(WFS_HEADERS) config.mk
	$(CC) -o $@ $(CFLAGS) $(@:=.c) libwfs.a $(LIBS)

clean:
	rm -rf libwfs.a $(SRCS:=.o) examples/surf

.PHONY: clean

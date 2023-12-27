.POSIX:
.SUFFIXES:

include config.mk

CFLAGS += -I.

WFS_HEADERS =\
	wfs/dom.h\

all: examples/surf libwfs.a

SRCS =\
	src/dom_core\
	src/infra_string\

src/dom_core.o: src/dom_core.c wfs/dom_core.h wfs/dom.h
src/infra_string.o: src/infra_string.c wfs/infra_string.h

libwfs.a: $(SRCS:=.o)
	$(AR) -r $@ $^
	$(RANLIB) $@

$(SRCS:=.o):
	$(CC) -c -o $@ $(CFLAGS) $(@:.o=.c)

examples/surf: examples/surf.c libwfs.a $(WFS_HEADERS)
	$(CC) -o $@ $(CFLAGS) $(@:=.c) libwfs.a $(LIBS)

clean:
	rm -rf libwfs.a $(SRCS:=.o) examples/surf

.PHONY: clean

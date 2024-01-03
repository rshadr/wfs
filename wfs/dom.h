#ifndef _LIBWFS_DOM_H
#define _LIBWFS_DOM_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

typedef struct DOMInterface_s DOMInterface;

typedef struct DOMHeader_s {
  const DOMInterface *interface;
  int_least32_t strong_refcnt;
  int_least32_t weak_refcnt;
} DOMHeader;

typedef struct DOMObject_s {
  DOMHeader header;
  /* object-specific data follows here (opaque) */
} DOMObject;

typedef struct DOMInterface_s {
  const DOMInterface * parent_interface;
  const char *         name;
  size_t               impl_size;

  void (*finalizer) (DOMObject *obj);
} DOMInterface;

#define DOM_DECLARE_INTERFACE(name) extern const DOMInterface k_dom_ ## name ## _interface
#define DOM_DEFINE_INTERFACE(name) const DOMInterface k_dom_ ## name ## _interface =
#define DOM_INTERFACE(name) (& k_dom_ ## name ## _interface)

typedef void DOMAny;

static inline const DOMInterface *
dom_get_interface(const DOMAny *obj)
{
  return ((const DOMObject *) obj)->header.interface;
}

static inline int
dom_implements_interface(const DOMAny *obj, const DOMInterface *interface)
{
  for (const DOMInterface *i = dom_get_interface(obj);
       i != NULL; i = i->parent_interface)
    if (i == interface)
      return 1;

  return 0;
}


static inline DOMAny *
dom_alloc_object(const DOMInterface *interface)
{
  /* NO INITIAL ARC REFERENCE! */
  DOMObject *obj;

  obj = malloc(interface->impl_size);
  memset(obj, 0, interface->impl_size);

  obj->header.interface = interface;

  return obj;
}

static inline void
dom_free_object(DOMObject *obj)
{
  for (const DOMInterface *i = dom_get_interface(obj);
       i != NULL; i = i->parent_interface)
    if (i->finalizer != NULL) { i->finalizer(obj); }

  free(obj);
}

static inline DOMAny *
dom_strong_ref_object(DOMAny *obj)
{
  if (obj != NULL) {
    ((DOMObject *) obj)->header.strong_refcnt++;
    printf("[DOM Debug]: Referenced %s now has %"PRIdLEAST32" strong references\n",
     dom_get_interface(obj)->name, ((DOMObject *) obj)->header.strong_refcnt);
  }

  return obj;
}

static inline void
dom_strong_unref_object(DOMAny *obj)
{
  DOMObject *o = obj;

  if (o != NULL)
    printf("[DOM Debug]: Dereferenced %s now had %"PRIdLEAST32" strong references\n",
     dom_get_interface(obj)->name, ((DOMObject *) obj)->header.strong_refcnt);

  if (o != NULL && --o->header.strong_refcnt <= 0)
    dom_free_object(o);
}

static inline DOMAny *
dom_weak_ref_object(DOMAny *obj)
{
  if (obj != NULL)
    ((DOMObject *) obj)->header.weak_refcnt++;

  return obj;
}

static inline void
dom_weak_unref_object(DOMAny *obj)
{
  if (obj != NULL)
    ((DOMObject *) obj)->header.weak_refcnt--;
}

/* Only works for interfaces known at compile-type */
#define DOM_NEW_OBJECT(name) (dom_alloc_object(DOM_INTERFACE(name)))
#define DOM_IMPLEMENTS(obj, name) (dom_implements_interface((obj), DOM_INTERFACE(name)))

#endif /* _LIBWFS_DOM_H */

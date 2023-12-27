#ifndef _LIBWFS_DOM_H
#define _LIBWFS_DOM_H

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

typedef struct DOMInterface_s DOMInterface;

typedef struct DOMHeader_s {
  const DOMInterface *interface;
  _Atomic(int_least32_t) strong_refcnt;
  _Atomic(int_least32_t) weak_refcnt;
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
  if (obj != NULL)
    ((DOMObject *) obj)->header.strong_refcnt++;

  return obj;
}

static inline void
dom_strong_unref_object(DOMAny *obj)
{
  DOMObject *o = obj;

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

#endif /* _LIBWFS_DOM_H */

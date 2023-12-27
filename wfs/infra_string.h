#ifndef _LIBWFS_INFRA_STRING_H
#define _LIBWFS_INFRA_STRING_H

#include <stdint.h>
#include <string.h>

typedef struct InfraString_s {
  char *data;
  _Atomic(int_least32_t) refcnt;
  uint32_t size;
  uint32_t cap;
} InfraString;

/* Has an initial reference! */
InfraString *infra_string_create(void);
void infra_string_free(InfraString *string);

static inline InfraString *
infra_string_ref(InfraString *string)
{
  if (string != NULL)
    string->refcnt++;

  return string;
}

static inline void
infra_string_unref(InfraString *string)
{
  if (string != NULL && --string->refcnt <= 0)
    infra_string_free(string);
}

static inline void
infra_string_zero(InfraString *string)
{
  if (string != NULL && string->size != 0)
    memset(string->data, 0, string->cap);
}

#endif /* _LIBWFS_INFRA_STRING_H */

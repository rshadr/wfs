#include <stdlib.h>
#include <wfs/infra_string.h>

static const uint32_t k_string_grow_step = 4;

InfraString *
infra_string_create(void)
{
  InfraString *string = malloc(sizeof (*string));
  memset(string, 0, sizeof (*string));

  uint32_t init_cap = k_string_grow_step;
  char *data = malloc(init_cap);
  memset(data, 0, init_cap);

  string->data = data;
  string->cap  = init_cap;

  return infra_string_ref(string);
}

void
infra_string_free(InfraString *string)
{
  free(string->data);
  free(string);
}

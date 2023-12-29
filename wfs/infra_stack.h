#ifndef _LIBWFS_INFRA_STACK_H
#define _LIBWFS_INFRA_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct InfraStack_s {
  void **  items;
  uint32_t  size;
  uint32_t  cap;
} InfraStack;

InfraStack *infra_stack_create(void);
void infra_stack_free(InfraStack *stack);
void *infra_stack_push(InfraStack *stack, void *item);
void *infra_stack_pop(InfraStack *stack);

static inline void *
infra_stack_peek(InfraStack *stack)
{
  if (stack == NULL || stack->size == 0)
    return NULL;

  return stack->items[stack->size - 1];
}

#define INFRA_STACK_FOREACH(stack, index) for (int32_t index = 0; index < (stack)->size; index++)

#endif /* _LIBWFS_INFRA_STACK_H */

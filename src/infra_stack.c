#include <stdlib.h>
#include <string.h>

#include <wfs/infra_stack.h>

static const uint32_t k_stack_grow_step = 4;

InfraStack *
infra_stack_create(void)
{
  InfraStack *stack = malloc(sizeof (*stack));
  memset(stack, 0, sizeof (*stack));

  uint32_t init_cap = k_stack_grow_step;
  stack->cap = init_cap;

  void **items = malloc(init_cap * sizeof (void *));
  memset(items, 0, sizeof (*items) * init_cap);
  stack->items = items;

  return stack;
}

void
infra_stack_free(InfraStack *stack)
{
  free(stack->items);
  free(stack);
}

static void
maybe_grow(InfraStack *stack, size_t need)
{
  uint32_t new_size = stack->size + need;

  if (new_size >= stack->cap) {
    uint32_t new_cap = stack->cap + k_stack_grow_step;
    void **new_items = malloc(new_cap * sizeof (void *));
    memset(new_items, 0, new_cap * sizeof (void *));
    stack->cap = new_cap;

    memcpy(new_items, stack->items, stack->size);
    free(stack->items);
    stack->items = new_items;
  }
}

void *
infra_stack_push(InfraStack *stack, void *item)
{
  maybe_grow(stack, 1);
  stack->items[stack->size++] = item;
  return item;
}

void *
infra_stack_pop(InfraStack *stack)
{
  if (stack == NULL || stack->size == 0)
    return NULL;

  return stack->items[--stack->size];
}

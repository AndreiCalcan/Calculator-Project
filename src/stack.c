#include "stack.h"

Stack *init_stack(uint8_t item_size)
{
    Stack *new_stack = malloc(sizeof(Stack));
    new_stack->item_size = item_size;
    new_stack->max_size = 200 / item_size;
    new_stack->size = 0;
    return new_stack;
}

int stack_push(Stack *stack, void *item)
{
    if (stack->size + 1 >= stack->max_size)
    {
        return -1;
    }
    int offset = stack->size * stack->item_size;
    memcpy(stack->data + offset, item, stack->item_size);
    stack->size = stack->size + 1;
    return 0;
}

int stack_pop(Stack *stack, void *item)
{
    if (stack->size <= 0)
        return -1;
    int offset = (stack->size - 1) * stack->item_size;
    memcpy(item, stack->data + offset, stack->item_size);
    stack->size = stack->size - 1;
    return 0;
}

void stack_top(Stack *stack, void *dest)
{
    int offset = (stack->size - 1) * stack->item_size;
    memcpy(dest, stack->data + offset, stack->item_size);
}
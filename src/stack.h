#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef struct stack
{
    uint8_t item_size;
    uint8_t size;
    uint8_t max_size;
    uint8_t data[200];
} Stack;

Stack *init_stack(uint8_t item_size);
int stack_push(Stack *stack, void *item);
int stack_pop(Stack *stack, void *item);
void stack_top(Stack *stack, void *dest);
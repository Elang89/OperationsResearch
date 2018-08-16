#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


Stack* stack_new(int stack_size)
{
  int i;
  Stack* stack = malloc(sizeof(Stack));
  stack->data = malloc(sizeof(int) * stack_size);
  stack->element_count = -1;
  stack->size = stack_size;
  return stack;

  for(i = 0; i < stack_size; i++)
  {
    stack->data[i] = 0;
  }
}

int stack_is_empty(Stack* stack)
{
  if(stack->element_count == -1)
  {
    return 1;
  }
  return 0;
}
int stack_is_full(Stack* stack)
{
  if(stack->element_count == stack->size)
  {
    return 1;
  }
  return 0;
}

int stack_peek(Stack* stack)
{
  return stack->data[stack->element_count];
}

void stack_pop(Stack* stack)
{
  if(stack_is_empty(stack) != 1)
  {
   stack->data[stack->element_count] = 0;
   stack->element_count--;
  }
  else
  {
    printf("Stack is Empty.\n");
  }
}

void stack_push(Stack* stack, int element)
{
  if(stack_is_full(stack) != 1)
  {
    stack->element_count++;
    stack->data[stack->element_count] = element;
  }
  else
  {
    printf("Stack is full.\n");
  }
}

void stack_destroy(Stack *stack)
{
  free(stack->data);
  free(stack);
}

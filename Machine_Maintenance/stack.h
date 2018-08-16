#ifndef STACK_H
#define STACK_H


typedef struct Stack{
  int * data;
  int element_count;
  int size;
}Stack;

Stack* stack_new(int stack_size);
int stack_is_empty(Stack* stack);
int stack_is_full(Stack* stack);
int stack_peek(Stack* stack);
void stack_pop(Stack* stack);
void stack_push(Stack* stack, int element);
void stack_destroy(Stack* stack);

#endif

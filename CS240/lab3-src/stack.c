
#include <stdio.h>
#include "stack.h"
#include <stdlib.h>

int top=0;
double stack[MAXSTACK];

void stack_clear() 
{
  top = 0;
}

double stack_pop()
{
	if (top == 0) {
		printf("Stack underflow\n");
		exit(1);
	} else {
		return stack[--top];
	}
}

void stack_push(double val)
{
	stack[top++] = 	val;
}

void stack_print()
{
	printf("Stack:\n");
	if (top == 0) printf("Stack is empty");
		int i;
		for (i = 0; i < top; i++) {
			printf("%d: %f\n",i,stack[i]);
		} 
}

int stack_top()
{
  return top;
}

int stack_max()
{
  return MAXSTACK;
}

int stack_is_empty()
{
  return top == 0;
}



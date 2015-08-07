// Manmeet Singh - msingh11@ucsc.edu
// $Id: stack.h,v 1.6 2014-01-24 18:33:47-08 - - $

#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include "bigint.h"

typedef struct stack stack;
typedef bigint *stack_item;

//
// Create a new empty stack.
//
stack *new_stack (void);

//
// Free up the stack.
// Precondition: stack must be empty.
//
void free_stack (stack*);

//
// Push a new stack_item onto the top of the stack.
//
void push_stack (stack *, stack_item);

//
// Pop the top stack_item from the stack and return it.
//
stack_item pop_stack (stack*);

//
// Peek into the stack and return a selected stack_item.
// Item 0 is the element at the top.
// Item size_stack - 1 is the element at the bottom.
// Precondition: 0 <= index && index < size_stack.
//
stack_item peek_stack (stack *, size_t index);

//
// Indicate whether the stack is empty or not.
// Same as size_stack == 0.
//
bool empty_stack (stack*);

//
// Return the current size of the stack (number of items on the stack).
//
size_t size_stack (stack*);

//
// Print part of the stack in debug format.
//
void show_stack (stack*);

#endif

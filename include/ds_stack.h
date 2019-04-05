#ifndef __DS_STACK__
#define __DS_STACK__

#include <stdlib.h>
#include "simple_logger.h"

/**
 * @brief A stack: first in, last out
 */
typedef struct stacknode_s
{
	void * data;				/**<The data held by the node*/
	struct stacknode_s * next;	/**<Pointer to next node in the stack*/
	size_t elementSize;			/**<The size of the data to hold*/
}Stack;

/**
 * @brief Inits a stack / stack node
 * @param elementSize The size of the element to hold
 * @returns Pointer to new Stack if successful; NULL if could not allocate memory
 */
Stack * stack_init(size_t elementSize);

/**
 * @brief Pushes new data onto a stack
 * @param stack The stack to push data onto
 * @param data The data to push onto the stack
 * @param elementSize The size of the data
 */
void stack_push(Stack ** stack, void * data, size_t elementSize);

/**
 * @brief Removes the topmost element from the stack
 * @param stack The stack to pop from
 * @returns The data at the top of the stack if successful; NULL if stack was NULL
 */
void * stack_pop(Stack ** stack);

/**
 * @brief Gets the topmost element from the stack without removing it
 * @param stack The stack to peek from
 * @returns The data at the top of the stack if successful; NULL if stack was NULL
 */
void * stack_peek(Stack * stack);

#endif // !__DS_STACK__

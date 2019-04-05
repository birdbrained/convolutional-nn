#include "ds_stack.h"

/**
 * @brief Frees a stack node from memory, called by stack_pop
 * @param self The node to free
 */
void stack_free_node(Stack * self)
{
	if (!self)
	{
		slog("Error: cannot free a null stack node");
		return;
	}
	if (self->data != NULL)
	{
		slog("Error: cannot free a stack node that still has data in it");
		return;
	}
	free(self);
}

Stack * stack_init(size_t elementSize)
{
	Stack * s = NULL;
	s = malloc(sizeof(Stack) + elementSize);
	if (!s)
	{
		slog("Error: could not allocate memory for a new stack");
		return NULL;
	}
	memset(s, 0, sizeof(Stack) + elementSize);
	s->elementSize = elementSize;
	return s;
}

void stack_push(Stack ** stack, void * data, size_t elementSize)
{
	Stack * s = NULL;
	if (!stack || !(*stack))
	{
		slog("Error: cannot push onto a null stack");
		return;
	}
	s = stack_init(elementSize);
	s->data = data;
	s->next = (*stack);
	(*stack) = s;
}

void * stack_pop(Stack ** stack)
{
	void * data = NULL;
	Stack * temp = NULL;
	if (!stack || !(*stack))
	{
		slog("Error: cannot pop data from a null stack");
		return NULL;
	}
	data = (*stack)->data;
	temp = (*stack);
	(*stack) = (*stack)->next;
	memset(temp, 0, sizeof(Stack) + temp->elementSize);
	stack_free_node(temp);
	return data;
}

void * stack_peek(Stack * stack)
{
	void * data = NULL;
	if (!stack)
	{
		slog("Warning: stack is null");
		return NULL;
	}
	data = stack->data;
	return data;
}

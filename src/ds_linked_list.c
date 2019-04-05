#include "ds_linked_list.h"
#include <SDL.h>
#include <stdlib.h>

IntNode * IntNode_init(int i)
{
	IntNode * head = NULL;
	head = malloc(sizeof(IntNode));
	head->data = i;
	head->next = NULL;
	return head;
}

void IntNode_Add(IntNode * ll, int i)
{
	int index = 0;
	IntNode *newNode;
	newNode = malloc(sizeof(IntNode));
	newNode->data = i;
	newNode->next = NULL;

	while (ll[index].next != NULL)
	{
		index++;
	}
	ll[index].next = newNode;
}

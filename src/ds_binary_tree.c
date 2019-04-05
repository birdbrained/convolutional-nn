#include "ds_binary_tree.h"

int bt_free_node(BinaryTreeNode * node)
{
	if (!node)
	{
		slog("Error: trying to free a bi tree node that is already NULL");
		return -2;
	}
	if (node->data != NULL)
	{
		slog("Error: trying to free a bi tree node that still has data in it");
		return -1;
	}
	free(node);
	return 0;
}

BinaryTreeNode * bt_new(size_t elementSize)
{
	BinaryTreeNode * node;
	node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode) + elementSize);
	if (node == NULL)
	{
		slog("Error: could not allocate memory for a new BinaryTreeNode");
		return NULL;
	}
	memset(node, 0, sizeof(BinaryTreeNode) + elementSize);
	node->elementSize = elementSize;
	return node;
}

void * bt_delete(BinaryTreeNode ** node)
{
	return NULL;
}

int bt_insert(BinaryTreeNode ** root, void * data, size_t elementSize)
{
	BinaryTreeNode * new_node = bt_new(elementSize);
	BinaryTreeNode * iterator = (*root);
	if (!new_node)
	{
		return -1;
	}
	new_node->data = data;

	if ((*root) == NULL)
	{
		(*root) == new_node;
	}
	else
	{
		while (iterator != NULL)
		{
			if ((int)data < (int)iterator->data)
			{
				if (iterator->left_child != NULL)
				{
					iterator = iterator->left_child;
				}
				else
				{
					iterator->left_child = new_node;
					new_node->parent = iterator;
					break;
				}
			}
			else
			{
				if (iterator->right_child != NULL)
				{
					iterator = iterator->right_child;
				}
				else
				{
					iterator->right_child = new_node;
					new_node->parent = iterator;
					break;
				}
			}
		}
	}

	return 0;
}

int bt_insert_from_node(BinaryTreeNode ** node, void * data, size_t elementSize, int direction)
{
	BinaryTreeNode * child = bt_new(elementSize);

	if (!node || !(*node))
	{
		slog("Error: cannot insert a child at a null node");
		return -1;
	}
	if (!child)
	{
		//slog in bt_new
		return -2;
	}

	child->depth = (*node)->depth + 1;
	child->data = data;
	child->parent = (*node);

	if (direction < 0) //insert left
	{
		if ((*node)->left_child != NULL)
		{
			(*node)->left_child = child;
		}
		else
		{
			slog("Error: left child already exists for node");
			memset(child, 0, sizeof(BinaryTreeNode) + elementSize);
			bt_free_node(child);
			return -3;
		}
	}
	else //insert right
	{
		if ((*node)->right_child != NULL)
		{
			(*node)->right_child = child;
		}
		else
		{
			slog("Error: right child already exists for node");
			memset(child, 0, sizeof(BinaryTreeNode) + elementSize);
			bt_free_node(child);
			return -3;
		}
	}

	return 0;
}

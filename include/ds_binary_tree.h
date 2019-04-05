#include <stdlib.h>
#include "simple_logger.h"

typedef struct btreenode_s
{
	void * data;
	struct btreenode_s * left_child;
	struct btreenode_s * right_child;
	struct btreenode_s * parent;
	size_t elementSize;
	int depth;
}BinaryTreeNode;

/**
 * @brief Creates a new binary tree node
 * @param elementSize The size of the data the node will hold
 * @returns A pointer to new node on success; NULL if could not allocate memory
 */
BinaryTreeNode * bt_new(size_t elementSize);

/**
 * @brief Currently unimplemented
 */
void * bt_delete(BinaryTreeNode ** node);

/**
 * @brief Inserts a new node into a binary tree
 * @param root The root node of the binary tree
 * @param data The data to insert into a node
 * @param elementSize The sizo of the data to hold
 * @returns 0 if successful; -1 if error
 */
int bt_insert(BinaryTreeNode ** root, void * data, size_t elementSize);

/**
 * @brief Manually insert a child node at a given node; fails if a child node already exists
 * @param node The node to insert a child at
 * @param data The data to insert
 * @param elementSize The size of the data to insert
 * @param direction Negative for left, zero or positive for right
 * @returns
 *	 0 if successful;
 *	-1 if node was NULL;
 *	-2 if new child node could not be created;
 *	-3 if node at specified direction already exists
 */
int bt_insert_from_node(BinaryTreeNode ** node, void * data, size_t elementSize, int direction);

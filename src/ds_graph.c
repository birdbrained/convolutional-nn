#include "ds_graph.h"

/**
 * @brief Called by pq_delete, frees memory associated with a dataless node
 * @param thingThatDies The node to free
 * @param elementSize The size of the data the node holds
 * @returns -2 if node is NULL, -1 if data still in node, 0 if successful
 */
int graph_free_node(GraphNode * thingThatDies, size_t elementSize)
{
	if (!thingThatDies)
	{
		slog("Error: trying to free graph node that is null");
		return -2;
	}
	if (thingThatDies->data != NULL)
	{
		slog("Error: trying to free graph node that still has data");
		return -1;
	}
	memset(thingThatDies, 0, sizeof(GraphNode) + elementSize);
	free(thingThatDies);
	return 0;
}

GraphNode * graph_new(size_t elementSize)
{
	GraphNode * node;
	node = (GraphNode *)malloc(sizeof(GraphNode) + elementSize);
	if (node == NULL)
	{
		slog("Error: could not allocate memory for a new graph node");
		return NULL;
	}
	memset(node, 0, sizeof(GraphNode) + elementSize);
	node->elementSize = elementSize;
	return node;
}

Graph * graph_init(int width, size_t elementSize)
{
	int i = 0;
	Graph * grape;
	grape = (Graph *)malloc(sizeof(Graph));
	if (grape == NULL)
	{
		slog("Error: could not allocate memory for a new graph");
		return NULL;
	}
	memset(grape, 0, sizeof(Graph));
	grape->head = graph_new(elementSize);
	grape->tail = graph_new(elementSize);
	grape->width = width;
	atexit(graph_delete_all);
	return grape;
}

void * graph_delete(GraphNode ** node)
{
	void * node_data = NULL;

	if ((*node) == NULL)
	{
		slog("Error: cannot delete a null graph node!");
		return NULL;
	}

	if ((*node)->up_node != NULL)
	{
		if ((*node)->down_node != NULL)
		{
			(*node)->up_node->down_node = (*node)->down_node;
		}
		else
		{
			(*node)->up_node->down_node = NULL;
		}
	}
	if ((*node)->right_node != NULL)
	{
		if ((*node)->left_node != NULL)
		{
			(*node)->right_node->left_node = (*node)->left_node;
		}
		else
		{
			(*node)->right_node->left_node = NULL;
		}
	}
	if ((*node)->down_node != NULL)
	{
		if ((*node)->up_node != NULL)
		{
			(*node)->down_node->up_node = (*node)->up_node;
		}
		else
		{
			(*node)->down_node->up_node = NULL;
		}
	}
	if ((*node)->left_node != NULL)
	{
		if ((*node)->right_node != NULL)
		{
			(*node)->left_node->right_node = (*node)->right_node;
		}
		else
		{
			(*node)->left_node->right_node = NULL;
		}
	}

	node_data = (*node)->data;
	(*node)->data = NULL;
	graph_free_node((*node), (*node)->elementSize);
	return node_data;
}

int graph_delete_all(Graph ** graph)
{
	GraphNode * hor_iter = NULL;
	GraphNode * ver_iter = NULL;
	GraphNode * temp = NULL;

	if (graph != NULL)
	{
		slog("Error: trying to delete a graph that is NULL");
		return -1;
	}

	hor_iter = (*graph)->head;
	ver_iter = (*graph)->head;

	while (ver_iter != NULL)
	{
		hor_iter = ver_iter;
		ver_iter = ver_iter->down_node;
		while (hor_iter != NULL)
		{
			temp = hor_iter;
			hor_iter = hor_iter->right_node;
			graph_delete(temp);
		}
	}

	return 0;
}

int graph_insert(Graph ** graph, void * data, int width, size_t elementSize)
{
	GraphNode * node = graph_new(elementSize);
	GraphNode * temp = NULL;
	//GraphNode * temp = node;
	if (node == NULL)
	{
		return -1;
	}

	node->data = data;
	node->elementSize = elementSize;

	//if the node is the first one to be added
	if ((*graph)->head == NULL)
	{
		(*graph)->head = node;
		(*graph)->tail = node;
		(*graph)->head->x = 0;
		(*graph)->head->y = 0;
	}
	//otherwise, gotta add shit on
	else
	{
		if ((*graph)->tail->x + 1 < width)		//if x + 1 < width...
		{
			node->x = (*graph)->tail->x + 1;	//new node has x++
			node->y = (*graph)->tail->y;		//new node's y is still the same
			(*graph)->tail->right_node = node;	//iterator's right = new node
			node->left_node = (*graph)->tail;	//new node's left = iterator
			(*graph)->tail = node;				//iterator = new node

			/*if ((*graph)->prevRow[node->x] != NULL) //check if reference[x] exists, otherwise we are making first row
			{
				(*graph)->prevRow[node->x]->down_node = node;	//reference[x]'s bottom = new node
				node->up_node = (*graph)->prevRow[node->x];		//new node's top = reference[x]
			}
			(*graph)->prevRow[node->x] = node;	//make reference[x] = new node
			if (prevRow[node->x] != NULL)
			{
				prevRow[node->x]->down_node = node;
				node->up_node = prevRow[node->x];
			}
			prevRow[node->x] = node;*/

			if (node->y != 0)					//if we aren't on the first row
			{
				temp = (*graph)->head;			//temp = start
				while (temp->y < node->y - 1)	//go to new node's y - 1
				{
					temp = temp->down_node;
				}
				while (temp->x < node->x)		//go to new node's x
				{
					temp = temp->right_node;
				}
				temp->down_node = node;			//temp's bottom = new node
				node->up_node = temp;			//new node's top = temp
			}
		}
		else //now it's time to move to the next row
		{
			node->x = 0;						//new node has x = 0
			(*graph)->tail = (*graph)->head;	//set iterator = start
			while ((*graph)->tail->down_node != NULL) //go down until next down pointer is NULL
			{
				(*graph)->tail = (*graph)->tail->down_node;
			}
			node->y = (*graph)->tail->y + 1;	//new node has y++
			(*graph)->tail->down_node = node;	//iterator's bottom = new node
			node->up_node = (*graph)->tail;		//new node's top = iterator
			(*graph)->tail = node;				//iterator = new node
			//prevRow[0] = node; //make reference[0] = new node
		}
	}

	if ((*graph)->tail == NULL)
	{
		(*graph)->tail = node;
	}

	return 0;
}

Graph * graph_load_from_tilemap(TileMap * tilemap, size_t elementSize, short unsigned int charToInt)
{
	Graph * graph = graph_init(tilemap->width, elementSize);
	int i = 0;
	int maxNodes = 0;
	if (!graph)
	{
		//error slog in graph_new
		return NULL;
	}
	if (!tilemap)
	{
		slog("Error: cannot load a graph from an empty tilemap");
		return NULL;
	}
	maxNodes = tilemap->height * tilemap->width;

	for (i = 0; i < maxNodes; i++)
	{
		if (charToInt)
		{
			graph_insert(graph, tilemap->map[i] - '0', tilemap->width, elementSize);
		}
		else
		{
			graph_insert(graph, tilemap->map[i], tilemap->width, elementSize);
		}
	}

	return graph;
}

GraphNode * graph_find_node(Graph ** graph, unsigned int x, unsigned int y)
{
	GraphNode * iter = NULL;

	if (graph == NULL)
	{
		slog("Error: cannot find node from an empty graph");
		return NULL;
	}

	iter = graph_new((*graph)->head->elementSize);
	iter = (*graph)->head;

	while (iter != NULL && iter->x < x)
	{
		iter = iter->right_node;
	}
	if (iter == NULL || iter->x != x)
	{
		slog("Error: node with x value (%i) does not exist in graph", x);
		return NULL;
	}
	while (iter != NULL && iter->y < y)
	{
		iter = iter->down_node;
	}
	if (iter == NULL || iter->y != y)
	{
		slog("Error: node with y value (%i) does not exist in graph", y);
		return NULL;
	}

	slog("Found node x (%i) y (%i) data (%i)", iter->x, iter->y, iter->data);
	return iter;
}

void graph_traverse(Graph ** graph)
{

}

int graph_a_star(GraphNode ** start, GraphNode * goal, size_t elementSize, PriorityQueue ** endingTrail_head, PriorityQueue ** endingTrail_tail)
{
	PriorityQueue * closedNodes_head = pq_new(sizeof(GraphNode) + elementSize);
	PriorityQueue * closedNodes_tail = pq_new(sizeof(GraphNode) + elementSize);
	PriorityQueue * openNodes_head = pq_new(sizeof(GraphNode) + elementSize);
	PriorityQueue * openNodes_tail = pq_new(sizeof(GraphNode) + elementSize);
	GraphNode * q = graph_new(elementSize);
	GraphNode * successor = graph_new(elementSize);
	int i = 0;
	int j = 0;
	int stepsTaken = 0;
	Vector2D location = { 0, 0 };

	if (start == NULL)
	{
		slog("Error: start was NULL");
		return -1;
	}
	if ((*start) == NULL)
	{
		slog("Error: start was NULL");
		return -1;
	}
	if (goal == NULL)
	{
		slog("Error: goal was NULL");
		return -2;
	}

	pq_insert(openNodes_head, openNodes_tail, (*start), elementSize, 0);

	while (openNodes_tail != NULL)
	{
		q = pq_delete_min(openNodes_head, openNodes_tail);
		stepsTaken++;

		if (q == NULL)
		{
			slog("Error: could not complete A* because q was NULL");
			return -3;
		}

		for (i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				successor = q->up_node;
				break;
			case 1:
				successor = q->right_node;
				break;
			case 2:
				successor = q->down_node;
				break;
			case 3:
				successor = q->left_node;
				break;
			}

			if (successor == NULL)
			{
				//node either doesn't exist or is impassible
				continue;
			}

			successor->g = stepsTaken;
			successor->h = abs(successor->x - goal->x) + abs(successor->y - goal->y);
			successor->f = successor->g + successor->h;
			slog("data of x (%i) y(%i) is (%i)", successor->x, successor->y, successor->data);

			if (successor == goal)
			{
				successor->parent = q;
				//we have found the goal!
				while (successor != NULL)
				{
					pq_insert(endingTrail_head, endingTrail_tail, successor, sizeof(successor), j);
					slog("parenting... x (%i) y (%i) data(%i)", successor->x, successor->y, successor->data);
					successor = successor->parent;
					j++;
				}
				//return endingTrail_tail;
				return 1;
			}
			else if (successor->traversed > 0 || successor->data > 0)
			{
				//if you have already been here or can't go here
				continue;
			}
			else
			{
				successor->parent = q;
				pq_insert(openNodes_head, openNodes_tail, successor, elementSize, successor->f);
			}
		}
		q->traversed = 1;
		pq_insert(closedNodes_head, closedNodes_tail, q, elementSize, q->f);
	}
	return 0;
	
}

void graph_print(Graph ** graph)
{
	GraphNode * hor_iter = (*graph)->head;
	GraphNode * ver_iter = (*graph)->head;
	int data = 0;

	while (ver_iter != NULL)
	{
		while (hor_iter != NULL)
		{
			slog("x: (%i), y: (%i), data: (%i)", hor_iter->x, hor_iter->y, hor_iter->data);
			hor_iter = hor_iter->right_node;
		}
		ver_iter = ver_iter->down_node;
		hor_iter = ver_iter;
	}
}

GraphNode * graph_print_squiggle(Graph ** graph, int numIterations)
{
	GraphNode * iter = (*graph)->head;
	int i = 0;

	if (iter == NULL)
	{
		slog("cant squiggle :(");
		return;
	}
	slog("====SQUIGGLE====");
	slog("x: (%i), y : (%i), data : (%d)", iter->x, iter->y, iter->data);
	for (i = 0; i < numIterations; i++)
	{
		if (i % 2 == 1)
		{
			iter = iter->right_node;
		}
		else
		{
			iter = iter->down_node;
		}
		if (iter == NULL)
		{
			slog("squiggle stopped early");
			return NULL;
		}
		slog("x: (%i), y : (%i), data : (%d)", iter->x, iter->y, iter->data);
	}
	return iter;
}

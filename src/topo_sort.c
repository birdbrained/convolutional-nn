#include "topo_sort.h"

VertGraph * vert_graph_init(int numVertices)
{
	VertGraph * graph = NULL;
	graph = malloc(sizeof(VertGraph));
	if (!graph)
	{
		slog("Error: could not allocate memory for a new VertGraph!");
		return NULL;
	}
	memset(graph, 0, sizeof(VertGraph));
	graph->numVertices = numVertices;
	graph->adjacencyList = malloc((sizeof(LL_Node) + sizeof(int)) * numVertices);
	if (!graph->adjacencyList)
	{
		slog("Error: could not allocate memory for a new VertGraph's adjacencyList!");
		graph->numVertices = 0;
		memset(graph, 0, sizeof(VertGraph));
		free(graph);
		return NULL;
	}
	malloc(graph->adjacencyList, 0, (sizeof(LL_Node) + sizeof(int)) * numVertices);
	return graph;
}

void vert_graph_add_edge(VertGraph ** graph, int v, int w)
{
	if (!graph || !(*graph))
	{
		slog("Error: cannot add an edge to a null graph");
		return;
	}
	if (v > (*graph)->numVertices - 1)
	{
		slog("Error: trying to insert an edge that is out of bounds");
		return;
	}

	linkedlist_insert(&((*graph)->adjacencyList)[v], w, sizeof(int));
	//((*graph)->adjacencyList)[v] = w;
}

/**
 * @brief Recursive helper function called by topo_sort
 */
void topo_sort_helper(VertGraph * graph, int v, int visited[], Stack ** stack)
{
	LL_Node * iterator = NULL;

	visited[v] = 1;
	iterator = &graph->adjacencyList[v];
	while (iterator != NULL)
	{
		if (!visited[(int)iterator->data])
		{
			topo_sort_helper(graph, (int)iterator->data, visited, stack);
		}
	}

	stack_push(stack, v, sizeof(int));
}

void topo_sort(VertGraph * graph)
{
	Stack * stack = NULL;
	int i = 0;
	int * visited;

	if (!graph)
	{
		slog("Error: cannot do a topological sort of a null graph");
		return;
	}

	stack = stack_init(sizeof(int));
	if (!stack)
	{
		return;
	}
	visited = malloc(sizeof(int) * graph->numVertices);
	if (!visited)
	{
		slog("Error: could not allocate memory for a topo sort");
		free(stack);
		return;
	}
	memset(visited, 0, sizeof(int) * graph->numVertices);

	for (i = 0; i < graph->numVertices; i++)
	{
		if (!visited[i])
		{
			topo_sort_helper(graph, i, visited, &stack);
		}
	}

	while (stack->data != NULL)
	{
		slog("Popping (%i) from stack", stack_pop(&stack));
	}

	//clean up
	memset(visited, 0, sizeof(int) * graph->numVertices);
	free(visited);
	free(stack);
}

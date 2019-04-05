#ifndef __DS_ADJ_GRAPH__
#define __DS_ADJ_GRAPH__

#include <stdlib.h>
#include <stdio.h>
#include "simple_logger.h"
#include "ds_stack.h"

/**
 * @brief One single node of a graph
 */
typedef struct adjnode_s
{
	int destination;			/**<Where this node points to*/
	struct adjnode_s * next;	/**<If the same pnode points to more than one node*/
}AdjNode;

/**
 * @brief A list of all the nodes a node points to
 */
typedef struct adjlist_s
{
	AdjNode * head;				/**<The list of nodes*/
}AdjList;

/**
 * @brief The graph structure itself
 */
typedef struct adjgraph_s
{
	int numVertices;			/**<The total number of vertices in the graph*/
	AdjList * adjacencyList;	/**<The list of nodes in the graph*/
}AdjGraph;

/**
 * @brief Creates a new AdjNode to be added when an edge is added
 * @param dest The destination node
 * @returns A pointer to a new AdjNode if successful; NULL if could not allocate memory
 */
AdjNode * adjnode_new_node(int dest);

/**
 * @brief Creates a new AdjGraph
 * @param numVerts The total number of vertices the graph should have
 * @returns A pointer to a new AdjGraph if successful; NULL if could not allocate memory
 */
AdjGraph * adjgraph_new(int numVerts);

/**
 * @brief Adds an edge into a graph
 * @param graph The graph to insert into
 * @param src The source node
 * @param dest The destination node
 * @returns The graph with the correct insertion if successful; the original graph if unsuccessful
 */
AdjGraph * adjgraph_add_edge(AdjGraph * graph, int src, int dest);

/**
 * @brief Prints data about which nodes point to which nodes in a graph
 * @param graph The graph to print
 */
void adjgraph_print(AdjGraph * graph);

/**
 * @brief Attempts to perform a topological sort of a certain graph
 * @param graph The graph to extract a sort from
 */
void adjgraph_topological_sort(AdjGraph * graph);

#endif // !__DS_ADJ_GRAPH__


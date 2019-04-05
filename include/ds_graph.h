#ifndef __DS_GRAPH__
#define __DS_GRAPH__

#define MAX_WIDTH 100

#include <stdlib.h>
#include "simple_logger.h"
#include "dj_tilemap.h"
#include "ds_priority_queue.h"

/**
 * @brief A graph node data type
 */
typedef struct graphnode_s
{
	void *data;							/**<data the node holds*/
	int x;								/**<x position in graph (left-right)*/
	int y;								/**<y position in graph (top-bottom)*/
	short unsigned int traversed;		/**<0 if not yet traversed, 1 if traversed*/
	struct graphnode_s * up_node;		/**<These represent the nodes next to the node like a tilesheet*/
	struct graphnode_s * right_node;
	struct graphnode_s * down_node;
	struct graphnode_s * left_node;
	size_t elementSize;					/**<size of the data*/
	int f, g, h;
	struct graphnode_s * parent;
}GraphNode;

typedef struct graph_s
{
	GraphNode * head;					/**<pointer to start of graph*/
	GraphNode * tail;					/**<pointer to most recently added node, aka the end*/
	int width;							/**<how wide a row should be before moving to the next*/
	//GraphNode * prevRow[MAX_WIDTH]; /**<used for linking up and down node pointers, this is bad*/
}Graph;

/**
 * @brief Creates a new graph node
 * @param elementSize Size of the data to hold
 * @returns Pointer to new GraphNode; NULL if could not allocate memory
 */
GraphNode * graph_new(size_t elementSize);

/**
 * @brief Initializes a new graph
 * @param width Max number of nodes in a single row
 * @param elementSize Size of data each node will hold
 * @returns Pointer to new Graph; NULL if could not allocate memory
 */
Graph * graph_init(int width, size_t elementSize);

/**
 * @brief Deletes a node from a graph, and re-assigns pointers to connect holes in graph
 * @param node The node that will be deleted
 * @returns The data the node holds; NULL if node is null
 */
void * graph_delete(GraphNode ** node);

int graph_delete_all(Graph ** graph);

/**
 * @brief Inserts a new graph node into the graph
 * @param graph The graph to insert into
 * @param data The data to insert
 * @param elementSize Size of data the node will hold
 * @returns -1 if could not allocate memory; 0 if successful
 */
int graph_insert(Graph ** graph, void * data, int width, size_t elementSize);

/**
 * @brief Loads a graph from an exisiting tilemap
 * @param tilemap The tilemap to build the graph from
 * @param elementSize The size of data each node will hold
 * @param charToInt Decides if it is required to remove the '\0' from a char or not
 * @returns The completed graph; NULL if failure
 */
Graph * graph_load_from_tilemap(TileMap * tilemap, size_t elementSize, short unsigned int charToInt);

/**
 * @brief Finds a certain node in the graph
 * @param graph The graph to search in
 * @param x X value of requested node
 * @param y Y value of requested node
 * @returns Pointer to GraphNode of the found node;
 *          NULL if node either could not be found or does not exist
 */
GraphNode * graph_find_node(Graph ** graph, unsigned int x, unsigned int y);

/**
 * @brief Unused
 */
void graph_traverse(Graph ** graph);

/**
 * @brief Performs an A* search in a graph to find if a path exists from start to end
 * @param start The starting node of the search
 * @param goal The ending node of the search
 * @param elementSize Size of data a node will hold
 * @returns 1 if search successful;
 *          0 if search ended with no path found;
 *          -1 if start was NULL;
 *          -2 if goal was NULL;
 *          -3 if search failed part-way through
 */
int graph_a_star(GraphNode ** start, GraphNode * goal, size_t elementSize, PriorityQueue ** endingTrail_head, PriorityQueue ** endingTrail_tail);

/**
 * @brief Simple slog of data in graph
 * @param graph The graph to print data from
 */
void graph_print(Graph ** graph);

/**
 * @brief Print function just to test if pointers are good
 * @param graph The graph tp print data from
 * @param numIterations Number of steps to take
 */
GraphNode * graph_print_squiggle(Graph ** graph, int numIterations);

#endif // !__DS_GRAPH__
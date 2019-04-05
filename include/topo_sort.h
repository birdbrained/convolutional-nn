#ifndef __TOPO_SORT__
#define __TOPO_SORT__

#include <stdlib.h>
#include "linkedlist.h"
#include "ds_stack.h"
#include "simple_logger.h"

typedef struct vertgraph_s
{
	int numVertices;
	LL_Node * adjacencyList;
}VertGraph;

VertGraph * vert_graph_init(int numVertices);
void vert_graph_add_edge(VertGraph ** graph, int v, int w);
void topo_sort(VertGraph * graph);

#endif // !__TOPO_SORT__

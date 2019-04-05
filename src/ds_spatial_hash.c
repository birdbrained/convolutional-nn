#include "ds_spatial_hash.h"

static Hashmap * Buckets = NULL;
static int SpaceWidth = 0;
static int SpaceHeight = 0;
static int CellSize = 0;
static int TotalCells = 0;

Hashmap * spatial_setup(int spaceWidth, int spaceHeight, int cellSize, size_t elementSize)
{
	int numColumns = spaceWidth / cellSize;
	int numRows = spaceHeight / cellSize;
	int totalCells = numColumns * numRows;
	int i = 0;
	Hashmap * hash = NULL;
	char key[16] = "";

	//Buckets = hashmap_init(totalCells);
	hash = hashmap_init(totalCells);

	if (!hash)
	{
		//slog in hashmap_init
		return NULL;
	}

	for (i = 0; i < totalCells; i++)
	{
		//hashmap_insert(&Buckets, (char)i, NULL, sizeof(LL_Node) + elementSize);
		sprintf(key, "%d", i);
		hashmap_insert(&hash, key, NULL, sizeof(List) + elementSize);
	}

	SpaceWidth = spaceWidth;
	SpaceHeight = spaceHeight;
	CellSize = cellSize;
	TotalCells = totalCells;

	return hash;
}

void spatial_clear(Hashmap ** hash)
{
	int i = 0;
	List * data = NULL;
	char key[64];

	for (i = 0; i < TotalCells; i++)
	{
		//data = hashmap_get_data(Buckets, (char)i); 
		sprintf(key, "%d", i);
		data = (List *)hashmap_get_data((*hash), key);
		//while (data != NULL)
		if (data != NULL)
		{
			//linkedlist_free_node(linkedlist_remove_front(data));
			gf2d_list_delete((List *)data);
			
		}
		else
		{
			gf2d_list_delete(data);
		}
	}

	hashmap_clear(*hash);
}

List * spatial_register_body(/*Hashmap ** hashmap*/List * spaceArray[], Body * body)
{
	//LL_Node * cellIDs = spatial_body_in_buckets(body);	//Should be a list of all the cells the body is in
	//LL_Node * iterator = cellIDs;						//To iterate through linked list
	//LL_Node * temp = NULL;

	List * cellIDs = spatial_body_in_buckets(body);
	//ListElement * iterator = cellIDs->elements;
	List * temp = NULL;
	int i = 0;
	int position = 0;
	char buffer[64] = "";

	//while (iterator != NULL)
	for (i = 0; i < cellIDs->count; i++)
	{
		//iterator = &cellIDs->elements[i];
		//sprintf(buffer, "%d", (int)(cellIDs->elements[i].data));
		position = (int)(cellIDs->elements[i].data);
		//linkedlist_insert(hashmap_get_data(Buckets, (char)iterator->data), body, sizeof(Body));
		//temp = (List *)hashmap_get_data(hashmap, buffer);
		temp = spaceArray[position];
		if (temp == NULL)
		{
			//temp = linkedlist_new_node(sizeof(Body));
			//linkedlist_insert(&temp, body, sizeof(Body));
			//hashmap_insert(hashmap, buffer, &temp, sizeof(LL_Node) + sizeof(Body));
			temp = gf2d_list_new();
			temp = gf2d_list_append(temp, body);
			//hashmap_insert(hashmap, buffer, &temp, sizeof(List) + sizeof(Body));
			spaceArray[position] = temp;
		}
		else
		{
			//linkedlist_insert((LL_Node *)hashmap_get_data(hashmap, buffer), body, sizeof(Body));
			//linkedlist_insert(temp, body, sizeof(Body));
			//(temp) = gf2d_list_append(temp, body);
			spaceArray[position] = gf2d_list_append(spaceArray[position], body);
		}
		//iterator = iterator->next;
	}

	//clean up
	//while (cellIDs != NULL)
	//{
		//linkedlist_remove_front(&cellIDs);
		gf2d_list_delete(cellIDs);
	//}

		return spaceArray;
}

List * spatial_body_in_buckets(Body * body)
{
	//LL_Node * ll = linkedlist_new_node(sizeof(int));
	List * ll = gf2d_list_new_size(16);
	Vector2D min = vector2d(0, 0);
	Vector2D max = vector2d(0, 0);
	float width = 0.0f;
	
	switch (body->shape->type) //determine the boundaries to check where a body is
	{
	case ST_RECT:
		min.x = body->position.x - (body->shape->s.r.w / 2);
		min.y = body->position.y - (body->shape->s.r.h / 2);
		max.x = body->position.x + (body->shape->s.r.w / 2);
		max.y = body->position.y + (body->shape->s.r.h / 2);
		break;
	case ST_CIRCLE:
		min.x = body->position.x - body->shape->s.c.r;
		min.y = body->position.y - body->shape->s.c.r;
		max.x = body->position.x + body->shape->s.c.r;
		max.y = body->position.y + body->shape->s.c.r;
		break;
	default:
		slog("Error: unknown shape type");
		return;
		break;
	}
	width = SpaceWidth / CellSize;

	//check each corner and add each corner to the bucket/grid space it's on
	//top left
	spatial_add_buckets(min, width, &ll);
	//top right
	spatial_add_buckets(vector2d(max.x, min.y), width, &ll);
	//bottom left
	spatial_add_buckets(vector2d(min.x, max.y), width, &ll);
	//bottom right
	spatial_add_buckets(max, width, &ll);

	return ll;
}

void spatial_add_buckets(Vector2D position, float width, /*Uint8 bodyID,*/ List ** bucket)
{
	//LL_Node ** bucket holds numbers corresponding to which cell(s) a body is in
	int cellNum = 0;
	Uint8 bodyAlreadyInBucket = 0;
	LL_Node * iterator = NULL;
	
	cellNum = (int)(floor(position.x / CellSize) + floor(position.y / CellSize) * width);	//the id of the one point in a cell
	//iterator = (*bucket);

	/*while (iterator != NULL)
	{
		if ((int)iterator->data == cellNum)  //if the same id has already been accounted for...dont add it again
		{
			bodyAlreadyInBucket = 1;
			break;
		}
		iterator = iterator->next;
	}*/

	if (gf2d_list_find_data(*bucket, cellNum) != NULL)
	{
		bodyAlreadyInBucket = 1;
	}

	if (!bodyAlreadyInBucket)
	{
		//linkedlist_insert(&bucket, cellNum, sizeof(int));
		(*bucket) = gf2d_list_append(*bucket, cellNum);
	}
}

List * spatial_update(Space * space, /*Hashmap ** hashmap*/List * spaceArray[]/*, int spaceWidth, int spaceHeight, int cellSize, size_t elementSize*/)
{
	int numBodies = 0;
	int i = 0;
	Body * body = NULL;

	numBodies = gf2d_list_get_count(space->bodyList);

	//step 1: clear the hashmap
	//spatial_clear(hashmap);
	for (i = 0; i < 100; i++)
	{
		if (spaceArray[i] != NULL)
		{
			gf2d_list_delete(spaceArray[i]);
			spaceArray[i] = gf2d_list_new();
		}
	}
	//hashmap_clear(*hashmap);
	//(*hashmap) = spatial_setup(spaceWidth, SpaceHeight, cellSize, elementSize);

	for (i = 0; i < numBodies; i++)
	{
		body = (Body*)gf2d_list_get_nth(space->bodyList, i);
		if (!body)
		{
			continue;
		}

		//step 2: rehash all bodies into the hashmap
		spaceArray = spatial_register_body(spaceArray, body);
	}

	//step 3: test collisions within the hashmap...handled in order function
	return spaceArray;
}

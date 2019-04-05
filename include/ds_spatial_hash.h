#ifndef __DS_SPATIAL_HASH__
#define __DS_SPATIAL_HASH__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "simple_logger.h"
#include "ds_hashmap.h"
#include "linkedlist.h"
#include "gf2d_list.h"
#include "gf2d_vector.h"
#include "gf2d_collision.h"

/**
 * @brief Sets up the hashmap "Buckets" that will be used to determine the spatial hash
 * @param spaceWidth The width of the entire space collisions can happen on
 * @param spaceHeight The height of the entire space collisions can happen on
 * @param cellSize The width and height each cell will be (always square shaped)
 * @param elementSize The size of the collision-enabled object
 * @returns A pointer to a new hashmap if setup was successful; NULL if could not allocate memory
 */
Hashmap * spatial_setup(int spaceWidth, int spaceHeight, int cellSize, size_t elementSize);

/**
 * @brief Clears the data from each node in the spatial hashmap
 * @param hash The hashmap to clear
 */
void spatial_clear(Hashmap ** hash);

/**
 * @brief Determines the buckets a body sits in, and assigns it to those buckets
 * @param hashmap The hashmap to add the body to
 * @param body The body is register
 */
List * spatial_register_body(/*Hashmap ** hashmap*/List * spaceArray[], Body * body);
//void spatial_register_body(Hashmap ** hashmap, Body * body);

/**
 * @brief Does the determining for spatial_register_body
 * @param body The body to lookup
 * @returns A linked list of all of the buckets the body is in
 */
List * spatial_body_in_buckets(Body * body);

/**
 * @brief Adds the "buckets" into the spatial hash (areas to break collisions down into)
 * @param position The position of the point to check
 * @param width The number of cells/buckets in one row
 * @param bucket The list of cell numbers a body is in, will get stuff added to it
 */
void spatial_add_buckets(Vector2D position, float width, /*Uint8 bodyID,*/ List ** bucket);

/**
 * @brief Updates the spatial hash based on what is currently in the space
 * @param The space that holds all of the bodies to check
 * @param hashmap The spatial hashmap to update
 */
List * spatial_update(Space * space, /*Hashmap ** hashmap*/List * spaceArray[]/*, int spaceWidth, int spaceHeight, int cellSize, size_t elementSize*/);
//void spatial_update(Space * space, Hashmap ** hashmap, int spaceWidth, int spaceHeight, int cellSize, size_t elementSize);

#endif // !__DS_SPATIAL_HASH__


#ifndef __DS_HASHMAP__
#define __DS_HASHMAP__

#include <stdlib.h>
#include <string.h>
#include "simple_logger.h"

#define MAX_KEY_LENGTH 32		/**<Maximum length for the key*/

/**
 * @brief A key/value pair of data
 */
typedef struct hashmapnode_s
{
	char key[MAX_KEY_LENGTH];	/**<The key of the data*/
	void * value;				/**<Value associated with a key*/
	size_t elementSize;			/**<Size of the data to hold*/
}HashmapNode;

/**
 * @brief An entire collection of key/value pairs
 */
typedef struct hashmap_s
{
	HashmapNode * map;			/**<All of the key/value pairs*/
	unsigned int maxNodes;		/**<Number of nodes in the hashmap*/
}Hashmap;

//unsigned long crappy_hash(char * str);

/**
 * @brief Initializes a new hashmap
 * @param maxNodes The max number of nodes for the hashmap
 * @returns A pointer to a new hashmap; NULL if could not allocate memory
 */
Hashmap * hashmap_init(unsigned int maxNodes);

/**
 * @brief Initializes a new hashmap node
 * @param elementSize The size of the element to hold
 * @returns A pointer to a new hashmap node; NULL if could not allocate memory
 */
HashmapNode * hashmap_new_node(size_t elementSize);

/**
 * @brief Called whenever the hashmap fills up, doubles the size of the hashmap and re-hashes all key/value pairs
 * @param hashbrown The hashmap to re-hash, it will get cleared from memory
 * @returns A re-sized hashmap with all elements from old hashmap re-hashed if successful; NULL is hashbrown was NULL
 */
Hashmap * hashmap_rehash(Hashmap * hashbrown);

/**
 * @brief Inserts a new key/value pair into a hashmap
 * @param hashbrown The hashmap to insert into
 * @param key The key of the pair
 * @param data The value of the pair
 * @param elementSize The size of the data to hold
 * @returns 0 if successful; -1 if hashbrown was null; -2 if could not allocate memory for a new node; -3 if same exact key already in hashbrown
 */
int hashmap_insert(Hashmap ** hashbrown, char * key, void * data, size_t elementSize);

/**
 * @brief Searches a hashmap for a value from a given key, does not delete pair from hashmap
 * @param hashbrown The hashmap to search from
 * @param key The key to lookup
 * @returns The data associated with the key; NULL if key was not found or if hashbrown was NULL
 */
void * hashmap_get_data(Hashmap * hashbrown, char * key);

/**
 * @brief Removes a key/value pair from the hashmap and returns the value
 * @param hashbrown The hashmap to search from
 * @param key The key to lookup
 * @returns The data associated with the key; NULL if key was not found or if hashbrown was NULL
 */
void * hashmap_delete(Hashmap ** hashbrown, char * key);

/**
 * @brief Clears an entire hashmap from memory
 * @param hashbrown The hashmap to clear
 * @returns 0 if successful; -1 if hashbrown was NULL
 */
int hashmap_clear(Hashmap * hashbrown);

/**
 * @brief Iterates through a hashmap and prints its valid key/value pairs
 * @param hashbrown The hashmap to iterate through
 */
void hashmap_print(Hashmap * hashbrown);

#endif // !__DS_HASHMAP__

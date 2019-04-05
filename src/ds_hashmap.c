#include "ds_hashmap.h"

/**
 * @brief Totes the bestest hash eva ~<3
 * @param str String you want to hash
 * @returns A hashed unsigned long
 */
unsigned long crappy_hash(char * str)
{
	int i = 0;
	unsigned long hash = 0;

	if (!str)
	{
		str = "0";
	}

	while (str[i] != '\0')
	{
		hash += (int)str[i];
		i++;
	}

	return hash;
}

Hashmap * hashmap_init(unsigned int maxNodes)
{
	Hashmap * hashbrown;
	hashbrown = (Hashmap *)malloc(sizeof(Hashmap));
	if (!hashbrown)
	{
		slog("Error: could not allocate memory for a new hashmap");
		return NULL;
	}
	memset(hashbrown, 0, sizeof(Hashmap));
	hashbrown->map = (HashmapNode *)malloc(sizeof(HashmapNode) * maxNodes);
	if (!hashbrown->map)
	{
		slog("Error: could not allocate memory for the hashmap's map");
		return NULL;
	}
	memset(hashbrown->map, 0, sizeof(HashmapNode) * maxNodes);
	hashbrown->maxNodes = maxNodes;
	return hashbrown;
}

HashmapNode * hashmap_new_node(size_t elementSize)
{
	HashmapNode * node;
	node = (HashmapNode *)malloc(sizeof(HashmapNode) + elementSize);
	if (!node)
	{
		slog("Error: could not allocate memory for a new hashmap node");
		return NULL;
	}
	memset(node, 0, sizeof(HashmapNode) + elementSize);
	node->elementSize = elementSize;
	return node;
}

Hashmap * hashmap_rehash(Hashmap * hashbrown)
{
	Hashmap * new_map = NULL;
	char key[MAX_KEY_LENGTH] = "";
	void * data = NULL;
	int i = 0;
	if (!hashbrown)
	{
		slog("Error: cannot re-hash a null hashmap");
		return NULL;
	}

	new_map = hashmap_init(hashbrown->maxNodes * 2);
	for (i = 0; i < hashbrown->maxNodes; i++)
	{
		if (strncmp(hashbrown->map[i].key, "", MAX_KEY_LENGTH) != 0)
		{
			strncpy(key, hashbrown->map[i].key, MAX_KEY_LENGTH);
			data = hashbrown->map[i].value;
			hashmap_insert(&new_map, key, data, hashbrown->map[i].elementSize);
		}
	}

	hashmap_clear(hashbrown);
	return new_map;
}

int hashmap_insert(Hashmap ** hashbrown, char * key, void * data, size_t elementSize)
{
	unsigned long _key = crappy_hash(key);
	int index = 0;
	int _index = 0;
	HashmapNode * new_node = hashmap_new_node(elementSize);

	if (!hashbrown)
	{
		slog("Error: cannot insert into a null hashmap");
		return -1;
	}
	if (!new_node)
	{
		//error slog in hashmap_new_code
		return -2;
	}
	if (!key)
	{
		key = "0";
	}

	strncpy(new_node->key, key, MAX_KEY_LENGTH);
	new_node->value = data;
	index = _key % (*hashbrown)->maxNodes;
	_index = index;

	//if ((*hashbrown)->map[index].key == NULL)
	if (strncmp((*hashbrown)->map[index].key, "", MAX_KEY_LENGTH) == 0)
	{
		(*hashbrown)->map[index] = (*new_node);
	}
	else if (strncmp((*hashbrown)->map[index].key, key, MAX_KEY_LENGTH) == 0)
	{
		//slog("Warning: That exact key already exists in the hashmap!");
		return -3;
	}
	else
	{
		while (strncmp((*hashbrown)->map[index].key, "", MAX_KEY_LENGTH) != 0)
		{
			index++;
			if (_index == index)
			{
				//slog("Error: hashmap was full");
				//return -3;
				(*hashbrown) = hashmap_rehash((*hashbrown));
				index = _key % (*hashbrown)->maxNodes;
				_index = index;
			}
			else if (index >= (*hashbrown)->maxNodes)
			{
				index = 0;
				if (_index == index)
				{
					//slog("Error: hashmap was full");
					//return -3;
					(*hashbrown) = hashmap_rehash((*hashbrown));
					index = _key % (*hashbrown)->maxNodes;
					_index = index;
				}
			}
		}
		(*hashbrown)->map[index] = (*new_node);
	}

	return 0;
}

void * hashmap_get_data(Hashmap * hashbrown, char * key)
{
	unsigned long _key = crappy_hash(key);
	void * data = NULL;
	int index = 0;
	int _index = 0;
	if (!hashbrown)
	{
		slog("Error: could not get data from a null hashmap");
		return NULL;
	}
	index = _key % hashbrown->maxNodes;
	_index = index;

	if (&hashbrown->map[index] != NULL)
	if (strncmp(hashbrown->map[index].key, key, MAX_KEY_LENGTH) == 0)
	{
		data = hashbrown->map[index].value;
	}
	else
	{
		while (strncmp(hashbrown->map[index].key, "", MAX_KEY_LENGTH) != 0)
		{
			index++;
			if (_index == index)
			{
				slog("Error: could not find key (%s) in hashmap", key);
				return NULL;
			}
			else if (index >= hashbrown->maxNodes)
			{
				index = 0;
				if (_index == index)
				{
					slog("Error: could not find key (%s) in hashmap", key);
					return NULL;
				}
			}
			else
			{
				if (strncmp(hashbrown->map[index].key, key, MAX_KEY_LENGTH) == 0)
				{
					data = hashbrown->map[index].value;
					break;
				}
			}
		}
	}

	return data;
}

void * hashmap_delete(Hashmap ** hashbrown, char * key)
{
	unsigned long _key = crappy_hash(key);
	void * data = NULL;
	int index = 0;
	int _index = 0;
	if (!hashbrown)
	{
		slog("Error: could not delete a node from a null hashmap");
		return NULL;
	}
	index = _key % (*hashbrown)->maxNodes;
	_index = index;

	if (strncmp((*hashbrown)->map[index].key, key, MAX_KEY_LENGTH) == 0)
	{
		data = (*hashbrown)->map[index].value;
		//memset(&(*hashbrown)->map[index], 0, sizeof(HashmapNode) + (*hashbrown)->map[index].elementSize);
		strncpy((*hashbrown)->map[index].key, "", MAX_KEY_LENGTH);
		(*hashbrown)->map[index].value = NULL;
		(*hashbrown)->map[index].elementSize = 0;
	}
	else
	{
		index++;
		while (index != _index)
		{
			if (index >= (*hashbrown)->maxNodes)
			{
				index = 0;
				if (_index == index)
				{
					slog("Error: could not find key (%s) in hashmap", key);
					return NULL;
				}
			}
			else
			{
				if (strncmp((*hashbrown)->map[index].key, key, MAX_KEY_LENGTH) == 0)
				{
					data = (*hashbrown)->map[index].value;
					//memset(&(*hashbrown)->map[index], 0, sizeof(HashmapNode) + (*hashbrown)->map[index].elementSize);
					strncpy((*hashbrown)->map[index].key, "", MAX_KEY_LENGTH);
					(*hashbrown)->map[index].value = NULL;
					(*hashbrown)->map[index].elementSize = 0;
					break;
				}
				index++;
			}
		}
	}

	return data;
}

/**
 * @brief Clears one node from memory
 * @param node The node to clear
 * @param elementSize The size of the data the node holds
 * @returns 0 if successful, -1 if node was null
 */
int hashmap_clear_node(HashmapNode * node, size_t elementSize)
{
	if (!node)
	{
		slog("Error: could not clear a node that is null");
		return -1;
	}
	if (node->value != NULL)
	{
		slog("Error: data still present in node");
		return -2;
	}

	memset(node, 0, sizeof(HashmapNode) + elementSize);
	//free(node);
	return 0;
}

 int hashmap_clear(Hashmap * hashbrown)
{
	int i = 0;

	if (!hashbrown)
	{
		slog("Error: could not clear a hashmap that is null");
		return -1;
	}

	for (i = 0; i < hashbrown->maxNodes; i++)
	{
		//slog("i (%i) key (%s)", i, hashbrown->map[i].key);
		//if (strncmp(hashbrown->map[i].key, "", MAX_KEY_LENGTH) != 0)
		//{
			strncpy(hashbrown->map[i].key, "", MAX_KEY_LENGTH);
			hashbrown->map[i].value = NULL;
			hashbrown->map[i].elementSize = 0;
			hashmap_clear_node(&hashbrown->map[i], hashbrown->map[i].elementSize);
		//}
	}
	
	memset(hashbrown, 0, sizeof(Hashmap));
	free(hashbrown);
	return 0;
}

void hashmap_print(Hashmap * hashbrown)
{
	int i = 0;
	if (!hashbrown)
	{
		slog("Error: cannot print from a null hashmap");
		return;
	}
	for (i = 0; i < hashbrown->maxNodes; i++)
	{
		//if (strncmp(hashbrown->map[i].key, "", MAX_KEY_LENGTH) != 0)
		//{
			slog("key: (%s) value: (%d)", hashbrown->map[i].key, hashbrown->map[i].value);
		//}
	}
}

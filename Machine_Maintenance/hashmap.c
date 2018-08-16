#include <stdlib.h>
#include "hashmap.h"

HashmapNode* hashmap_new_node(unsigned int hash_index,int values_size)
{
  HashmapNode *hashmap_node = malloc(sizeof(HashmapNode));
  hashmap_node->hash_index = hash_index;
  hashmap_node->values_size = values_size;
  hashmap_node->values = malloc(sizeof(values_size * sizeof(int)));

  return hashmap_node;
}

HashMap* hashmap_new(int size)
{
  int i;
  HashMap *hashmap = (HashMap*)malloc(sizeof(HashMap));
  hashmap->node_list = malloc(size * sizeof(HashmapNode*));
  hashmap->element_count = 0;
  hashmap->map_size = size;

  for(i = 0; i < size; i++)
  {
    hashmap->node_list[i] = malloc(sizeof(HashmapNode));
    hashmap->node_list[i] = NULL;
  }
  return hashmap;
}

unsigned int hashmap_hash_func(HashMap *hashmap, unsigned int key)
{
   int hash = key;

   hash = (hash >> 3) * 2654435761;
   hash = hash % hashmap->map_size;
   return hash;
}

void hashmap_expand(HashMap *hashmap)
{
  int i;
  int hash_index;
  int old_size = hashmap->map_size;
  HashmapNode *hashmap_node;

  hashmap->map_size = old_size * 2;
  HashmapNode **new_node_list = malloc(hashmap->map_size * sizeof(HashmapNode*));

  for(i = 0; i < hashmap->map_size; i++)
  {
    new_node_list[i] = malloc(sizeof(HashmapNode));
    new_node_list[i] = NULL;
  }

  for(i = 0; i < old_size; i++)
  {
    hashmap_node = hashmap->node_list[i];
    if(hashmap_node != NULL)
    {
      hash_index = hashmap_hash_func(hashmap, hashmap_node->hash_index);
      hashmap_node->hash_index = hash_index;
      new_node_list[hash_index] = hashmap_node;
    }
    free(hashmap_node);
  }
  hashmap->node_list = new_node_list;
}

void hashmap_delete(HashMap *hashmap, int key)
{
  int hash = hashmap_hash_func(hashmap, key);
  HashmapNode *hashmap_node = hashmap->node_list[hash];
  hashmap_node->values = NULL;
  hashmap_node->hash_index = 0;
  hashmap_node->values_size = 0;
}

HashmapNode* hashmap_lookup(HashMap *hashmap, unsigned int key)
{
  unsigned int hash = hashmap_hash_func(hashmap, key);
  HashmapNode *hashmap_node = hashmap->node_list[hash] ;

  if(hashmap_node == NULL)
  {
    return NULL;
  }
  else
  {
    return hashmap->node_list[hash];
  }
}

void hashmap_insert(HashMap *hashmap, unsigned int key, int value)
{
  unsigned int hash = hashmap_hash_func(hashmap, key);
  double map_percentage;
  HashmapNode *hashmap_node = hashmap->node_list[hash];

  map_percentage = (double)hashmap->element_count / (double)hashmap->map_size;

  if(map_percentage >= 0.75)
  {
    hashmap_expand(hashmap);
  }

  if(hashmap_node != NULL && hashmap->node_list[hash]->hash_index == key)
  {
    return;
  }
  else
  {
    hashmap_node = hashmap_new_node(hash, value);
    hashmap->element_count++;
  }

}

void hashmap_update_value(HashMap *hashmap, unsigned int key, int value)
{
  HashmapNode *hashmap_node = hashmap_lookup(hashmap, key);

  if(hashmap_node != NULL && hashmap->node_list[hash]->hash_index == key)
  {
    return;
  }
  else
  {
    hashmap_node->value = value;
  }
}

void hashmap_destroy(HashMap *hashmap)
{
  int i;

  for(i = 0; i < hashmap->map_size; i++)
  {
    free(hashmap->node_list[i]);
  }
  free(hashmap);
}

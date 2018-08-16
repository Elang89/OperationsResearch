#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashmapNode{
  unsigned int hash_index;
  int value;
}HashmapNode;

typedef struct HashMap{
  int element_count;
  int map_size;
  HashmapNode ** node_list;
}HashMap;

HashmapNode* hashmap_new_node(unsigned int hash_index, int * values);
HashMap* hashmap_new(int size);
unsigned int hashmap_hash_func(HashMap * hashmap, unsigned int key);
void hashmap_expand(HashMap *hashmap);
void hashmap_delete(HashMap *hashmap, int key);
void hashmap_insert(HashMap *hashmap, unsigned int key, int * values, int values_size);
HashmapNode* hashmap_lookup(HashMap *hashmap, unsigned int key);
void hashmap_destroy(HashMap *hashmap);


#endif

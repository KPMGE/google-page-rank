#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACITY 1301

typedef struct Ht_item Ht_item;

typedef struct LinkedList LinkedList;

typedef struct HashTable HashTable;

// static LinkedList* allocate_list ();
// static LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item);
// static Ht_item* linkedlist_remove(LinkedList* list);
// static void free_linkedlist(LinkedList* list);
// static LinkedList** create_overflow_buckets(HashTable* table);
// static void free_overflow_buckets(HashTable* table);

Ht_item* create_item(char* key);
HashTable* create_table(int size);
void free_item(Ht_item* item);
void free_table(HashTable* table);
void handle_collision(HashTable* table, unsigned long index, Ht_item* item);
void ht_insert(HashTable* table, char* key);
bool ht_search(HashTable* table, char* key);
void search_world(HashTable* table, char* key);
void print_table(HashTable* table);

#endif

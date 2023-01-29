#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CAPACITY 1301

typedef struct HashTable HashTable;

HashTable* ht_new(int size);
void ht_free(HashTable* table);
void ht_insert(HashTable* table, char* key);
bool ht_search(HashTable* table, char* key);
void search_word(HashTable* table, char* key);
void print_table(HashTable* table);

#endif

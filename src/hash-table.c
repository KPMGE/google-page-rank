#include "../include/hash-table.h"
#include "../include/linked-list.h"
#include "../include/utils.h"
#include <strings.h>
#include <assert.h>

#define LARGEST_PRIME 65521

struct HashTable {
  char **items;
  LinkedList **overflow_buckets;
  int size;
  int count;
};

static unsigned long hash(const char *s) {
  unsigned long s1 = 1;
  unsigned long s2 = 0;
  for (int i = 0; s[i]; i++) {
    s1 = (s1 + s[i]) % LARGEST_PRIME; 
    s2 = (s1 + s2) % LARGEST_PRIME;
  }
  return ((s2 << 16) | s1) % CAPACITY;
}

static void free_overflow_buckets(HashTable *table) {
  LinkedList **buckets = table->overflow_buckets;

  for (int i = 0; i < table->size; i++) {
    linked_list_free(buckets[i]);
  }

  free(buckets);
}

HashTable *ht_new(int size) {
  HashTable *table = malloc(sizeof(HashTable));
  table->size = size;
  table->count = 0;
  table->items = calloc(table->size, sizeof(char *));
  table->overflow_buckets = linked_list_vec_new(table->size);

  return table;
}

void ht_free(HashTable *table) {
  for (int i = 0; i < table->size; i++) {
    char *item = table->items[i];
    if (item) {
      free(item);
    }
  }

  free_overflow_buckets(table);
  free(table->items);
  free(table);
}

static void handle_collision(HashTable *table, unsigned long index, char *item) {
  LinkedList *head = table->overflow_buckets[index];

  if (!head) {
    head = linked_list_init();
    linked_list_insert(head, item);
    table->overflow_buckets[index] = head;
    return;
  } 

  linked_list_insert(head, item);
}

void ht_insert(HashTable *table, char *key) {
  str_to_lower(&key);

  unsigned long index = hash(key);
  char *current_item = table->items[index];

  if (!current_item) {
    assert((table->count < table->size) && "table is full!");
    table->items[index] = strdup(key);
    table->count++;
    return;
  } 

  handle_collision(table, index, key);
}

bool ht_search(HashTable *table, char *key) {
  str_to_lower(&key);

  int index = hash(key);
  char *item = table->items[index];

  if (!item) return false;
  if (!strcasecmp(item, key)) return true;

  LinkedList *head = table->overflow_buckets[index];

  return head ? linked_list_has_page(head, key) : false;
}

#include "../include/hash-table.h"
#include "../include/utils.h"
#include <strings.h>

#define LARGEST_PRIME 65521

struct Ht_item {
  char *key;
};

struct LinkedList {
  Ht_item *item;
  LinkedList *next;
};

struct HashTable {
  Ht_item **items;
  LinkedList **overflow_buckets;
  int size;
  int count;
};

static unsigned long hash_function(const char *s) {
  unsigned long s1 = 1;
  unsigned long s2 = 0;
  for (int i = 0; s[i]; i++) {
    s1 = (s1 + s[i]) % LARGEST_PRIME; 
    s2 = (s1 + s2) % LARGEST_PRIME;
  }
  return ((s2 << 16) | s1) % CAPACITY;
}

static LinkedList *allocate_list() {
  LinkedList *list = malloc(sizeof(LinkedList));
  return list;
}

static LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item) {
  if (!list) {
    LinkedList *head = allocate_list();
    head->item = item;
    head->next = NULL;
    list = head;
    return list;
  }

  if (list->next == NULL) {
    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    list->next = node;
    return list;
  }

  LinkedList *temp = list;
  while (temp->next->next) {
    temp = temp->next;
  }

  LinkedList *node = allocate_list();
  node->item = item;
  node->next = NULL;
  temp->next = node;

  return list;
}

static Ht_item *linkedlist_remove(LinkedList *list) {
  if (!list) {
    return NULL;
  }
  if (!list->next) {
    return NULL;
  }

  LinkedList *node = list->next;
  LinkedList *temp = list;
  temp->next = NULL;
  list = node;
  Ht_item *it = NULL;

  memcpy(temp->item, it, sizeof(Ht_item));
  free(temp->item->key);
  free(temp->item);
  free(temp);

  return it;
}

static void free_linkedlist(LinkedList *list) {
  LinkedList *temp = list;
  while (list) {
    temp = list;
    list = list->next;
    free_item(temp->item);
    free(temp);
  }
}

static LinkedList **create_overflow_buckets(HashTable *table) {
  LinkedList **buckets = calloc(table->size, sizeof(LinkedList *));
  for (int i = 0; i < table->size; i++) {
    buckets[i] = NULL;
  }
  return buckets;
}

static void free_overflow_buckets(HashTable *table) {
  LinkedList **buckets = table->overflow_buckets;
  for (int i = 0; i < table->size; i++) {
    free_linkedlist(buckets[i]);
  }
  free(buckets);
}

Ht_item *create_item(char *key) {
  Ht_item *item = malloc(sizeof(Ht_item));
  item->key = strdup(key); 

  return item;
}

HashTable *create_table(int size) {
  HashTable *table = malloc(sizeof(HashTable));
  table->size = size;
  table->count = 0;
  table->items = calloc(table->size, sizeof(Ht_item *));
  for (int i = 0; i < table->size; i++)
    table->items[i] = NULL;
  table->overflow_buckets = create_overflow_buckets(table);

  return table;
}

void free_item(Ht_item *item) {
  free(item->key);
  free(item);
}

void free_table(HashTable *table) {
  for (int i = 0; i < table->size; i++) {
    Ht_item *item = table->items[i];
    if (item != NULL) {
      free_item(item);
    }
  }

  free_overflow_buckets(table);
  free(table->items);
  free(table);
}

void handle_collision(HashTable *table, unsigned long index, Ht_item *item) {
  LinkedList *head = table->overflow_buckets[index];

  if (head == NULL) {
    head = allocate_list();
    head->item = item;
    table->overflow_buckets[index] = head;
    return;
  } 

  table->overflow_buckets[index] = linkedlist_insert(head, item);
}

void ht_insert(HashTable *table, char *key) {
  str_to_lower(&key);

  Ht_item *item = create_item(key);
  unsigned long index = hash_function(key);
  Ht_item *current_item = table->items[index];

  if (current_item == NULL) {
    if (table->count == table->size) {
      printf("Insert Error: Hash Table is full\n");
      free_item(item);
      return;
    }

    table->items[index] = item;
    table->count++;
  } else {
    handle_collision(table, index, item);
  }
}

bool ht_search(HashTable *table, char *key) {
  str_to_lower(&key);

  int index = hash_function(key);
  Ht_item *item = table->items[index];
  LinkedList *head = table->overflow_buckets[index];

  while (item != NULL) {
    if (strcasecmp(item->key, key) == 0) {
      return true;
    }
    if (head == NULL) {
      return false;
    }
    item = head->item;
    head = head->next;
  }
  return false;
}

void search_world(HashTable *table, char *key) {
  bool val;
  if ((val = ht_search(table, key)) == false) {
    printf("%s nao existe na tabela\n", key);
    return;
  }
  printf("Palavra encontrada: Key:%s\n", key);
}

void print_table(HashTable *table) {
  printf("\n-------------------\n");
  for (int i = 0; i < table->size; i++) {
    if (table->items[i]) {
      printf("Index:%d, Chave:%s", i, table->items[i]->key);
      if (table->overflow_buckets[i]) {
        printf(" => Overflow Bucket => ");
        LinkedList *head = table->overflow_buckets[i];
        while (head) {
          printf("Key:%s ", head->item->key);
          head = head->next;
        }
      }
      printf("\n");
    }
  }
  printf("-------------------\n");
}

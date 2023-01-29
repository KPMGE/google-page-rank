#include "../include/linked-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct cell Cell;

struct cell {
  char *item;
  Cell *next;
};

typedef struct linked_list {
  Cell *first;
  Cell *last;
  int amount_nodes;
} LinkedList;

LinkedList *linked_list_init() {
  LinkedList *list = malloc(sizeof(LinkedList));
  list->first = NULL;
  list->last = NULL;
  list->amount_nodes = 0;
  return list;
}

void linked_list_insert(LinkedList *list, char *item) {
  Cell *new = malloc(sizeof(Cell));
  new->item = strdup(item);
  new->next = NULL;
  list->amount_nodes++;

  if (list->first == NULL) {
    list->first = list->last = new;
    return;
  }

  list->last->next = new;
  list->last = new;
}

bool linked_list_has_item(LinkedList *list, const char *item) {
  Cell *head = list->first;

  while (head) {
    if (strcasecmp(head->item, item) == 0) {
      return true;
    }
    head = head->next;
  }

  return false;
}

void linked_list_print(LinkedList *list) {
  Cell *p;
  for (p = list->first; p != NULL; p = p->next) {
    printf(" %s ", p->item);
  }
}

void linked_list_free(LinkedList *list) {
  if (!list) return;

  Cell *head = list->first;

  while (head) {
    Cell *temp = head;
    head = head->next;
    free(temp->item);
    free(temp);
  }

  free(list);
}

int linked_list_amount_nodes(LinkedList *list) {
  return list->amount_nodes;
}

char *linked_list_at(LinkedList *list, int pos) {
  Cell *head = list->first;

  for (int i = 0; i < pos; i++) {
    head = head->next;
  }

  return head->item;
}

char **linked_list_to_items(LinkedList *list) {
  Cell *head = list->first;
  char **items = malloc(sizeof(char *) * list->amount_nodes);
  int i = 0;

  while (head) {
    items[i++] = head->item;
    head = head->next;
  }

  return items;
}

LinkedList **linked_list_vec_new(size_t size) {
  LinkedList **vec = calloc(size, sizeof(LinkedList *));
  return vec;
}

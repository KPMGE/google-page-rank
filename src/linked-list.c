#include "../include/linked-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct cell Cell;

struct cell {
  char *page;
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

void linked_list_insert(LinkedList *list, char *page) {
  Cell *new = malloc(sizeof(Cell));
  new->page = strdup(page);
  new->next = NULL;
  list->amount_nodes++;

  if (list->first == NULL) {
    list->first = list->last = new;
    return;
  }

  list->last->next = new;
  list->last = new;
}

bool linked_list_has_page(LinkedList *list, const char *page) {
  Cell *head = list->first;

  while (head) {
    if (strcasecmp(head->page, page) == 0) {
      return true;
    }
    head = head->next;
  }

  return false;
}

void linked_list_print(LinkedList *list) {
  Cell *p;
  for (p = list->first; p != NULL; p = p->next) {
    printf(" %s ", p->page);
  }
}

void linked_list_free(LinkedList *list) {
  Cell *head = list->first;

  while (head) {
    Cell *temp = head;
    head = head->next;
    free(temp->page);
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

  return head->page;
}

char **linked_list_to_pages(LinkedList *list) {
  Cell *head = list->first;
  char **pages = malloc(sizeof(char *) * list->amount_nodes);
  int i = 0;

  while (head) {
    pages[i++] = head->page;
    head = head->next;
  }

  return pages;
}

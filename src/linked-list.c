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
} LinkedList;

LinkedList *linked_list_init() {
  LinkedList *list = malloc(sizeof(LinkedList));
  list->first = NULL;
  list->last = NULL;
  return list;
}

void linked_list_insert(LinkedList *list, char *page) {
  Cell *new = malloc(sizeof(Cell));
  new->page = strdup(page);
  new->next = NULL;

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

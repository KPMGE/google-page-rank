#include "../include/red-black-tree.h"
#include "../include/generic-red-black-tree.h"
#include "../include/linked-list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct node {
  GRBT *rbt;
};

static void free_linked_list(void *data) {
  LinkedList *list = data;
  linked_list_free(list);
}

static void print_linked_list(void *data) {
  LinkedList *list = data;
  if (!list) return;
  linked_list_print(list);
}

RBT *RBT_init() {
  RBT *new = malloc(sizeof(RBT));
  new->rbt = NULL;
  return new;
}

static void* alloc_initial_linked_list() {
  return linked_list_init();
}

void RBT_insert(RBT *h, char *word) {
  h->rbt = grbt_insert(h->rbt, word, alloc_initial_linked_list);
}

RBT *RBT_search(RBT *n, char *word) {
  GRBT *node = grbt_search(n->rbt, word);

  if (!node) return NULL;

  RBT *new = RBT_init();
  new->rbt = node;

  return new;
}


void RBT_free(RBT *h) { 
  grbt_free(h->rbt, free_linked_list);
  free(h);
}

void RBT_print(RBT *h) {
  grbt_print(h->rbt, print_linked_list);
}

void RBT_add_page(RBT *h, char *key, char *page) {
  GRBT *node = grbt_search(h->rbt, key);
  if (!node) return;

  LinkedList *pages = grbt_data(node);
  if (!pages) return;

  bool page_exists = linked_list_has_page(pages, page);
  if (!page_exists) {
    linked_list_insert(pages, page); 
  }
}

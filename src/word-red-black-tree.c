#include "../include/word-red-black-tree.h"
#include "../include/generic-red-black-tree.h"
#include "../include/linked-list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

static void* alloc_initial_linked_list() {
  return linked_list_init();
}

WRBT *word_rbt_init() {
  WRBT *new = malloc(sizeof(WRBT));
  new->rbt = NULL;
  return new;
}

void word_rbt_insert(WRBT *h, char *word) {
  h->rbt = grbt_insert(h->rbt, word, alloc_initial_linked_list);
}

char **word_rbt_search(WRBT *n, char *word, int *amount_pages) {
  GRBT *node = grbt_search(n->rbt, word);
  if (!node) return NULL;

  LinkedList *list = grbt_data(node);
  char **pages = linked_list_to_pages(list);
  *amount_pages = linked_list_amount_nodes(list);

  return pages;
}

void word_rbt_free(WRBT *h) { 
  grbt_free(h->rbt, free_linked_list);
  free(h);
}

void word_rbt_print(WRBT *h) {
  grbt_print(h->rbt, print_linked_list);
}

void word_rbt_add_page(WRBT *h, char *key, char *page) {
  GRBT *node = grbt_search(h->rbt, key);
  if (!node) return;

  LinkedList *pages = grbt_data(node);
  if (!pages) return;

  bool page_exists = linked_list_has_page(pages, page);
  if (!page_exists) {
    linked_list_insert(pages, page); 
  }
}

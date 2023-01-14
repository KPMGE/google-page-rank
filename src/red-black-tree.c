#include "../include/red-black-tree.h"
#include "../include/linked-list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define RED true
#define BLACK false

typedef struct node RBT;

struct node {
  char *word;
  LinkedList *pages;
  bool color;
  RBT *l, *r;
};

RBT *RBT_create_node(char *word, bool color) {
  RBT *node = malloc(sizeof(RBT));
  node->word = strdup(word);
  node->pages = linked_list_init();
  node->color = color;
  node->l = node->r = NULL;

  return node;
}

RBT *RBT_init() {
  RBT *node = malloc(sizeof(RBT));
  node->pages = linked_list_init();
  node->color = RED;
  node->word = NULL;
  node->l = node->r = NULL;

  return node;
}

void RBT_add_page(RBT *h, char *page) {
  bool page_exists = linked_list_has_page(h->pages, page);
  if (!page_exists) {
    linked_list_insert(h->pages, page); 
  }
}

bool is_red(RBT *x) {
  if (x == NULL)
    return BLACK;
  return x->color; // RED == true
}

RBT *rotate_left(RBT *h) {
  RBT *x = h->r;
  h->r = x->l;
  x->l = h;
  x->color = x->l->color;
  x->l->color = RED;
  return x;
}

RBT *rotate_right(RBT *h) {
  RBT *x = h->l;
  h->l = x->r;
  x->r = h;
  x->color = x->r->color;
  x->r->color = RED;
  return x;
}

void flip_colors(RBT *h) {
  h->color = RED;
  h->l->color = BLACK;
  h->r->color = BLACK;
}

RBT *RBT_insert(RBT *h, char *word) {
  // Insert at bottom and color it red.
  if (h == NULL) {
    return RBT_create_node(word, RED);
  }

  int cmp = strcasecmp(word, h->word);
  if (cmp < 0) {
    h->l = RBT_insert(h->l, word);
  } else if (cmp > 0) {
    h->r = RBT_insert(h->r, word);
  } else /*cmp == 0*/ {
    h->word = strdup(word);
  }

  // Lean left.
  if (is_red(h->r) && !is_red(h->l)) {
    h = rotate_left(h);
  }
  // Balance 4-node.
  if (is_red(h->l) && is_red(h->l->l)) {
    h = rotate_right(h);
  }
  // Split 4-node.
  if (is_red(h->l) && is_red(h->r)) {
    flip_colors(h);
  }

  return h;
}


RBT *RBT_search(RBT *n, char *word) {
  while (n != NULL) {
    int cmp;
    cmp = strcasecmp(word, n->word);
    if (cmp < 0)
      n = n->l;
    else if (cmp > 0)
      n = n->r;
    else
      return n;
  }
  return NULL;
}

void RBT_print(RBT *h) {
  if (h) {
    if (h->word) {
      printf("%s -> {", h->word);
    }
    linked_list_print(h->pages);
    printf("}\n");
    RBT_print(h->l);
    RBT_print(h->r);
  }
}

void RBT_free(RBT *h) {
  if (h) {
    RBT_free(h->l);
    RBT_free(h->r);
    if (h->word) {
      free(h->word);
    }
    linked_list_free(h->pages);
    free(h);
  }
}

#include "../include/generic-red-black-tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define RED true
#define BLACK false

struct grbt {
  char *word;
  void *data;
  bool color;
  GRBT *l, *r;
};

static bool is_red(GRBT *x) {
  if (x == NULL)
    return BLACK;
  return x->color; // RED == true
}

static GRBT *rotate_left(GRBT *h) {
  GRBT *x = h->r;
  h->r = x->l;
  x->l = h;
  x->color = x->l->color;
  x->l->color = RED;
  return x;
}

static GRBT *rotate_right(GRBT *h) {
  GRBT *x = h->l;
  h->l = x->r;
  x->r = h;
  x->color = x->r->color;
  x->r->color = RED;
  return x;
}

static void flip_colors(GRBT *h) {
  h->color = RED;
  h->l->color = BLACK;
  h->r->color = BLACK;
}

static GRBT *grbt_create_node(char *word, bool color, alloc_init_data fn) {
  GRBT *node = malloc(sizeof(GRBT));

  node->word = strdup(word);
  node->data = fn();
  node->color = color;
  node->l = node->r = NULL;

  return node;
}

void *grbt_data(GRBT *h) {
  return h->data;
}

void grbt_set_data(GRBT *h, void *data) {
  h->data = data;
}

GRBT *grbt_insert(GRBT *h, char *word, alloc_init_data fn) {
  // Insert at bottom and color it red.
  if (h == NULL) {
    return grbt_create_node(word, RED, fn);
  }

  int cmp = strcasecmp(word, h->word);
  if (cmp < 0) {
    h->l = grbt_insert(h->l, word, fn);
  } else if (cmp > 0) {
    h->r = grbt_insert(h->r, word, fn);
  } else /*cmp == 0*/ {
    free(h->word);
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

GRBT *grbt_search(GRBT *n, char *word) {
  while (n) {
    int cmp = strcasecmp(word, n->word);
    if (cmp < 0) {
      n = n->l;
    } else if (cmp > 0) {
      n = n->r;
    } else {
      return n;
    }
  }
  return NULL;
}

void grbt_print(const GRBT *h, print_fn fn) {
  if (h) {
    printf("\n%s -> {", h->word);
    fn(h->data);
    printf("}\n");
    grbt_print(h->l, fn);
    grbt_print(h->r, fn);
  }
}

void grbt_free(GRBT *h, free_fn fn) {
  if (h) {
    grbt_free(h->l, fn);
    grbt_free(h->r, fn);
    free(h->word);
    fn(h->data);
    free(h);
  }
}

void traverse_tree(GRBT *h, traverse_fn fn) {
  if (h) {
    traverse_tree(h->l, fn);
    fn(h, h->data);
    traverse_tree(h->r, fn);
  }
} 

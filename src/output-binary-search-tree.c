#include "../include/output-binary-search-tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char *page_name;
  double page_rank;
} OutputData;

struct bst {
  OutputData *value;
  OutBst *left;
  OutBst *right;
};

static OutputData *alloc_out_data(char *page_name, double page_rank) {
  OutputData *d = malloc(sizeof(OutputData));
  d->page_name = strdup(page_name);
  d->page_rank = page_rank;
  return d;
}

static void free_out_data(OutputData *d) {
  if (d) {
    free(d->page_name);
  }
  free(d);
}

static void print_out_data(OutputData *out) {
  printf("\n------------------------------------\n");
  printf("page_name: %s\n", out->page_name);
  printf("page_rank: %.6lf\n", out->page_rank);
  printf("\n------------------------------------\n");
}

OutBst *output_bst_new(char *page_name, double page_rank) {
  OutBst *b = malloc(sizeof(OutBst));
  b->left = b->right = NULL;
  b->value = alloc_out_data(page_name, page_rank);
  return b;
}

OutBst *output_bst_add(OutBst *b, char *page_name, double page_rank) {
  if (!b)
    return output_bst_new(page_name, page_rank);
  if (page_rank < b->value->page_rank)
    b->left = output_bst_add(b->left, page_name, page_rank);
  if (page_rank > b->value->page_rank)
    b->right = output_bst_add(b->right, page_name, page_rank);
  return b;
}

void output_bst_free(OutBst *b) {
  if (b) {
    output_bst_free(b->left);
    output_bst_free(b->right);
    free_out_data(b->value);
    free(b);
  }
}

void output_bst_rec_in_order(OutBst *b) {
  if (!b)
    return;
  output_bst_rec_in_order(b->left);
  print_out_data(b->value);
  output_bst_rec_in_order(b->right);
}

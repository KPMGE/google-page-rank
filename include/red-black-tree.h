#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdbool.h>

typedef struct node RBT;

RBT *RBT_init();
void RBT_add_page(RBT *h, char *page);
bool is_red(RBT *x);
RBT* rotate_left(RBT *h);
RBT* rotate_right(RBT *h);
void flip_colors(RBT *h);
RBT* RBT_insert(RBT *h, char *word);
RBT *RBT_search(RBT *n, char *word);
void RBT_print(RBT *h);
void RBT_free(RBT *h);

#endif

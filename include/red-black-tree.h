#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdbool.h>

typedef struct node RBT;

RBT* RBT_init();

void RBT_add_page(RBT *h, char *key, char *page);
void RBT_insert(RBT *h, char *word);
RBT *RBT_search(RBT *n, char *word);
void RBT_print(RBT *h);
void RBT_free(RBT *h);

#endif

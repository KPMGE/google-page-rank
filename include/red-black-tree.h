#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdbool.h>

typedef struct node RBT;

void RBT_add_page(RBT *h, char *page);
RBT* RBT_insert(RBT *h, char *word);
RBT *RBT_search(RBT *n, char *word);
void RBT_print(RBT *h);
void RBT_free(RBT *h);
RBT *RBT_create_node(char *word, bool color);

#endif

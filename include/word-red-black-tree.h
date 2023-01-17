#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdbool.h>

typedef struct node WRBT;

WRBT* word_rbt_init();
void word_rbt_add_page(WRBT *h, char *key, char *page);
void word_rbt_insert(WRBT *h, char *word);
WRBT *word_rbt_search(WRBT *n, char *word);
void word_rbt_print(WRBT *h);
void word_rbt_free(WRBT *h);

#endif

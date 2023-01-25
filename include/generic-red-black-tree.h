#ifndef GEN_RBT
#define GEN_RBT
#include <stdbool.h>

typedef struct grbt GRBT;
typedef void (*print_fn)(void *);
typedef void (*free_fn)(void *);
typedef void* (*alloc_init_data)();
typedef void (*traverse_fn)(void *);

void traverse_tree(GRBT *h, traverse_fn fn);

void *grbt_data(GRBT *h);
void grbt_set_data(GRBT *h, void *data);
GRBT *grbt_insert(GRBT *h, char *word, alloc_init_data fn);
GRBT *grbt_search(GRBT *n, char *word);
void grbt_print(const GRBT *h, print_fn fn);
void grbt_free(GRBT *h, free_fn fn);

#endif // !GEN_RBT

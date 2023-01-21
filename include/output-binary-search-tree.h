#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

typedef struct bst OutBst;

OutBst *output_bst_new(char *page_name, double page_rank);
OutBst *output_bst_add(OutBst *b, char *page_name, double page_rank);
void output_bst_free(OutBst *b);
void output_bst_print_page_rank_in_order(OutBst *b);
void output_bst_print_pages_in_order(OutBst *b);

#endif // !BINARY_SEARCH_TREE_H

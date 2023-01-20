#ifndef UTILS_H
#define UTILS_H

#include "hash-table.h"
#include "word-red-black-tree.h"
#include "page-red-black-tree.h"
#include <stdio.h>

void usage();
WRBT *parse_lookup_rbt(HashTable *table, const char *index_file_path, const char *pages_dir, int *total_pages);
char **split(const char *str, const char *delimiters, int *num_tokens);
PRBT *parse_graph_rbt(const char *graph_file_path);
char *read_whole_file(const char *file_path);
void str_to_lower(char **str);
HashTable *parse_stop_words(const char *stopwords_file_path);
void check_null_pointer(void *input, const char *message);
char **parse_searches(const char *search_file_path, int *num_searches);
char** intersection_pages_search_words(WRBT *lookup_rbt, char **search_words, int num_search_words, int *num_result_set);

#endif // !UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include "hash-table.h"
#include <stdio.h>

void usage();
void read_input(HashTable *table, const char *index_file_path, const char *graph_file_path);
char **split(const char *str, const char *delimiters, int *num_tokens);
char *read_whole_file(const char *file_path);
void str_to_lower(char **str);
HashTable *parse_stop_words(const char *stopwords_file_path);

#endif // !UTILS_H

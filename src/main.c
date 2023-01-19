#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../include/utils.h"
#include "../include/hash-table.h"
#include "../include/word-red-black-tree.h"
#include "../include/page-red-black-tree.h"

#define DELIMITERS "\n "

int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }

  const char *index_file_path = argv[1];
  char *index_copy = strdup(index_file_path);
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];
  int total_pages = 0;

  HashTable *table = parse_stop_words(stopwords_file_path);
  WRBT *lookup_rbt = parse_lookup_rbt(table, index_file_path, dirname(index_copy), &total_pages);
  PRBT *pages_rbt = parse_graph_rbt(graph_file_path);
  
  int num_searches = 0;
  char **searches = parse_searches("inputs/small/searches.txt", &num_searches);

  for (int i = 0; i < num_searches; i++) {
    int num_search_words = 0;
    char **search_words = split(searches[i], DELIMITERS, &num_search_words); 

    printf("\n======================================\n");
    printf("intersection pages for search: '%s'\n", searches[i]);
    intersection_pages_search_words(lookup_rbt, search_words, num_search_words);
    printf("\n======================================\n");
  }

  for (int i = 0; i < num_searches; i++) {
    free(searches[i]);
  }
  free(searches);

  // print_table(table);
  // word_rbt_print(lookup_rbt);
  // page_rbt_print(pages_rbt);

  free(index_copy);
  free_table(table);
  word_rbt_free(lookup_rbt);
  page_rbt_free(pages_rbt);
}


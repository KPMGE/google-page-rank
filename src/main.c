#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/hash-table.h"
#include "../include/word-red-black-tree.h"
#include "../include/page-red-black-tree.h"

int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }

  const char *index_file_path = argv[1];
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];

  int total_pages = 0;

  HashTable *table = parse_stop_words(stopwords_file_path);
  WRBT *lookup_rbt = parse_lookup_rbt(table, index_file_path, &total_pages);
  PRBT *pages_rbt = parse_graph_rbt(graph_file_path);

  char *query = "commercial";
  int amount_pages = 0;
  char **found_pages = word_rbt_search(lookup_rbt, query, &amount_pages);

  check_null_pointer(found_pages,  "No pages found for query: %s'\n");

  printf("found pages for %s\n", query);
  for (int i = 0; i < amount_pages; i++) {
    float page_rank = calculate_page_rank(pages_rbt, found_pages[i], total_pages);
    printf("page: %s, page_rank: %.3f\n", found_pages[i], page_rank);
  }
  free(found_pages);

  // print_table(table);
  // word_rbt_print(lookup_rbt);
  // page_rbt_print(pages_rbt);

  free_table(table);
  word_rbt_free(lookup_rbt);
  page_rbt_free(pages_rbt);
}


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

  HashTable *table = parse_stop_words(stopwords_file_path);
  WRBT *lookup_rbt = parse_lookup_rbt(table, index_file_path);
  PRBT *pages_rbt = parse_graph_rbt(graph_file_path);

  print_table(table);
  word_rbt_print(lookup_rbt);
  page_rbt_print(pages_rbt);

  free_table(table);
  word_rbt_free(lookup_rbt);
  page_rbt_free(pages_rbt);
}


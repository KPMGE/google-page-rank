#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/hash-table.h"
#include "../include/red-black-tree.h"
#include "../include/generic-red-black-tree.h"

int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }

  const char *index_file_path = argv[1];
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];

  parse_graph_rbt(graph_file_path);

  HashTable *table = parse_stop_words(stopwords_file_path);
  RBT *lookup_rbt = parse_lookup_rbt(table, index_file_path);

  print_table(table);
  RBT_print(lookup_rbt);

  RBT_free(lookup_rbt);
  free_table(table);
}


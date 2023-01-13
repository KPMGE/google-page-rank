#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/hash-table.h"

int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }

  const char *index_file_path = argv[1];
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];

  // read_input(index_file_path, graph_file_path);
  HashTable *hash = parse_stop_words(stopwords_file_path);
  print_table(hash);
}

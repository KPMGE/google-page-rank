#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../include/utils.h"
#include "../include/hash-table.h"
#include "../include/word-red-black-tree.h"
#include "../include/page-red-black-tree.h"

#define DELIMITERS "\n "

void display_word_set(char **set, int size_set) {
  printf("\nset: ");
  for (int i = 0; i < size_set; i++) {
    if (i % 5 == 0) {
      printf("\n");
    }
    printf("%s ", set[i]);
  }
  printf("\n");
}


void free_words_set(char **set, int size) {
  for (int i = 0; i <  size; i++) {
    if (set[i]) free(set[i]);
  }
  free(set);
}

int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }


  const char *index_file_path = argv[1];
  char *index_copy = strdup(index_file_path);
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];
  const char *directory_path = dirname(index_copy);

  int total_pages = 0;

  HashTable *table = parse_stop_words(stopwords_file_path);
  WRBT *lookup_rbt = parse_lookup_rbt(table, index_file_path, directory_path, &total_pages);
  PRBT *pages_rbt = parse_graph_rbt(graph_file_path, total_pages);
  
  int num_searches = 0;
  char **searches = parse_searches(directory_path, &num_searches);

  for (int i = 0; i < num_searches; i++) {
    int num_search_words = 0;
    char **search_words = split(searches[i], DELIMITERS, &num_search_words); 

    printf("\n======================================\n");
    printf("intersection pages for search: '%s'\n", searches[i]);
    int size_result_set = 0;
    char **result_set = intersection_pages_search_words(lookup_rbt, search_words, num_search_words, &size_result_set);

    printf("\nFINAL INTERSECTION SET: ");
    display_word_set(result_set, size_result_set);
    printf("\n");

    printf("\nPAGE RANKS FOR FINAL SET:\n");
    for (int i = 0; i < size_result_set; i++) {
      printf("page: %s, page_rank: %.6f\n", result_set[i], calculate_page_rank(pages_rbt, result_set[i], total_pages));
    }

    printf("\n======================================\n");

    free_words_set(search_words, num_search_words);
    free_words_set(result_set, size_result_set);
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


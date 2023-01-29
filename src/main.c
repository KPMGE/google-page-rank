#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../include/utils.h"
#include "../include/hash-table.h"
#include "../include/word-red-black-tree.h"
#include "../include/page-red-black-tree.h"
#include "../include/output-binary-search-tree.h"

#define DELIMITERS "\n "

int main (int argc, char *argv[]) {
  if (argc < 2) {
    usage();
  }

  char *directory_path = argv[1];

  char *index_file_path = alloc_string(line_size(directory_path, "index.txt"));
  sprintf(index_file_path, "%sindex.txt", directory_path);

  char *stopwords_file_path = alloc_string(line_size(directory_path, "stopwords.txt"));;
  sprintf(stopwords_file_path, "%sstopwords.txt", directory_path);

  char *graph_file_path = alloc_string(line_size(directory_path, "graph.txt"));;
  sprintf(graph_file_path, "%sgraph.txt", directory_path);

  int total_pages = 0;

  HashTable *table = parse_stop_words(stopwords_file_path);
  WRBT *lookup_rbt = parse_lookup_rbt(table, index_file_path, directory_path, &total_pages);
  PRBT *pages_rbt = parse_graph_rbt(graph_file_path, total_pages);

  int num_searches = 0;
  char **searches = parse_searches(directory_path, &num_searches);

  for (int i = 0; i < num_searches; i++) {
    int num_search_words = 0, size_result_set = 0;

    char **search_words = split(searches[i], DELIMITERS, &num_search_words); 
    char **result_set = intersection_pages_search_words(lookup_rbt, search_words, num_search_words, &size_result_set);

    OutBst *output_bst = NULL;
    for (int i = 0; i < size_result_set; i++) {
      if (!output_bst) {
        output_bst = output_bst_new(result_set[i], calculate_page_rank(pages_rbt, result_set[i]));
        continue;
      }
      output_bst = output_bst_add(output_bst, result_set[i], calculate_page_rank(pages_rbt, result_set[i]));
    }

    printf("search:%s\n", searches[i]);
    printf("pages:");
    output_bst_print_pages_in_descending_order(output_bst);
    printf("\npr:");
    output_bst_print_page_rank_in_descending_order(output_bst);
    printf("\n");
    
    output_bst_free(output_bst);
    free_words_set(search_words, num_search_words);
    free_words_set(result_set, size_result_set);
  }

  for (int i = 0; i < num_searches; i++) {
    free(searches[i]);
  }
  free(searches);

  free(index_file_path);
  free(graph_file_path);
  free(stopwords_file_path);
  ht_free(table);
  word_rbt_free(lookup_rbt);
  page_rbt_free(pages_rbt);
}

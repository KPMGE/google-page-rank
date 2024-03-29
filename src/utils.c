#include "../include/utils.h"
#include "../include/page-red-black-tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 400
#define DELIMITERS " \n"

void str_to_lower(char **str) {
  char *str2 = *str;
  for (int i = 0; str2[i] != '\0'; i++) {
    str2[i] = tolower(str2[i]);
  }
}

void check_null_pointer(void *input, const char *message) {
  if (!input) {
    fprintf(stderr, "could not open file: %s\n", message);
    exit(1);
  }
}

void check_read_file(FILE *file, const char *file_path) {
  if (!file) {
    fprintf(stderr, "could not open file: %s\n", file_path);
    exit(1);
  }
}

void usage() {
  fprintf(stderr, "Usage: trab3 <entry folder>\n");
  exit(1);
}

void free_words_set(char **set, int size) {
  for (int i = 0; i <  size; i++) {
    if (set[i]) free(set[i]);
  }
  free(set);
}

WRBT *parse_lookup_rbt(HashTable *table, const char *index_file_path, const char *pages_dir, int *total_pages) {
  FILE *index_file = fopen(index_file_path, "r");
  check_read_file(index_file, index_file_path);

  WRBT *rb_tree = word_rbt_init();
  char file_name[MAX_LINE_SIZE];

  while (fscanf(index_file, "%s", file_name) == 1) {
    *total_pages += 1;

    const size_t buffer_size = strlen(pages_dir) + strlen("/pages/") + strlen(file_name);
    char file_path[buffer_size];
    sprintf(file_path, "%s/pages/%s", pages_dir, file_name);

    char *file_content = read_whole_file(file_path);

    int amount_words = 0;
    char **words = split(file_content, DELIMITERS, &amount_words);

    free(file_content);

    for (int i = 0; i < amount_words; i++) {
      bool is_stop_word = ht_search(table, words[i]);
      if (!is_stop_word) {
        word_rbt_insert(rb_tree, words[i]);
        word_rbt_add_page(rb_tree, words[i], file_name);
      }

      free(words[i]);
    }

    free(words);
  }

  fclose(index_file);
  return rb_tree;
}

char *read_whole_file(const char *file_path) {
  FILE *f = fopen(file_path, "rb");
  check_read_file(f, file_path);

  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;

  return string;
}

char **split(const char *str, const char *delimiters, int *num_tokens) {
  char **tokens = malloc(sizeof(char *));
  char *copy = strdup(str);
  int tokens_allocated = 1;
  int tokens_used = 0;
  char *token, *rest = copy;
  while ((token = strtok_r(rest, delimiters, &rest))) {
    if (tokens_used == tokens_allocated) {
      tokens_allocated *= 2;
      tokens = realloc(tokens, tokens_allocated * sizeof(char *));
    }
    tokens[tokens_used++] = strdup(token);
  }
  if (tokens_used == 0) {
    free(tokens);
    tokens = NULL;
  } else {
    tokens = realloc(tokens, tokens_used * sizeof(char *));
  }
  *num_tokens = tokens_used;
  free(copy);
  return tokens;
}

HashTable *parse_stop_words(const char *stopwords_file_path) {
  FILE *stopwords_file = fopen(stopwords_file_path, "r");
  check_read_file(stopwords_file, stopwords_file_path);

  char stopword[MAX_LINE_SIZE];
  HashTable *table = ht_new(CAPACITY);

  while (fscanf(stopwords_file, "%s\n", stopword) == 1) {
    ht_insert(table, stopword);
  }

  fclose(stopwords_file);

  return table;
}

PRBT* parse_graph_rbt(const char *graph_file_path, int total_pages) {
  FILE *graph_file = fopen(graph_file_path, "r");
  check_read_file(graph_file, graph_file_path);

  char current_file[MAX_LINE_SIZE];
  int amount_links = 0;
  PRBT *rbt = page_rbt_init(total_pages);

  char links_buffer[MAX_LINE_SIZE * 10];

  while (fscanf(graph_file, "%s %d %[^\n]", current_file, &amount_links, links_buffer) == 3) {
    // insert current file into the pages rbt
    page_rbt_insert(rbt, current_file);
    // split links_buffer into tokens(words)
    char **links = split(links_buffer, DELIMITERS, &amount_links);
    // add links to rbt
    page_rbt_add_links(rbt, current_file, amount_links, links);

    // free links
    for (int i = 0; i < amount_links; i++) {
      free(links[i]);
    }
    free(links);
  }

  fclose(graph_file);

  return rbt;
}

static int count_file_lines(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  check_read_file(file, file_path);

  int count = 0;
  char buffer[MAX_LINE_SIZE];
  while (fscanf(file, "%[^\n]\n", buffer) == 1) {
    count++;
  }

  fclose(file);

  return count;
}

char **parse_searches(const char *search_dir_path, int *num_searches) {
  char search_file_path[MAX_LINE_SIZE];
  sprintf(search_file_path, "%s/searches.txt", search_dir_path);

  FILE *search_file = fopen(search_file_path, "r");
  check_read_file(search_file, search_file_path);

  int search_file_num_lines = count_file_lines(search_file_path);
  *num_searches = search_file_num_lines;
  char **searches = malloc(sizeof(char *) * search_file_num_lines);

  int k = 0;
  char buffer[MAX_LINE_SIZE];
  while (fscanf(search_file, "%[^\n]\n", buffer) == 1) {
    searches[k++] = strdup(buffer);
  }

  fclose(search_file);
  return searches;
}

// the funcion 'display_word_set' is not used, though it can be helpful for debugging purposes, 
// so that we just make the compiler shut up about it not giving us a warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
  static void display_word_set(char **set, int size_set) {
    printf("\nset: ");
    for (int i = 0; i < size_set; i++) {
      if (i % 5 == 0) {
        printf("\n");
      }
      printf("%s ", set[i]);
    }
    printf("\n");
  }
#pragma GCC diagnostic pop

static char **intersection_word_sets(
  char **first_set,
  int size_first_set,
  char **second_set,
  int size_second_set, 
  int *size_set
) {
  // hash-table for quickly looking up elements 
  HashTable *table = ht_new(CAPACITY);

  // insert all elements from the first set into the hash-table
  for (int i = 0; i < size_first_set; i++) {
    ht_insert(table, first_set[i]);
  }

  int k = 0;
  char **result_set = malloc(sizeof(char *));

  for (int i = 0; i < size_second_set; i++) {
    char *current_page = second_set[i];

    const bool current_page_intersects = ht_search(table, current_page);
    // if current for the second term appears on the hash-table, we know we needa add it to the intersection set
    if (current_page_intersects) {
      result_set = realloc(result_set, ++k * sizeof(char *));
      result_set[k - 1] = strdup(current_page);
    }
  }

  *size_set = k;
  ht_free(table);

  return result_set;
}

// find the intersection given a list of search words
char** intersection_pages_search_words(WRBT *lookup_rbt, char **search_words, int num_search_words, int *num_result_set) {
  // NOTE: from basic set theory, we know that: (A ∩ B) ∩ C == A ∩ (B ∩ C) == B ∩ (A ∩ C)
  // so that, we can always take pairs of sets, calculate their intersection and then use it
  // to calculate the intersection with next ones, the final result shall be the intersection between
  // all sets.
 
  // find pages for the first search word
  int num_pages = 0;
  // get pages for the first word, this is useful cuz if the search has just one 
  // item the whole for loop will be skipped later
  char **pages = word_rbt_search(lookup_rbt, search_words[0], &num_pages);

  char **previous_set = NULL;

  // stores the first set into memory
  int size_result_set = num_pages;
  char **result_set = malloc(sizeof(char *) * num_pages);
  for (int i = 0; i < num_pages; i++) {
    result_set[i] = strdup(pages[i]);
  }
  free(pages);

  // iteratively calculates the final result_set, updating the 'result_set' reference
  for (int i = 1; i < num_search_words; i++) {
    // find intersection pages between the current word and the next one and set the 'pages' buffer to the result
    int next_num_pages = 0;
    char **next_word_pages = word_rbt_search(lookup_rbt, search_words[i], &next_num_pages);

    // saves current set
    previous_set = result_set;
    int previous_set_size = size_result_set;

    // calculate the intersection between the current set and the next one
    result_set = intersection_word_sets(result_set, size_result_set, next_word_pages, next_num_pages, &size_result_set);

    // free unnecessary previous_set
    free_words_set(previous_set, previous_set_size);
    free(next_word_pages);
  }

  *num_result_set = size_result_set;
  return result_set;
} 


int line_size(const char *dir_path, const char *file_name) {
  return strlen(dir_path) + strlen(file_name);
}

char *alloc_string(size_t size) {
  return malloc(sizeof(char) * size + 1);
}

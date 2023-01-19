#include "../include/utils.h"
#include "../include/page-red-black-tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 300
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
  fprintf(stderr, "Usage: trab3 <index_file> <stopwords_file> <graph_file>\n");
  exit(1);
}

WRBT *parse_lookup_rbt(HashTable *table, const char *index_file_path, const char *pages_dir, int *total_pages) {
  FILE *index_file = fopen(index_file_path, "r");
  check_read_file(index_file, index_file_path);

  WRBT *rb_tree = word_rbt_init();
  char file_name[MAX_LINE_SIZE];

  while (fscanf(index_file, "%s", file_name) == 1) {
    *total_pages += 1;

    char file_path[MAX_LINE_SIZE];
    sprintf(file_path, "%s/pages/%s", pages_dir, file_name);
    // printf("file_path: %s\n", file_path);
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
  HashTable *table = create_table(CAPACITY);

  while (fscanf(stopwords_file, "%s\n", stopword) == 1) {
    ht_insert(table, stopword);
  }

  fclose(stopwords_file);

  return table;
}

// void parse_graph_rbt(const char *graph_file_path) {
//   FILE *graph_file = fopen(graph_file_path, "r");
//   check_read_file(graph_file, graph_file_path);

//   char current_file[MAX_LINE_SIZE];
//   int amount_links = 0;
//   WRBT *rbt = word_rbt_init();

//   while (fscanf(graph_file, "%s %d ", current_file, &amount_links) == 2) {
//     word_rbt_insert(rbt, current_file);

//     char link[MAX_LINE_SIZE];
//     for (int i = 0; i < amount_links; i++) {
//       fscanf(graph_file, "%s", link);
//       word_rbt_add_page(rbt, current_file, link);
//     }
//   }

//   fclose(graph_file);
//   word_rbt_print(rbt);
//   word_rbt_free(rbt);
// }

PRBT* parse_graph_rbt(const char *graph_file_path) {
  FILE *graph_file = fopen(graph_file_path, "r");
  check_read_file(graph_file, graph_file_path);

  char current_file[MAX_LINE_SIZE];
  int amount_links = 0;
  PRBT *rbt = page_rbt_init();

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

char **parse_searches(const char *search_file_path, int *num_searches) {
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

static char **intersection_word_sets(
  char **first_set,
  int size_first_set,
  char **second_set,
  int size_second_set, 
  int *size_set
) {
  // hash-table for quickly looking up elements 
  HashTable *table = create_table(CAPACITY);

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
  free_table(table);

  return result_set;
}

// find the intersection given a list of search words
char** intersection_pages_search_words(WRBT *lookup_rbt, char **search_words, int num_search_words) {
  // NOTE: (A ∩ B) ∩ C == A ∩ (B ∩ C) == B ∩ (A ∩ C)
 
  // find pages for the first search word
  printf("pages for each search word sets: \n");

  int num_pages = 0;
  char **pages = word_rbt_search(lookup_rbt, search_words[0], &num_pages);
  printf("\nword: %s\n", search_words[0]);
  display_word_set(pages, num_pages);

  int size_result_set = num_pages;

  for (int i = 1; i < num_search_words; i++) {
    printf("\nword: %s\n", search_words[i]);
    // find intersection pages between the current word and the next one and set the 'pages' buffer to the result
    int next_num_pages = 0;
    char **next_word_pages = word_rbt_search(lookup_rbt, search_words[i], &next_num_pages);

    display_word_set(next_word_pages, next_num_pages);

    pages = intersection_word_sets(pages, size_result_set, next_word_pages, next_num_pages, &size_result_set);
  }

  printf("\nFINAL INTERSECTION SET: ");
  display_word_set(pages, size_result_set);
  printf("\n");

  return pages;
} 

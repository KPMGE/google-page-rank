#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/utils.h"
#include "../include/red-black-tree.h"

#define MAX_LINE_SIZE 400
#define DELIMITERS " \n"

void str_to_lower(char **str) {
  char *str2 = *str;
  for (int i = 0; str2[i] != '\0'; i++) {
    str2[i] = tolower(str2[i]);
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

RBT *parse_lookup_rbt(HashTable *table, const char *index_file_path) {
  FILE *index_file = fopen(index_file_path, "r");
  check_read_file(index_file, index_file_path);

  RBT *rb_tree = NULL;
  char file_name[MAX_LINE_SIZE];

  while (fscanf(index_file, "%s", file_name) == 1) {
    char file_path[MAX_LINE_SIZE];
    sprintf(file_path, "inputs/small/pages/%s", file_name);
    printf("file_path: %s\n", file_path);
    char *file_content = read_whole_file(file_path);

    int amount_words = 0;
    char **words = split(file_content, DELIMITERS, &amount_words);

    free(file_content);
    
    for (int i = 0; i < amount_words; i++) {
      bool is_stop_word = ht_search(table, words[i]);

      if (!is_stop_word) {
        rb_tree = RBT_insert(rb_tree, words[i]);
        RBT *node = RBT_search(rb_tree, words[i]);
        RBT_add_page(node, file_name);
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

char** split(const char* str, const char* delimiters, int* num_tokens) {
  char** tokens = malloc(sizeof(char *));
  char* copy = strdup(str);
  int tokens_allocated = 1;
  int tokens_used = 0;
  char* token, *rest = copy;
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

void parse_graph_rbt(const char *graph_file_path) {
  FILE *graph_file = fopen(graph_file_path, "r");
  check_read_file(graph_file, graph_file_path);

  char current_file[MAX_LINE_SIZE];
  int amount_links = 0;
  RBT *rbt = NULL;

  while (fscanf(graph_file, "%s %d", current_file, &amount_links) == 2) {
    rbt = RBT_insert(rbt, current_file);

    char link[MAX_LINE_SIZE];
    for (int i = 0; i < amount_links; i++) {
      fscanf(graph_file, "%s", link);
      RBT *node = RBT_search(rbt, current_file);
      RBT_add_page(node, link);
    }
  }

  RBT_print(rbt);
  RBT_free(rbt);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

#define MAX_LINE_SIZE 250

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

void read_input(const char *index_file_path, const char *stopwords_file_path, const char *graph_file_path) {
  FILE *index_file = fopen(index_file_path, "r");
  FILE *stopwords_file = fopen(stopwords_file_path, "r");
  FILE *graph_file = fopen(graph_file_path, "r");
  check_read_file(index_file, index_file_path);
  check_read_file(stopwords_file, stopwords_file_path);
  check_read_file(graph_file, graph_file_path);

  printf("all files: \n");
  char file_name[MAX_LINE_SIZE];
  while (fscanf(index_file, "%s", file_name) == 1) {
    printf("file: %s\n", file_name);
  }

  printf("lines: \n");
  char line[MAX_LINE_SIZE];
  while (fscanf(graph_file, "%s", line) == 1) {
    printf("line: %s\n", line);
  }

  printf("all stopwords: \n");
  char stopword[MAX_LINE_SIZE];
  while (fscanf(stopwords_file, "%s", stopword) == 1) {
    printf("stopword: %s\n", stopword);
  }
}

char *read_whole_file(const char *file_path) {
  FILE *f = fopen(file_path, "rb");
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
    tokens = realloc(tokens, tokens_used * sizeof(char*));
  }
  *num_tokens = tokens_used;
  free(copy);
  return tokens;
}

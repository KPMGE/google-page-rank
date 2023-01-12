#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"


int main (int argc, char *argv[]) {
  if (argc < 3) {
    usage();
  }

  const char *index_file_path = argv[1];
  const char *stopwords_file_path = argv[2];
  const char *graph_file_path = argv[3];

  read_input(index_file_path, graph_file_path);
  parse_stop_words(stopwords_file_path);

  // const char file_to_read[200] = "inputs/small/pages/305.txt";
  // char *content = read_whole_file(file_to_read);

  // int amount_tokens =  0;
  // char **tokens = split(content, " \n", &amount_tokens);
  // for (int i = 0; i < amount_tokens; i++) {
  //   printf("token: %s\n", tokens[i]);
  // }

  // free(content);
  // for (int i = 0; i < amount_tokens; i++) {
  //   free(tokens[i]);
  // }
  // free(tokens);
}

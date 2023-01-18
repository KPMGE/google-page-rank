#include "../include/page-red-black-tree.h"
#include "../include/generic-red-black-tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INCOMING_LINKS 1000
#define DAMPING_FACTOR 0.8
#define EPSILON 0.0001

typedef struct {
  float page_rank; 
  int num_incoming_links;
  char *incoming_links[MAX_INCOMING_LINKS];
  int num_outgoing_links;
} PageData;

static void *alloc_initial_page_data() {
  PageData *d = malloc(sizeof(PageData));

  d->num_incoming_links = 0;
  d->num_outgoing_links = 0;
  d->page_rank = 1.0;

  return d;
}

static void print_data(void *data) {
  PageData *d = data;
  printf("\nnum_outgoing_links: %d", d->num_outgoing_links);
  printf("\nnum_incoming_links: %d\n", d->num_incoming_links);

  printf("incoming_links: ");
  for (int i = 0; i < d->num_incoming_links; i++) {
    if (i % 7 == 0) {
      printf("\n");
    }
    printf("%s ", d->incoming_links[i]);
  }
  printf("\n");
}

static void free_data(void *data) {
  PageData *d = data;
  if (!d) return;
  for (int i = 0; i < d->num_incoming_links; i++) {
    free(d->incoming_links[i]);
  } 
  // for now, our array is static, so we don't needa free it yet. 
  // but we must do that later
  free(d);
}

struct page_rbt {
  GRBT *rbt;
};

PRBT* page_rbt_init() {
  PRBT *new_rbt = malloc(sizeof(PRBT));
  new_rbt->rbt = NULL;
  return new_rbt;
}

void page_rbt_insert(PRBT *h, char *page_name) {
  h->rbt = grbt_insert(h->rbt, page_name, alloc_initial_page_data);
}

void page_rbt_print(PRBT *h) {
  grbt_print(h->rbt, print_data);
}

void page_rbt_add_links(PRBT *h, char *page_name, int num_outgoing_links, char **links) {
  // find page node in the generic rbt
  GRBT *node = grbt_search(h->rbt, page_name);
  if (!node) return;

  // pull the data out of such a node
  PageData *data = grbt_data(node);
  if (!data) return;

  // adjust the number of outgoing links for the found node
  data->num_outgoing_links = num_outgoing_links;

  // insert the links in the rbt, just in case
  for (int i = 0; i < num_outgoing_links; i++) {
    // printf("inserting '%s' on the tree\n", links[i]);
    page_rbt_insert(h, links[i]);
  }

  // update the incoming_links for each link
  for (int i = 0; i < num_outgoing_links; i++) {
    GRBT *node = grbt_search(h->rbt, links[i]);
    PageData *data = grbt_data(node);
    int p = data->num_incoming_links++;
    // printf("store %s in the incoming_links array of %s\n", page_name, links[i]);
    data->incoming_links[p] = strdup(page_name);
  }
}

void page_rbt_free(PRBT *h) {
  grbt_free(h->rbt, free_data);
  free(h);
}

float calculate_page_rank(PRBT *p, char *page_name, int num_pages) {
  GRBT *node = grbt_search(p->rbt, page_name); 
  if (!node) return -1; // Return -1 if page is not found

  //Calculate the PageRank through iterations
  PageData *page = grbt_data(node);
  page->page_rank = 1.0 / num_pages;

  float prev_page_rank = -1;
  float new_rank = page->page_rank;

  while((new_rank - prev_page_rank) > EPSILON) {
    prev_page_rank = new_rank;
    new_rank = (1 - DAMPING_FACTOR);
    for (int i = 0; i < page->num_outgoing_links; i++) {
      GRBT *incoming_link_node = grbt_search(p->rbt, page->incoming_links[i]);
      PageData *incoming_link = grbt_data(node);

      new_rank += DAMPING_FACTOR * (incoming_link->page_rank / incoming_link->num_outgoing_links);
    }
  }
  page->page_rank = new_rank;

  return new_rank;
}

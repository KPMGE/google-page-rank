#include "../include/page-red-black-tree.h"
#include "../include/generic-red-black-tree.h"
#include "../include/linked-list.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define DAMPING_FACTOR 0.85
#define EPSILON 10e-10

typedef struct {
  double page_rank; 
  int num_incoming_links;
  LinkedList *incoming_links;
  int num_outgoing_links;
  int total_pages;
} PageData;

static void *alloc_initial_page_data() {
  PageData *d = malloc(sizeof(PageData));

  d->num_incoming_links = 0;
  d->num_outgoing_links = 0;
  d->incoming_links = linked_list_init();
  d->page_rank = 1;

  return d;
}

static void free_data(void *data) {
  PageData *d = data;
  if (!d) return;
  linked_list_free(d->incoming_links);
  free(d);
}

struct page_rbt {
  GRBT *rbt;
  int total_pages;
};

PRBT* page_rbt_init(int total_pages) {
  PRBT *new_rbt = malloc(sizeof(PRBT));
  new_rbt->rbt = NULL;
  new_rbt->total_pages = total_pages;
  return new_rbt;
}

void page_rbt_insert(PRBT *h, char *page_name) {
  h->rbt = grbt_insert(h->rbt, page_name, alloc_initial_page_data);
  GRBT *node = grbt_search(h->rbt, page_name);
  PageData *data = grbt_data(node);
  data->page_rank = 1.0 / h->total_pages;
  data->total_pages = h->total_pages;
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
    page_rbt_insert(h, links[i]);
  }

  // update the incoming_links for each link
  for (int i = 0; i < num_outgoing_links; i++) {
    GRBT *node = grbt_search(h->rbt, links[i]);
    PageData *data = grbt_data(node);
    data->num_incoming_links++;
    linked_list_insert(data->incoming_links, page_name);
  }
}

void page_rbt_free(PRBT *h) {
  grbt_free(h->rbt, free_data);
  free(h);
}

// update the page rank of each page for the power-method
static void update_page_ranks(GRBT *root, void *data) {
  // convert void* into a PageData
  PageData *page = data;
  if (!data) return;

  // initilize constant for initial new page-rank
  double new_page_rank = (1.0 - DAMPING_FACTOR) / page->total_pages;

  // if |Out(i)| == 0
  if (page->num_outgoing_links == 0) {
    new_page_rank += DAMPING_FACTOR * page->page_rank;
  }

  // sum up page ranks from each incoming page 
  for (int i = 0; i < page->num_incoming_links; i++) {
    GRBT *node = grbt_search(root, linked_list_at(page->incoming_links, i)); 

    if (!node) continue; 
    PageData *page_inc = grbt_data(node);
    if (page_inc->num_outgoing_links == 0) continue;

    new_page_rank += DAMPING_FACTOR * (page_inc->page_rank / page_inc->num_outgoing_links);
  }

  // update page-rank of the current page
  page->page_rank = new_page_rank;
}

static double get_page_rank(GRBT *h, char *page_name) {
  GRBT *node = grbt_search(h, page_name); 
  if (!node) return -1; // Return -1 if page is not found
  PageData *page = grbt_data(node);
  return page->page_rank;
}

double calculate_page_rank(PRBT *p, char *page_name) {
  double error = INFINITY;
  double new_rank = 0;

  while (error > EPSILON) {
    // get the page-rank of the page before updating.
    double prev_rank = get_page_rank(p->rbt, page_name);

    // traverse rbt, updating the links
    traverse_tree(p->rbt, p->rbt, update_page_ranks);

    // get the page-rank of the page after updating
    new_rank = get_page_rank(p->rbt, page_name);

    // update error
    error = fabs((1.0 / p->total_pages) * (new_rank - prev_rank));
  }

  return new_rank;
}

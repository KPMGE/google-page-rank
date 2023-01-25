#include "../include/page-red-black-tree.h"
#include "../include/generic-red-black-tree.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// the max number of incoming_links to a especific node
#define MAX_INCOMING_LINKS 1000

#define DAMPING_FACTOR 0.85
#define EPSILON 10e-10

// NOTE: i'm using those global variables in here, cuz currently, i needa use the 'grbt_search' function to 
// get a node given its name, but as soon as a linked-list is implemented, there is no need for that anymore!
GRBT *rbt_static = NULL;
// i need the total amount of pages too, probably there is a better way to store that value, but it depends on 
// the approach used to refactor the code
int total_pages_static = 0;

typedef struct {
  double page_rank; 
  int num_incoming_links;
  char *incoming_links[MAX_INCOMING_LINKS];
  int num_outgoing_links;
} PageData;

static void *alloc_initial_page_data() {
  PageData *d = malloc(sizeof(PageData));

  d->num_incoming_links = 0;
  d->num_outgoing_links = 0;
  d->page_rank = 1;

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
    if (p > MAX_INCOMING_LINKS) exit(1);
    // printf("store %s in the incoming_links array of %s\n", page_name, links[i]);
    data->incoming_links[p] = strdup(page_name);
  }
}

void page_rbt_free(PRBT *h) {
  grbt_free(h->rbt, free_data);
  free(h);
}

// update the page rank of each page for the power-method
static void update_page_ranks(void *data) {
  // convert void* into a PageData
  PageData *page = data;
  if (!data) return;

  // initilize constant for initial new page-rank
  double new_page_rank = (1.0 - DAMPING_FACTOR) / total_pages_static;

  // if |Out(i)| == 0
  if (page->num_outgoing_links == 0) {
    new_page_rank += DAMPING_FACTOR * page->page_rank;
  }

  // sum up page ranks from each incoming page 
  for (int i = 0; i < page->num_incoming_links; i++) {
    GRBT *node = grbt_search(rbt_static, page->incoming_links[i]); 

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

double calculate_page_rank(PRBT *p, char *page_name, int total_pages) {
  // update the global variable with the rbt from the page-rbt. this is awful and ought be refactored!!
  rbt_static = p->rbt;
  // update the global variable with the total_pages. this is awful and ought be refactored!!
  total_pages_static = total_pages;

  // error starts as infinity
  double error = INFINITY;
  double new_rank = 0;

  while (error > EPSILON) {
    // get the page-rank of the page before updating.
    // NOTE: as i said, as currently the implementation's been made with a char **, i needa 
    // keep searching the rbt to get the nodes, which is bad
    double prev_rank = get_page_rank(p->rbt, page_name);

    traverse_tree(p->rbt, update_page_ranks);

    // get the page-rank of the page after updating
    // NOTE: as i said, as currently the implementation's been made with a char **, i needa 
    // keep searching the rbt to get the nodes, which is bad
    new_rank = get_page_rank(p->rbt, page_name);

    // update error
    error = fabs((1.0 / p->total_pages) * (new_rank - prev_rank));
  }

  return new_rank;
}

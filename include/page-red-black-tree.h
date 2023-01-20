#ifndef PAGE_RBT_H
#define PAGE_RBT_H

typedef struct page_rbt PRBT;

PRBT* page_rbt_init();
void page_rbt_insert(PRBT *h, char *page_name);
void page_rbt_print(PRBT *h);
void page_rbt_add_links(PRBT *h, char *page_name, int num_outgoing_links, char **links);
void page_rbt_free(PRBT *h);
float calculate_page_rank(PRBT *p, char *page_name, int num_total_pages);

#endif // !PAGE_RBT_H

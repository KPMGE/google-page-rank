#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct linked_list LinkedList;
 
LinkedList *linked_list_init();
void linked_list_insert(LinkedList *list, char *page);
void linked_list_print(LinkedList *list);
bool linked_list_has_page(LinkedList *list, const char *page);
void linked_list_free(LinkedList *list);
char **linked_list_to_pages(LinkedList *list);
int linked_list_amount_nodes(LinkedList *list);
char *linked_list_at(LinkedList *list, int pos);
LinkedList **linked_list_vec_new(size_t size);

#endif

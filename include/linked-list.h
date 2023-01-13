#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct linked_list LinkedList;
 
LinkedList *linked_list_init();
void linked_list_insert(LinkedList *list, char *page);
void linked_list_print(LinkedList *list);
bool linked_list_has_page(LinkedList *list, const char *page);
void linked_list_free(LinkedList *list);

#endif

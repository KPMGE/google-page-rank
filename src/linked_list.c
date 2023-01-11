#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/linked_list.h"

typedef struct cell Cell;

struct cell {
    char *page;
    Cell *next;
};

typedef struct linked_list {
    Cell *first;
    Cell *last;
} LinkedList;

LinkedList *linked_list_init(){
    LinkedList *list = malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    return list;
}

void linked_list_insert(LinkedList *list, char *page){
    Cell *new = malloc(sizeof(Cell));
    new->page = strdup(page);
    new->next = NULL;

    if(list->first == NULL){
        list->first = list->last = new;
        return;
    }

    list->last->next = new;
    list->last = new;
}

void linked_list_print(LinkedList *list){
    Cell *p;
    for(p = list->first; p != NULL; p = p->next){
        printf("%s ", p->page);
    }
}

void linked_list_free(LinkedList *list){
    Cell *p;
    Cell *c;

    for(p = list->first; p != NULL; p = p->next){
        c = p->next;
        free(p->page);
        free(p);
        p = c;
    }
    free(list); 
}


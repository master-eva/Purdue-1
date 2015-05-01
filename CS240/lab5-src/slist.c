
#include <stdio.h>
#include <stdlib.h>

#include "slist.h"

void
sllist_init(SLList * list)
{
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void
sllist_add_end( SLList *list, int value )  {
	SLEntry *test = list -> head;
	SLEntry *last = NULL;
	while( test != NULL ){
		last=test;
		test=test->next;
	}

	SLEntry *new=(SLEntry*)malloc(sizeof(SLEntry));
	new->value = value;
	new->next = NULL;
	
	if( last==NULL){
		list -> head = new; 
	} else {
		last->next=new;
	}
}


// Remove first occurrence of value in the list. It returns 1 if value is found or 0 otherwise.
int sllist_remove(SLList *list, int value) {
	SLEntry * e = list -> head;
	SLEntry * last = NULL;
		while ( e != NULL) {
			if (e -> value == value) {
					if (last == NULL) {
						list -> head = e -> next;
					} else {
						last -> next = e -> next;
					}
					return 1;
			}
			last = e;
			e = e -> next;
		}
	return 0;
}


// Removes from the list and frees the nodes of all the items that are min <= value <= max
void sllist_remove_interval(SLList *list, int min, int max) {
	SLEntry * e = list -> head;
	while (e != NULL) {
		if (e -> value <= max && e -> value >= min)
			sllist_remove(list,e -> value);
		e = e -> next;
	}
}


// It allocates and returns a new list with the intersection of the two lists, that is the 
// list of all the items that are common to the two lists. The items in the intersection are
// unique. 
SLList *
sllist_intersection(SLList *a, SLList *b) {
	SLList * list = (SLList *)malloc(sizeof(SLList));
	sllist_init(list);

	if (a -> head == NULL || b -> head == NULL) return list;


	SLEntry * e = a -> head;
	while (e != NULL) {
		int n = e -> value;
		if (sllist_remove(b,n)) {
				if( list -> head == NULL ){
					list -> head = (SLEntry*)malloc(sizeof(SLEntry));
					list -> head -> next = NULL;
					list -> head -> value = n;
				} else {
					SLEntry *new = (SLEntry*)malloc(sizeof(SLEntry));
					new -> value = n;
					new -> next = list -> head;
					list -> head = new;
				}
		}
		e = e -> next;
	}

	return list;
}

void
sllist_print(SLList *list)
{
	// Move to the end
	SLEntry * e = list->head;

	printf("--- List ---\n");
	while (e!= NULL) {
		printf("val=%d\n", e->value);
		e = e->next;
	}
}



#include "SLList.h"
#include <stdio.h>
#include <string.h>

// Create a single-linked list
SLList * sllist_create() {
	SLList * list = (SLList *) malloc(sizeof(SLList));
	if (list == NULL) return NULL;
	list -> head = NULL;
	return list;
}

// Insert an element in the linked list. If name already exists
// substitute value. Otherwise insert entry.
// Return 0 if name is not there.
int sllist_insert(SLList *sllist,  char * name, char * value) {
	SLListEntry * e = sllist->head;

	while (e != NULL && strcmp(e->name,name) != 0) {
		e = e->next;
	}

	if (e != NULL) {
		free(e -> value);
		e -> value = strdup(value);
		return 1;
	}

	SLListEntry * new = (SLListEntry *) malloc(sizeof(SLListEntry));
	new -> name = strdup(name);
	new -> value = strdup(value);
	new -> next = sllist->head;
	sllist -> head = new;
	return 0;
}

// Remove entry in single linked list. 
int sllist_remove(SLList *sllist, char * name) {
	SLListEntry * e = sllist->head;
	SLListEntry * last = NULL;
	
	while (e!=NULL && strcmp(e->name,name)) {
		last = e;
		e = e -> next;
	}

	if (e==NULL) {
		// Entry not found
		return 0;
	}

	// Entry found. Two cases
	if (last == NULL) {
		// Entry is first one in the list.
		sllist -> head = e -> next;
	}
	else {
		// Entry is in the middle of the list.
		last -> next = e -> next;
	}

	// free name and value
	free(e -> name);
	free(e -> value);
}

// Return last entry in pname, pvalue. Return 0 if list is empty or 1 otherwise.
int sllist_last(SLList *sllist, char ** pname, char **pvalue) {

	SLListEntry * e = sllist->head;
	SLListEntry * last = NULL;
	
	if (e==NULL) {
		// Empty list
		return 0;
	}
	
	while (e!=NULL) {
		last = e;
		e = e -> next;
	}

	// Last entry is in prev
	*pname = last -> name;
	*pvalue = last -> value;

	return 1;

}

// Print linked list in the format:
// 0: name=name0 value=value0
// 1: name=name1 value=cvalue1
void sllist_print(SLList * sllist) { 
	SLListEntry * e = sllist->head;
	int i = 0;
	while (e != NULL) {
		printf("%d: name=%s value=%s\n", i++, e->name, e->value);
		e = e->next;
	}
}

// Reverse the list. Use the existing list and nodes.
// Do not allocate a new SLLIST nor use an array.
void sllist_reverse(SLList * sllist) {
 	SLListEntry * e = sllist->head;
	SLListEntry * newHead = NULL;
	SLListEntry * next;
	
	while (e!=NULL) {
		// Save next pointer
		next = e->next;

		// Add e to new Head
		e->next = newHead;
		newHead = e;

		// Move to next node in the list
		e = next;
	}

	// Set new head
	sllist->head = newHead;
}


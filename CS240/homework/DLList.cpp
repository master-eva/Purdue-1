#include <string.h>
#include <stdlib.h>

#include "DLList.h"

// Constructor
DLList::DLList() {
	// Create Sentinel node. This node does not store any data
	// but simplifies the list implementation.
	head = new DLListEntry();
	
	head -> next = head;
	head -> previous = head;
}

// Adds new name, value pair at the end of the list.
// If name already exists substitute value in that entry and return 1. Otherwise return 0
int DLList::add(char *  name, char * value) {
    DLListEntry * e;
	e = head -> next;
	// Try to find if entry is already there
	while (e != head) {
		if (!strcmp(e -> name, name)) {
			// Entry found. Substitute value
			free(e -> value);
			e -> value = strdup(value);
			return 1;
		}
		e = e -> next;
	}

	// Entry not found. Add new entry at the end of the list
	e = new DLListEntry;
	e -> name = strdup(name);
	e -> value = strdup(value);

	//Add at the beginning
	e -> next = head;
	e -> previous = head -> previous;
	head -> previous = e;
	e -> previous -> next = e;
	
	return 0;
}

// Removes node from the list with that name. Return 0 if nde does not exist.
int DLList::remove(char * name) {
	DLListEntry * e;
	e = head->next;
	
	// Try to find if entry is already there
	while (e!=head) {
		if (!strcmp(e->name,name)) {
			// Entry found.
			break;
		}
		e = e -> next;
	}

	if (e == head) {
		// Entry not found.
		return 0;
	}

	e -> previous -> next = e -> next;
	e -> next -> previous = e -> previous;

	free(e -> name);
	free(e -> value);
	free(e);

	return 1;
}

// Concatenates an existing list to this list.
void DLList::concatenate(DLList * list) {
	// Iterate over all items in list
	DLListEntry * e;
	e = list -> head -> next;
	while (e != head) {
		// Add item to list
		add(e->name, e->value);
		e = e->next;
	}
}


// Destructor
DLList::~DLList() {
	// Iterate over all items in list and remove them
	DLListEntry * e;
	DLListEntry *next;
	e = head -> next;
	while (e != head) {
		// Save next
		next = e -> next;
		free(e -> name);
		free(e -> value);
		delete e;
		// Go to next item
		e = next;
	}
}



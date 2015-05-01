
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

//
// Initialize a linked list
//
void llist_init(LinkedList * list)
{
	list->head = NULL;
}

//
// It prints the elements in the list in the form:
// 4, 6, 2, 3, 8,7
//
void llist_print(LinkedList * list) {
	
	ListNode * e;

	if (list->head == NULL) {
		printf("{EMPTY}\n");
		return;
	}

	printf("{");

	e = list->head;
	while (e != NULL) {
		printf("%d", e->value);
		e = e->next;
		if (e!=NULL) {
			printf(", ");
		}
	}
	printf("}\n");
}

//
// Appends a new node with this value at the beginning of the list
//
void llist_add(LinkedList * list, int value) {
	// Create new node
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	n->value = value;
	
	// Add at the beginning of the list
	n->next = list->head;
	list->head = n;
}

//
// Returns true if the value exists in the list.
//
int llist_exists(LinkedList * list, int value) {
	ListNode *e;
	e = list -> head;
	while (e != NULL) {
		if (e -> value == value) return 1;
		e = e -> next;
	}
	return 0;
}

//
// It removes the entry with that value in the list.
//
int llist_remove(LinkedList * list, int value) {
	ListNode *e;
	ListNode *t;
	e = list -> head;
	int m = 0;
	
	if (e -> value == value) {
		list -> head = e -> next;
	}
	
	t = list -> head;
	e = t -> next;

	while (e != NULL) {
		if (e -> value == value) {
			t -> next = e -> next;
			m = 1;
		}
		t = e;
		e = t -> next;
	}
	return m;
}

//
// It stores in *value the value that correspond to the ith entry.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_get_ith(LinkedList * list, int ith, int * value) {
	ListNode *e;
	int i;
	e = list -> head;
	for (i = 0; i< ith; i++) {
		if (e == NULL) return 0;
		e = e -> next;
	}
	*value = e -> value;
	return 1;
}

//
// It removes the ith entry from the list.
// It returns 1 if success or 0 if there is no ith entry.
//
int llist_remove_ith(LinkedList * list, int ith) {
	ListNode *e;
	int i;
	e = list -> head;
	for (i = 0; i< ith - 1; i++) {
		if (e == NULL) return 0;
		e = e -> next;
	}
	e -> next = e -> next -> next;
	return 1;
}

//
// It returns the number of elements in the list.
//
int llist_number_elements(LinkedList * list) {
	ListNode *e;
	int cnt = 0;
	e = list -> head;
	while (e != NULL) {
		cnt++;
		e = e->next;
	}
	return cnt;
}


//
// It saves the list in a file called file_name. The format of the
// file is as follows:
//
// value1\n
// value2\n
// ...
//
int llist_save(LinkedList * list, char * file_name) {
	FILE * fd = fopen(file_name,"w+");
	ListNode * e = list -> head;
	if (fd == NULL) return 0;
	while (e != NULL) {
		fprintf(fd,"%d\n",e -> value);
		e = e -> next;
	}
	fclose(fd);
	return 0;
}
//
// It reads the list from the file_name indicated. If the list already has entries, 
// it will clear the entries.
//
int llist_read(LinkedList * list, char * file_name) {
	llist_init(list);
	FILE * fd = fopen(file_name,"r");
	int value;
	if (fd == NULL) return 0;
	//if not end of file then read the value
	while (!feof(fd)) {
		fscanf(fd,"%d",&value);
		if (feof(fd)) break;
		llist_add(list,value);
	}
	fclose(fd);	
	return 1;
}


//
// It sorts the list. The parameter ascending determines if the
// order is ascending (1) or descending(0).
//

//swap the two nodes and return the second one.
ListNode * swap (ListNode *p1, ListNode *p2){
	p1 -> next = p2 -> next;
	p2 -> next = p1;
	return p2;
}

void llist_sort(LinkedList * list, int ascending) {
	int m = 1;
	ListNode * top = (ListNode *)malloc(sizeof(ListNode));
	ListNode * p;
	ListNode * q;
	ListNode t;
	top -> next = list -> head;
	if (ascending) {
		while (m) {
			m = 0;
			q = top;
			p = top -> next;
			while (p -> next != NULL) {
				if (p -> value > p -> next -> value) {
					q -> next = swap(p, p-> next);
					m = 1;
				}
				q = p;
				if (p -> next != NULL) p = p -> next;
			}
		}
	} else {
		while (m) {
			m = 0;
			q = top;
			p = top -> next;
			while (p -> next != NULL) {
				if (p -> value < p -> next -> value) {
					q -> next = swap(p, p-> next);
					m = 1;
				}
			q = p;
			if (p -> next != NULL) p = p -> next;
			}
		}
	
	}	
	list -> head = top -> next;
	free(top);
}

//
// It removes the first entry in the list and puts value in *value.
// It also frees memory allocated for the node
//
int llist_remove_first(LinkedList * list, int * value) {
	ListNode *n = list -> head;
	if (n != NULL) {
		*value = n -> value;
		list -> head = n -> next;
		free(n);
		return 1;
	}
	return 0;
}

//
// It removes the last entry in the list and puts value in *value/
// It also frees memory allocated for node.
//
int llist_remove_last(LinkedList * list, int *value) {
  	ListNode *e;
	ListNode *t;
	ListNode *q;
	t = list -> head;
	if (t != NULL) {
		e = t -> next;
		while (e != NULL) {
			q = t;
			t = e;
			e = t -> next;
		}
		* value = t -> value;
		q -> next = NULL;
		t = NULL;
		free(t);
		return 1;
	}

  	return 0;
}

//
// Insert a value at the beginning of the list.
// There is no check if the value exists. The entry is added
// at the beginning of the list.
//
void llist_insert_first(LinkedList * list, int value) {
	ListNode * n = (ListNode *)malloc(sizeof(ListNode));
	n -> value = value;
	n -> next = list -> head;
	list -> head = n;
}

//
// Insert a value at the end of the list.
// There is no check if the name already exists. The entry is added
// at the end of the list.
//
void llist_insert_last(LinkedList * list, int value) {
  	ListNode *e;
	ListNode *t;
	
	//Create a new node
	ListNode *n = (ListNode *)malloc(sizeof(ListNode));
	n -> value = value;
	n -> next = NULL;

	t = list -> head;
	if (t == NULL){ 
		list -> head = n;
		return;
	}
	e = t -> next;
	
	//Find the last node of linked list
	while (e != NULL) {
		t = e;
		e = t -> next;
	}

	t -> next = n;
}

//
// Clear all elements in the list and free the nodes
//
void llist_clear(LinkedList *list)
{
	ListNode *e = list -> head;
	if (e == NULL) return;
	ListNode *t = e -> next;;
	while (t != NULL) {
		t = e -> next;
		free(e);
		e = t;
	}

	list -> head = NULL;
}


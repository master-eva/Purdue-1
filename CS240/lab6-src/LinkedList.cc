
#include "LinkedList.h"

//constructor
LinkedList::LinkedList() {
	head = NULL;
}


void LinkedList::add(const char * value) {
	ListNode * n = (ListNode *)malloc(sizeof(ListNode));
	n -> value = value;
	n -> next = NULL;

	if (head == NULL) {
		head = n;
		return;
	}

	ListNode * e = head;
	while (e -> next != NULL) e = e -> next;
	e -> next = n;
}

bool LinkedList::exists(const char * value) {
	ListNode *e;
	e = head;
	while (e != NULL) {
		if (strcmp(e -> value, value) == 0) return true;
		e = e -> next;
	}
	return false;
}

ListNode * LinkedList::swap (ListNode *p1, ListNode *p2) {
	p1 -> next = p2 -> next;
	p2 -> next = p1;
	return p2;
}			

void LinkedList::sort() {
	if (head == NULL) return;
	bool flag = true;
	ListNode * top = (ListNode *)malloc(sizeof(ListNode));
	ListNode * p;
	ListNode * q;
	
	top -> next = head;
	while (flag) {
		flag = false;
		q = top;
		p = top -> next;
		while (p -> next != NULL) {
			if (strcmp(p -> value, p -> next -> value) > 0) {
				q -> next = swap(p, p -> next);
				flag = true;
			}
		q = p;
		if (p -> next != NULL) p = p -> next;
		}
	}

	head = top -> next;
	free(top);
}

bool LinkedList::remove(const char * value) {
	if (head == NULL) return false;

	if (strcmp(head -> value, value) == 0) {
		head = head -> next;
		return true;
	}

	ListNode *e;
	ListNode *t;
	e = head;
	bool m = false;
	
	t = head;
	e = t -> next;

	while (e != NULL) {
		if (strcmp(e -> value, value) == 0) {
			t -> next = e -> next;
			m = true;
		}
		t = e;
		e = t -> next;
	}
	return m;
}


bool LinkedList::removeHead() {
  	ListNode *n = head;
	if (n != NULL) {
		head = n -> next;
		free(n);
		return true;
	}
	return false;
}


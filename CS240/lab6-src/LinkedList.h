#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ListNode {
	const char * value;
	struct ListNode * next;
};

class LinkedList {
public:
	ListNode * head;

public:
	LinkedList();
	void add(const char * value);
	bool exists(const char * value);
	ListNode * swap(ListNode *p1, ListNode *p2);
	void sort();
	bool remove(const char *);
	bool removeHead();
};

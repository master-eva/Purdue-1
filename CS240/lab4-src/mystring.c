
#include <stdlib.h>
#include "mystring.h"

int mystrlen(char * s) {
	int i = 0;
	while (*s != '\0') {
		i++;
		s++;
	}

	return i;
}

char * mystrcpy(char * dest, char * src) {
	char * p = src;
	char * q = dest;
	while (* p != '\0') {
		* q = * p;
		p++;
		q++;
	}
	* q = '\0';
	return dest;
}

char * mystrcat(char * dest, char * src) {
	char * p;
	p = dest;
	while(*p != '\0') {p++;}
	char * q = src;
	while (*q) {
		*p = *q;
		p++;
		q++;
	}

	*p = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	if (mystrlen(s1) > mystrlen(s2)) {
		return 1;
	} else if (mystrlen(s1) < mystrlen(s2)){
		return -1;
	} else {
		char * p = s1;
		char * q = s2;
		while (*p){
			if (*p > *q) return 1;
			if (*p < *q) return -1;
			p++;
			q++;
		}
	}
	return 0;
}

char * mystrstr(char * hay, char * needle) {
	int m = 1;
	char * p = hay;
	char * q = needle;
	while ((*p) && m == 1) {
		q = needle;
		if (*p == *q) {
			m = 0;
			while (*q){
				if (*p != *q) {
					m = 1;
					break;
				}
				p++;
				q++;
			}
		}
		p++;
	}
	if (m == 1) return NULL;
	else return p-mystrlen(needle)-1;
}

char * mystrdup(char * s) {
	char * ts = (char *)malloc(mystrlen(s)+1);
	mystrcpy(ts,s);
	return ts;
}

char * mymemcpy(char * dest, char * src, int n)
{
	char * p = src;
	char * q = dest;
	int i = 0;
	while (i < n) {
		*q = *p;
		p++;
		q++;
		i++;
	}
	
	return dest; 
}


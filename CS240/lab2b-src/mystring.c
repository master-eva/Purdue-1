
#include <stdlib.h>
#include "mystring.h"

// Type "man string" to see what every function expects.

int mystrlen(char * s) {
	int i = 0;
	while (s[i] != '\0') i++;
	return i;
}

char * mystrcpy(char * dest, char * src) {
	int i = 0;
	while (src[i] != '\0'){
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return dest;
}

char * mystrcat(char * dest, char * src) {
	int len = mystrlen(dest);
	int i = 0;
	while (src[i] != '\0'){
		dest[len + i] = src[i];
		i++;
	}
	dest [len + i] = '\0';
	return dest;
}

int mystrcmp(char * s1, char * s2) {
	int i = 0;
	int l1 = mystrlen(s1);
	int l2 = mystrlen(s2);
	if (l1 > l2) return 1;
	else if (l2 > l1) return -1;
	else {
		while (s1[i] != '\0') {
			if (s1[i] > s2[i]) return 1;
			if (s1[i] < s2[i]) return -1;
			i++;
		}
	}
	return 0;
}

char * mystrstr(char * hay, char * needle) {
	int i = 0;
	int j;
	int mark =  -1;
	int l = mystrlen(needle);
	while (hay[i] != '\0' && mark == -1) {
		if (hay[i] == needle[0]) {
			mark = 0;
			for (j = 0; j < l; j++) {
				if (hay[i+j] != needle[j])
					mark = -1;
			}
		}
	i++;
	}
	if (mark == -1) return NULL;
	else return &hay[i-1];
}

char * mystrdup(char * s) {
	char *s1 = (char *)malloc(mystrlen(s)+1);
	mystrcpy(s1,s);
	return s1;
}

char * mymemcpy(char * dest, char * src, int n)
{
	int i = 0;
	for ( ;i<n;i++){
		dest[i] = src[i];
	}
	return dest;
}


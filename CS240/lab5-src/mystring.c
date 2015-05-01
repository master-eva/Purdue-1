
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Adds src at the end of dest. Return the value of dest.
char *mystrcat(char * dest, char * src) {
	char *p = dest;
	char *q = src;
	while (*p != '\0') p++;
	while (*q != '\0') {
		*p = *q;
		p++;
		q++;
	}
	*p = '\0';
	return dest; 
}


// Return a pointer to the first occurrence of the substring needle in
// the string haystack. 
// Return NULL if the string is not found.
char * mystrstr(char * haystack, char * needle) {
	char *p = haystack;
	char *q = needle;
	char *last;
	while( *p != '\0' ){
		last = p;
		while(*p == *q){
			if(*(q+1) == '\0'){//end of needle->needle is found
				return last;
			}
			q++;
			p++;
		}
		p = last;
		p++; //advance along hay
		q=needle; //reset needle
	}
	return NULL;
}


// Trims any spaces that are at the beginning or the end of s and returns s.
// Use the same memory passed in s. 
// Example:
// char s1[20];
// strcpy(s1,      hello    );
// strtrim(s1);
// printf(<s1=%s>\n, s1); // it will print <s1=hello>  without the spaces.
char * mystrtrim(char * s) {
	char * p = s;
	char * test = s;
	while (*test == ' ') test++;
	while (*test != ' ' && *test != '\0') {
		*p = *test;
		p++;
		test++;
	}
	* p = '\0';
	return s;
}


// Returns a new string that will substitute the first occurrence of the substring from
//  to the substring to in the string src
// This call will allocate memory for the new string and it will return the new string.
// Example:
// char a[6];
// strcpy(a, apple);
// char * b = strreplace(a, pp, orange);
// printf(<b=%s>\n, b); // It will print <b=aorangele>  
char * mystrreplace( char * src, char * from, char * to)  {
	// handle length of new sthring
	char * str = (char *)malloc(1000*sizeof(char));
	char * p = str;
	char * t = src;
	
	while ((mystrstr(src,from) - src) >= 0 ) {
		int slen = mystrstr(src,from) - src;
		int i;
		char * q;

		for (i = 0; i < slen; i++) {
			*p = *src;
			p++;
			src++;
		}
		*p = '\0';

		q = from;
		while (*q != '\0') {
			src++;
			q++;
		}

		strcat(str,to);
		q = to;
		while (*q != '\0') {
			p++;
			q++;
		}
	}
	
	strcat(str,src);
	return str;
}




#include <stdlib.h>

int strlen(char * str) {
	char *p = str;
	int i = 0;
	while (*p != '\0') {
		p++;
		i++;
	}
	return i;
}

char * strcpy(char * dest, char * src) {
	char * p = src;
	char * q = dest;
	while (*p) {
		*q = *p;
		p++;
		q++;
	}
	*q = '\0';
	return dest;
}

char * strcat(char * dest, char * src) {
	char * p = src;
	char * q = dest;

	while (*q != '\0') q++;

	while (*p != '\0') {
		*q = *p;
		p++;
		q++;
	}
	*q = '\0';
	return dest;
}

char * strstr(char * hay, char * needle) {
	// Compare until the end of hay
	while (*hay != '\0') {
		char * p = hay;
		char * q = needle;
		// Compare character by character until end of needle or hay
		while (*p != '\0' && *q != '\0' && *p == *q) {
			p++;
			q++;
		}
		if (*q == '\0') {
			// If we reached the end of needle then needle is
			// contained by hay
			return hay;
		}
		// Needle not contained in hay at that position. Increase hay
		hay++;
	}

	// Needle not found in hay
	return NULL;
}


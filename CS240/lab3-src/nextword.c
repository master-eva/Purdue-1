
#include <stdio.h>
#include <stdlib.h>

//
// Separates the file into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;

// It returns the next word from fd.
// If there are no more more words it returns NULL. 
char * nextword(FILE * fd) {
  	int c;
  	char * nw = NULL;
  	wordLength = 0;

	while ((c = fgetc(fd)) != -1) {
 		if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
			word[wordLength++] = c;
			nw = word;
			nw[wordLength] = '\0';
		} else {
			if (wordLength != 0){
				wordLength = 0;
				return nw;
			} 
		}
 	}	

 	if (wordLength != 0) return nw;
	return NULL;
}


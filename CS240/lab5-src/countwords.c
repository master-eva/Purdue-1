#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WordInfo {
	char * word;
	int count;
};

typedef struct WordInfo WordInfo;

int maxWords;
int nWords;
WordInfo*  wordArray;

#define MAXWORD 100
int wordLength;
char word[MAXWORD];
FILE * fd;
int charCount;
int wordPos;

void toLower(char *s) {
	while (*s != '\0') {
		if (*s >= 'A' && *s <= 'Z') 
			*s = *s + 'a' - 'A';
		s++;
	}
}


// It returns the next word from stdin.
// If there are no more more words it returns NULL. 
static char * nextword() {
  	int c;
  	char * nw = NULL;
  	wordLength = 0;

	while ((c = fgetc(fd)) != -1) {
 		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			word[wordLength++] = c;
			word[wordLength] = '\0';
			nw  = word;
		} else {
			if (wordLength > 0) {
				toLower(nw);
				return nw;
			}
		}
 	}	

 	if (wordLength > 0) {
		toLower(nw);
		return nw;
	}

	return NULL;
}


int
main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: countwords filename\n");
		exit(1);
	}

	fd = fopen(argv[1], "r");
	
	if (fd == NULL) {
		printf("No such file.\n");
		exit(1);
	}
	
	maxWords = 10;
	nWords = 0;
	wordArray = (WordInfo *) malloc(maxWords * sizeof(WordInfo));
	int i;
	for (i = 0; i < maxWords; i++) {
		wordArray[i].word = NULL;
		wordArray[i].count = 0;
	}

	char * nw;
	while ((nw = nextword()) != NULL) {
		int found = 0;

		//case that found the word
		for (i = 0; i < nWords; i++) {
			if (strcmp(wordArray[i].word,nw) == 0) {
				found = 1;
				wordArray[i].count += 1;
				break;
			}
		}

		//case that cannot find the word
		if (!found) {
			//handle the case that need resize
			if (nWords >= maxWords) {
				maxWords += 10;
				wordArray = (WordInfo *)realloc(wordArray,maxWords * sizeof(WordInfo));
				for (i = maxWords - 10; i < maxWords; i++) {
					wordArray[i].word = NULL;
					wordArray[i].count = 0;
				}
			}

			wordArray[nWords].word = strdup(nw);
			wordArray[nWords].count = 1;
			nWords += 1;
		}
	}

	//sort
	int j;
	WordInfo * p;
	WordInfo * q;
	WordInfo temp;
	for (i = 0; i < nWords -1; i++) {
		for (j = 0; j < nWords - 1 - i; j++) {
			p = wordArray + j;
			q = wordArray + j + 1;
			if (strcmp((*p).word,(*q).word) > 0) {
				temp = *p;
				*p = *q;
				*q = temp;
			}
		}
	}

	for (i = 0; i < nWords; i++){
		printf("%s %d\n",wordArray[i].word,wordArray[i].count);
	}

	return 0;
}


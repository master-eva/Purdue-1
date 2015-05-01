
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordTable.h"

// Initializes a word table
void wtable_init(WordTable * wtable)
{
	// Allocate and initialize space for the table
	wtable->nWords = 0;
	wtable->maxWords = 10;
	wtable->wordArray = (WordInfo *) malloc(wtable->maxWords * sizeof(WordInfo));
	for (int i = 0; i < wtable->maxWords; i++) {
		llist_init(&wtable->wordArray[i].positions);
	}
}

// Add word to the tableand position. Position is added to the corresponding linked list.
void wtable_add(WordTable * wtable, char * word, int position)
{
	// Find first word if it exists
	for (int i = 0; i < wtable->nWords; i++) {
		if ( strcmp(wtable->wordArray[i].word, word)== 0 ) {
			// Found word. Add position in the list of positions
			llist_insert_last(&wtable->wordArray[i].positions, position);
			return;
		}
	}

	// Word not found.

	// Make sure that the array has space.
	// Expand the wordArray here.
	if (wtable -> nWords >= wtable -> maxWords) {
		wtable -> maxWords += 10;
		wtable -> wordArray =(WordInfo *) realloc(wtable -> wordArray,wtable -> maxWords * sizeof(WordInfo));
		memset(&wtable -> wordArray[wtable -> nWords], 0, (wtable -> maxWords - wtable -> nWords) * sizeof(WordInfo));
	}

	// Add new word and position
	wtable->wordArray[wtable->nWords].word = strdup(word);
	llist_insert_last(&wtable->wordArray[wtable->nWords].positions, position);
	wtable->nWords++;
}

// Print contents of the table.
void wtable_print(WordTable * wtable, FILE * fd)
{
	fprintf(fd, "------- WORD TABLE -------\n");

	// Print words
	for (int i = 0; i < wtable->nWords; i++) {
		fprintf(fd, "%d: %s: ", i, wtable->wordArray[i].word);
		llist_print( &wtable->wordArray[i].positions);
	}
}

// Get positions where the word occurs
LinkedList * wtable_getPositions(WordTable * wtable, char * word)
{
	for (int i = 0; i < wtable->nWords; i++) {
		if ( strcmp(wtable->wordArray[i].word, word)== 0 ) {
			//Found word, return positions.
			return &wtable->wordArray[i].positions;
		}
	}
	return NULL;
}

//
// Separates the string into words
//

#define MAXWORD 200
char word[MAXWORD];
int wordLength;
int length;
int wordCount;
int charCount;
int wordPos;

// It returns the next word from stdin.
// If there are no more more words it returns NULL.
// A word is a sequence of alphabetical characters.
static char * nextword(FILE * fd) {
  	int c;
  	char * nw = NULL;
  	wordLength = 0;

	while ((c = fgetc(fd)) != -1) {
		wordPos++;
 		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			word[wordLength++] = c;
			nw = word;
			nw[wordLength] = '\0';
		} else {
			if (wordLength != 0) {
				length = wordLength;			
				wordLength = 0;
				return nw;
			}
		}
 	}	

 	if (wordLength != 0) {
		length = wordLength;
		wordLength = 0;
		return nw;
	}
	return NULL;
}

// Conver string to lower case
void toLower(char *s) {
	while (*s != '\0') {
		if (*s >= 'A' && *s <= 'Z') 
			*s = *s + 'a' - 'A';
		s++;
	}
}


// Read a file and obtain words and positions of the words and save them in table.
int wtable_createFromFile(WordTable * wtable, char * fileName, int verbose)
{
	wordPos = 0;
	wordCount = 0; 
	FILE * fd = fopen(fileName,"r");
	if (fd == NULL) return 0;
	char * nw;
	while ((nw = nextword(fd))!= NULL) {
		toLower(nw);
		if (verbose) printf("%d: word=%s, pos=%d\n", wordCount++, nw, wordPos - length - 1);
		wtable_add(wtable,nw,wordPos - length - 1);
	}
	return 0;
}

//cmp alphabetical 
int cmp(char *p, char *q) {
	while (*p != '\0' && *q != '\0') {
		if (*p > *q) return 1;
		else if (*q > *p) return 0;
		p++;
		q++;
	}
	if (*q == '\0') return 1;
	return 0;
}

// Sort table in alphabetical order.
void wtable_sort(WordTable * wtable)
{
	int i;
	int j;
	int n = wtable -> nWords;
	WordInfo * p;
	WordInfo * q;
	WordInfo temp;
	for (i = 0; i < n -1; i++) {
		for (j = 0; j < n - 1 - i; j++) {
			p = wtable -> wordArray + j;
			q = wtable -> wordArray + j + 1;
			if (cmp((*p).word,(*q).word)) {
				temp = *p;
				*p = *q;
				*q = temp;
			}
		}
	}
}

// Print all segments of text in fileName that contain word.
// at most 200 character. Use fseek to position file pointer.
// Type "man fseek" for more info. 
int wtable_textSegments(WordTable * wtable, char * word, char * fileName)
{
	FILE * fd = fopen(fileName,"r");
	int i;
	char c;
	LinkedList * list = wtable_getPositions(wtable,word);
	ListNode * e = list -> head;
	printf("===== Segments for word \"%s\" in book \"%s\" =====\n",word,fileName);
	while (e != NULL) {
		printf("---------- pos=%d-----\n",e->value);
		printf("......");
		fseek(fd, e-> value, SEEK_SET);
		for (i = 0; i < 200; i++) {
			c = fgetc(fd);
			if (c == -1) break;
			printf("%c",c);
		}
		printf("......\n");
		e = e -> next;
	}

}


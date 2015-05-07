
#include <stdlib.h>

#include "RArray.h"

//Create a new resizable array. Make maxElements=10, that is the initial max
//size of the array before resizing.
RArray * rarray_create( ) {

	RArray * rarray = (RArray *) malloc(sizeof(RArray));
	if (rarray == NULL) return NULL;
	
	rarray -> maxElements = 10;
	rarray -> nElements = 0;
	
	rarray -> array = (struct RArrayEntry *) malloc(rarray -> maxElements*sizeof(RArrayEntry));
	if (rarray -> array==NULL) {
		return NULL;
	}
	
	return rarray;

}

// Append a new value at the end of the resizable array
void rarray_append(RArray *rarray, char * value) {
	rarray_insert_at(rarray, rarray -> nElements - 1, value);
}

// Insert a value in the middle of the array at index ith.
// Move the elements beyond ith one position up if necessary
// and enlarge the array if necessary. Return 0 if ith is out of range or 1 otherwise.
int rarray_insert_at(RArray * rarray, int ith, char * value) {
	if ( ith < 0 || ith >= rarray -> nElements) {
		return 0;
	}

	// Make sure that there is enough space
	if (rarray -> maxElements == rarray -> nElements) {
		// Not enough space. Enlarge table. Double the size
		rarray->maxElements = 2 * rarray->maxElements;
		rarray->array = (RArrayEntry *) realloc( rarray->array, rarray->maxElements * sizeof(RArrayEntry));
		if (rarray->array==NULL) {
			exit(1);	
		}
	}

	// Move elements one position up starting from the last element
	int i;
	for (i = rarray -> nElements-1; i >= ith; i--) {
		rarray->array[i+1] = rarray->array[i];
	}

	//
	// Add value to a new entry.
	// We need to use strdup to create a copy of the string.
	// otherwise ame and value can be overwritten by the caller
	//
	rarray -> array[ith].value = strdup(value);
	rarray -> nElements++;
	
	return 1;
}

// Remove an element in the middle of the array at index ith. Return 0 if ith is out of range or 1 otherwise.
// Move the elements beyond ith one position down.
int rarray_remove_at(RArray * rarray, int ith) {

	if ( ith < 0 || ith >= rarray -> nElements) {
		return 0;
	}

	// Remove entry.
	free(rarray->array[ith].value);

	int j;
	// Push elements backwards
	for (j=ith; j<rarray->nElements-1; j++) {
		rarray->array[j].value = rarray->array[j+1].value;
	}
	
	return 1;
}

// Free the memory needed by the resizable array
void rarray_free(RArray * rarray) {
	int j;
	// Free elements 
	for (j=0; j<rarray->nElements-1; j++) {
		free(rarray->array[j].value);
	}

	// Free array
	free(rarray->array);

	// Free rarray struct
	free(rarray);
}


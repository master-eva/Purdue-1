#include "mysort.h"
#include <alloca.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//
// Sort an array of element of any type
// it uses "compFunc" to sort the elements.
// The elements are sorted such as:
//
// if ascending != 0
//   compFunc( array[ i ], array[ i+1 ] ) <= 0
// else
//   compFunc( array[ i ], array[ i+1 ] ) >= 0
//
// See test_sort to see how to use mysort.
//
void mysort( int n,                      // Number of elements
	     int elementSize,            // Size of each element
	     void * array,               // Pointer to an array
	     int ascending,              // 0 -> descending; 1 -> ascending
	     CompareFunction compFunc )  // Comparison function.
{
	int i,j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			void * a = array + elementSize * j;
			void * b = array + elementSize * (j + 1);
			void * temp = malloc(elementSize);
			if (ascending) {
				if ((*compFunc)(a,b) > 0) {
					memcpy(temp, a, elementSize);
					memcpy(a, b, elementSize);
					memcpy(b, temp, elementSize);
				}
			} else {
				if ((*compFunc)(a,b) < 0) {
					memcpy(temp, a, elementSize);
					memcpy(a, b, elementSize);
					memcpy(b, temp, elementSize);	
				}
			}
		}
	}
}


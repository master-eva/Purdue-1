
#include <stdio.h>

#include "array.h"

// Return sum of the array
double sumArray(int n, double * array) {
  double sum = 0;
  
  double * p = array;
  double * pend = p+n;

  while (p < pend) {
    sum += *p;
    p++;
  }

  return sum;
}

// Return maximum element of array
double maxArray(int n, double * array) {
	double max = *array;
	int i = 0;
	double * p = array;
	while (i < n) {
		if (*p > max ) max = *p;
		p++;
		i++;
	}

	return max;
}

// Return minimum element of array
double minArray(int n, double * array) {
	double min = *array;
	int i = 0;
	double * p = array;
	while (i < n) {
		if (*p < min ) min = *p;
		p++;
		i++;
	}

	return min;
}

// Find the position int he array of the first element x 
// such that min<=x<=max or -1 if no element was found
int findArray(int n, double * array, double min, double max) {
 	double *p = array;
	int i = 0;
	while (i < n) {
		if ((*p >= min)&&(*p <= max)) return i;
		p++;
		i++;
	}

	return -1;
}

// Sort array without using [] operator. Use pointers 
// Hint: Use a pointer to the current and another to the next element
int sortArray(int n, double * array) {
	double * p;
	double * q;
	double temp;
	int i;
	int j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1 - i; j++) {
			p = array + j;
			q = array + j + 1;
			if (*p > *q) {
				temp = *q;
				*q = *p;
				*p = temp;
			}
		}
	}
}

// Print array
void printArray(int n, double * array) {
	double *p = array;
	int i = 0;
	while (i < n) {
		printf("%d:%lf\n",i,*p);
		p++;
		i++;
	}
}


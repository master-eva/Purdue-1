
#include <string.h>
#include <stdlib.h>

// Comparison function. Returns 1 if a > b, -1 if a<b, 0 if a == b
typedef int (*COMP_FUNC)(void * a, void *b);

void sort_array(void * array, int num_elements, int element_size, COMP_FUNC comp_func) {
	int i,j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			void * a = array + elementSize * j;
			void * b = array + elementSize * (j + 1);
			void * temp = malloc(elementSize);
			if ((*compFunc)(a,b) < 0) {
				memcpy(temp, a, elementSize);
				memcpy(a, b, elementSize);
				memcpy(b, temp, elementSize);	
			}
			free(temp);
		}
	}
}

int compInt(void* a, void * b) {
	// Write int comparison function
	int *p1 = (int *)a;
	int *p2 = (int *)b;

	if(*p1 > *p2){
		return 1;
	}
	else if(*p1< *p2) {
		return -1;
	}
	else{
		return 0;
	}
}

int compStr(void* a, void * b) {
	// write string comparison function
	char **p1 = (char**)a;
	char **p2 = (char**)b;

	//a Pointer to a pointer to the string
	if(strcmp(*p1, *p2)>0) {
		return 1;
	}
	else if (strcmp(*p1, *p2)<0) {
		return -1;
	}
	else {
		return 0;
	}
}

int main(){
	int a[] = {7,8,1,4,3,2};
       int n=sizeof(a)/sizeof(int);
       sort_array(a, n, sizeof(int), compInt);
       int i=0;
       for(i=0;i<n;i++){
              printf("%d\n", a[i]);
       }

       char *strings[] = {"pear", "banana", "apple", "strawberry"};
       n = sizeof(strings)/sizeof(char*);
       sort_array(strings, n, sizeof(char*), compStr);
       for(i=0; i<n; i++) {
              printf("%s\n", strings[i]);
       }
}






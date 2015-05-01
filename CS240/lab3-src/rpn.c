
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include "rpn.h"
#include "nextword.h"
#include "stack.h"

double rpn_eval(char * fileName, double x) {
	double a, b, c;

	// Open file in fileName it.
 	FILE * fd = fopen(fileName,"r");

 	char * word;
 	while (word = nextword(fd)) {
 		if (!strcmp(word,"+") || !strcmp(word,"-") || !strcmp(word,"*") || 
 			!strcmp(word,"/") || !strcmp(word,"sin") || !strcmp(word,"cos") || 
 			!strcmp(word,"pow") || !strcmp(word,"log") || !strcmp(word,"exp")) {
 			if (!strcmp(word,"+")) {
 				a = stack_pop();
 				b = stack_pop();
 				stack_push(a+b);
 			} else if (!strcmp(word,"-")) {
 				a = stack_pop();
 				b = stack_pop();
 				stack_push(b-a);
 			} else if (!strcmp(word,"*")) {
 				a = stack_pop();
 				b = stack_pop();
 				stack_push(a*b);
 			} else if (!strcmp(word,"/")) {
 				a = stack_pop();
 				b = stack_pop();
 				stack_push(b/a);
 			} else if (!strcmp(word,"sin")) {
 				a = stack_pop();
 				a = sin(a);
 				stack_push(a);
 			} else if (!strcmp(word,"cos")) {
 				a = stack_pop();
 				a = cos(a);
 				stack_push(a);
 			} else if (!strcmp(word,"log")) {
 				a = stack_pop();
 				a = log(a);
 				stack_push(a);
 			} else if (!strcmp(word,"pow")) {
 				a = stack_pop();
 				b = stack_pop();
 				c = pow(b,a);
 				stack_push(c);
 			} else if (!strcmp(word,"exp")) {
				a = stack_pop();
				a = exp(a);
				stack_push(a);
			}
 		} else if (!strcmp(word,"x")) {
 			stack_push(x);
 		} else {
 			stack_push(atof(word));
 		}
 	}

 	if (stack_top() > 1) {
		printf("Elements remain in the stack\n");
		exit(1);
	} else {
		double result =  stack_pop();
		return result;
	}
 }


#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Wrong Usage.\n");
		return 2;
	}

	char * s = argv[1];

	char stack[100];
	int top = 0;

	while (*s != '\0') {
		if (*s == '(' || *s == '[' || *s == '{') {
			stack[top++] = *s;
		} else if (*s == ')' || *s == '}' || *s == ']') {
			top--;
			if (top < 0) {
				printf("string is not balanced\n");
				return 1;
			}
			if ((*s == ')' && stack[top] != '(') || (*s == '}' && stack[top] != '{') || (*s == ']' && stack[top] != '[')) {
				printf("string is not balanced\n");
				return 1;
			}

		}
		s++;
	}

	if (top == 0) printf("string is balanced\n");
	else printf("string is not balanced\n");

}

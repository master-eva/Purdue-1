#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check(int base, char * number) {
	int i;
	int length = strlen(number);
	for (i = 0; i < length; i++) {
		if (number[i] - '0' < 10) {
			if (number[i] - '0' >= base) {
				printf("Wrong digit in number.\n");
				exit(1);
			}
		} else {
			if (number[i] - 'A' + 10 >= base) {
				printf("Wrong digit in number.\n");
				exit(1);
			}
		}
	}
	return 0;
}

int pow(int base, int exp){
	int ans = 1;
	while (exp > 0) {
		ans = ans * base;
		exp--;
	}
	return ans;
}

int converttoten(int base, char * number) {
	if (base == 10) return atoi(number);

	int i;
	int ans = 0;
	int length = strlen(number);
	int t = length - 1;
	
	for (i = 0; i < length; i++) {
		if (number[i] - '0' < 10) {
			ans = ans + (number[i] - '0') * pow(base,t--);
		} else {
			ans = ans + (number[i] - 'A' + 10) * pow(base,t--);
		}
	}
	return ans;
}

char * converttobase(int base, int number) {
	char num[100];
	int length = 0;
	while (number > 0) {
		int t = number % base;
		number = number / base;
		if (t < 10) {
			num[length++] =  t + '0';
		} else {
			num[length++] = t - 10 + 'A';
		}
	}

	char * ans;
	char reverse[length];
	int i;

	for (i = 0; i < length; i++) {
		reverse[i] = num[length - i - 1];
		ans = reverse;
	}

	reverse[length] = '\0';

	return ans;
}

int 
main(int argc, char ** argv){
	if (argc != 4) {
		printf("Usage:  convert <basefrom> <baseto> <number>\n");
		exit(1);
	}
	
	int basefrom = atoi(argv[1]);
	int baseto = atoi(argv[2]);
	char * t = argv[3];
	printf("Number read in base %d: %s\n",basefrom, t);

	if (!check(basefrom,t)) {
		int x;
		printf("Converted to base 10: %d\n", x = converttoten(basefrom,t));
		printf("Converted to base %d: %s\n",baseto,converttobase(baseto,x));
	}
}

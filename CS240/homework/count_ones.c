#include <stdio.h>

int countOnes(unsigned int value) {
	int n  = 0;
	int mask  = 1;

	while (value > 0) {
		n += value & mask;
		value = value >> 1;
	}

	return n;
}

int main()
{
	printf("countOnes(0x30303)=%d\n",
	       countOnes(0x30303));
}

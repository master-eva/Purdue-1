#include <stdio.h>
// It prints the bits in bitmap as 0s and 1s.
void printBits(unsigned int bitmap) {
	int i;
	for (i = 0; i < 32; i++) 
		printf("%d",bitmap >> (31 - i) & 1);
	printf("\n10987654321098765432109876543210\n");
}


// Sets the ith bit in *bitmap with the value in bitValue (0 or 1)
void setBitAt( unsigned int *bitmap, int i, int bitValue ) {
	int mask = 1 << i;
	if (bitValue) *bitmap = *bitmap | mask;
	else *bitmap = *bitmap & (~mask);

}

// It returns the bit value (0 or 1) at bit i
int getBitAt( unsigned int bitmap, unsigned int i) {
	return (bitmap >> i) & 1;
}

// It returns the number of bits with a value "bitValue".
// if bitValue is 0, it returns the number of 0s. If bitValue is 1, it returns the number of 1s.
int countBits( unsigned int bitmap, unsigned int bitValue) {	
	int count = 0;
	
	while (bitmap) {
		if (bitmap & 1) count++;
		bitmap = bitmap >> 1;
	}
	
	if (!bitValue) count = 32 - count;
	
	return count;
}

// It returns the number of largest consecutive 1s in "bitmap".
// Set "*position" to the beginning bit index of the sequence.
int maxContinuousOnes(unsigned int bitmap, int * position) {
	int count = 0;
	int max = 0;
	int i = 0;

	while (bitmap) {
		if (bitmap & 1) {
			count += 1;
		} else {
			if (count > max) {
				max = count;
				*position = i - count;
			}
			count = 0;
		}
		bitmap = bitmap >> 1;
		i++;
	}

	return max;
}


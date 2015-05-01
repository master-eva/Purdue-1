#include <stdio.h>

int
main(int argc, char **argv) {
  printf("Welcome to the High Low game...\n");
  while (1) {
    printf("Think of a number between 1 and 100 and press press <enter>\n");
    getchar();
    int high = 100;
    int low = 1;
    int mid;
    char c;
    while (high>=low) {
      mid = (high+low)/2;
      printf("Is it higher than %d? (y/n)\n",mid);
      c = getchar();
	  getchar();
        if (c == 'y') {
          low = mid + 1;
        } else if (c == 'n') {
          high = mid - 1;
        } else {
          printf("Type y or n\n");
        }
    }
    printf("\n>>>>>> The number is %d\n\n",low);
    printf("Do you want to continue playing (y/n)?\n");
	c = getchar();
	getchar();
	if (c != 'y') {
	  printf("Thanks for playing!!!\n");
	  break;
	}
  }

}


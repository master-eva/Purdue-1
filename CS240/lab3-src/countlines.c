
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char ** argv) {
	int c;
	int lines=0;

 	//get and open file; if not exist then exit
 	printf("Program that counts lines.\n");
 	char * fileName = argv[1];
 	FILE * fd = fopen(fileName,"r");
 	if (fd == NULL) {
 		printf("Could not open file %s\n", fileName);
 		exit(1);
	}

	//iterate over all characters and count lines, then print
 	while ((c=fgetc(fd))!=-1) {
 		if (c == '\n') lines++;
 	}
 	printf("Total lines: %d\n", lines);
 	
 	fclose(fd);
	exit(0);
}

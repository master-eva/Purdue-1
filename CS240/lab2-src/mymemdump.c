
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mymemdump(FILE * fd, char * p , int len) {
    int i,j;
    //loop to control lines
    for (i = 0; i < len/16; i++){
    	//print address of pointer
    	fprintf(fd,"0x%016lX: ", (unsigned long) (p + 16*i));
    	//print address of char
	for (j = 0; j < 16; j++){
            int c = p[i*16+j]&0xFF;
            fprintf(fd, "%02X ", c);
        }
        fprintf(fd," ");
	//print the character
        for (j = 0; j < 16; j++) {
            int c = p[i*16+j];
            fprintf(fd, "%c", (c>=32)?c:'.');
        }
	fprintf(fd,"\n");
    }
    
    //the last line if there are remain characters
    if (i*16 < len) {
        int r = len % 16;
	//print address
        fprintf(fd,"0x%016lX: ", (unsigned long) (p + 16*i));
        for (j = 0; j < r; j++){
            int c = p[i*16+j]&0xFF;
            fprintf(fd, "%02X ", c);
        }
	//print the space
        for (j = r; j < 16; j++) {
            fprintf(fd,"   ");
        }
	fprintf(fd," ");
	//print the character
        for (j = 0; j < r; j++) {
            int c = p[i*16+j];
            fprintf(fd, "%c", (c>=32)?c:'.');
        }
        fprintf(fd,"\n");
    }
}

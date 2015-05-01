
#include <stdio.h>
#include <stdlib.h>

#include "rpn.h"

#define MAXCOLS 80
#define MAXROWS 40

char plot[MAXROWS][MAXCOLS];

void clearPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      plot[i][j] = ' ';
    }
  }
}

void printPlot()
{
  for (int i = 0; i < MAXROWS; i++) {
    for (int j = 0; j < MAXCOLS; j++) {
      printf("%c", plot[i][j]);
    }
    printf("\n");
  }
}

void plotXY(int x, int y, char c) {
  if ( x <0 || x>=MAXCOLS || y < 0 || y >=MAXROWS) return;
  plot[y][x]=c;
}

void createPlot( char * funcFile, double minX, double maxX) {
  int nvals = MAXCOLS;
  double yy[MAXCOLS];
  
  clearPlot();

  double stepX = (maxX - minX) / MAXCOLS;

  // Evaluate function and store in vector yy
  int i = 0;
  for (i = 0; i < 80; i++){
  	yy[i] = rpn_eval(funcFile, (minX + i * stepX));
  }
  //Compute maximum and minimum y in vector yy
  double maxY = yy[0];
  double minY = yy[0];
  for (i = 1; i < 80; i++) {
	if (yy[i]> maxY) maxY = yy[i];
	if (yy[i]< minY) minY = yy[i];
  }

  double stepY = (maxY - minY) / 40;

  //Plot x axis
  int cy = 39 + minY/stepY;
  for (i = 0; i < 80; i++) {
	plotXY(i,cy,'_');
  }

  //Plot y axis
  int cx = 80 - maxX/stepX;
  for (i = 0; i < 40; i++) {
	plotXY(cx,i,'|');
  }

  // Plot function. Use scaling.
  for (i = 0; i < 80; i++)
	plotXY(i, cy - yy[i]/stepY,'*');

  printPlot();

}

int main(int argc, char ** argv)
{
  printf("RPN Plotter.\n");
  
  if (argc < 4) {
    printf("Usage: plot func-file xmin xmax\n");
    exit(1);
  }

  // Get arguments
  //char * funcName = argv[1];
  //double xmin = 0;
  //double xmax = 0;
  //sscanf(argv[2],"%lf",&xmin);
  //sscanf(argv[3],"%lf",&xmax);
  createPlot(argv[1], atof(argv[2]), atof(argv[3]));
}

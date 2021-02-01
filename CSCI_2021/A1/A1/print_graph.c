#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Prints a graph of the values in data which is an array of integers
// that has len elements. The max_height argument is the height in
// character rows that the maximum number data[] should be.  A sample
// graph is as follows:
//
// length: 50
// min: 13
// max: 996
// range: 983
// max_height: 10
// units_per_height: 98.30
//      +----+----+----+----+----+----+----+----+----+----
// 996 |                X
// 897 |       X        X X            X
// 799 |  X    X X   X  X X    X       X                X
// 701 |  XX   X X   X  XXX    X      XX   XXX    X   X XX
// 602 |  XX   X X  XX  XXX X  X      XX  XXXX    XX  X XX
// 504 |  XX   XXX  XX  XXX XX X      XXX XXXX XX XX  X XX
// 406 |  XX X XXX XXXX XXX XX X  XXX XXX XXXXXXXXXX  X XX
// 307 | XXX X XXX XXXXXXXXXXX X XXXX XXXXXXXXXXXXXXX X XX
// 209 | XXX XXXXXXXXXXXXXXXXX XXXXXX XXXXXXXXXXXXXXXXX XX
// 111 | XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  13 |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//      +----+----+----+----+----+----+----+----+----+----
//      0    5    10   15   20   25   30   35   40   45

void print_graph(int *data, int len, int max_height) {
  int min = data[0];
  int max = data[0];
  int current;

  //FINDING MAX AND MIN FROM ARRAY
  for (int i=1; i < len; i++) {
    current = data[i];
    if (current < min)
      min = current;
    else if (current > max)
      max = current;
  }
  int range = max - min;
  float units_per_height = ((float)range)/max_height;

  //DATA
  printf("length: %d\n", len);
  printf("min: %d\n", min);
  printf("max: %d\n", max);
  printf("range: %d\n", range);
  printf("max_height: %d\n", max_height);
  printf("units_per_height: %.2f\n", units_per_height);

  //DASHED LINES
  printf("     "); //5 spaces
  for (int i = 0; i < len; i++) {
    if (i % 5 == 0)
      printf("+");
    else
      printf("-");
  }//for
  printf("\n");

  //MAIN BODY
  int num;
  for (int i = max_height; i >= 0; i--) {
    num = (int) (min + i*units_per_height);
    printf("%3d |", num);

    //PRINTS X OR SPACE
    for (int k = 0; k < len; k++) {
      if (data[k] >= num)
        printf("X");
      else
        printf(" ");
      }//inner for
    printf("\n");
  }//outer for

  //DASHED LINES
  printf("     "); //5 spaces
  for (int i = 0; i < len; i++) {
    if (i % 5 == 0)
      printf("+");
    else
      printf("-");
  }//for
  printf("\n");

  //INDEX REFERENCE
  printf("     "); //5 spaces
  for (int i = 0; i < len; i+=5) {
    printf("%-5d", i);
  }//for
  printf("\n");
}

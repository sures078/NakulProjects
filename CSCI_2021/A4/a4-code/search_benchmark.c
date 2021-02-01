#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "search.h"
#include "search_funcs.c"

int main(int argc, char *argv[]){
  char* algorithms;
  int min_data_size = atoi(argv[1]);
  int pow_min = pow(2, min_data_size); //2^min
  int max_data_size = atoi(argv[2]);
  int pow_max = pow(2, max_data_size); //2^max
  int reps = atoi(argv[3]);
  if (argc == 5)
    algorithms = argv[4];
  else
    algorithms = "albt";

  int run_linear_array = 0;
  int run_linear_list = 0;
  int run_binary_array = 0;
  int run_binary_tree = 0;

  printf("LENGTH\tSEARCHES\t");

  //checks which algorithms to run
  for(int i=0; i < strlen(algorithms); i++) {
    if(algorithms[i] == 'a') {
      run_linear_array = 1;
      printf("array\t\t");
    }

    if(algorithms[i] == 'l') {
      run_linear_list = 1;
      printf("list\t\t");
    }

    if(algorithms[i] == 'b') {
      run_binary_array = 1;
      printf("binary\t\t");
    }

    if(algorithms[i] == 't') {
      run_binary_tree = 1;
      printf("tree");
    }
  }

  printf("\n");

  int length = pow_min;

    for (int i = 0; i < max_data_size - min_data_size + 1; i++) { // loops over different data sizes
      printf("%6d\t%8d\t",length,length * 2 * reps);

      if(run_linear_array) {
      // run loops to time linear search in an array
        int* arr = make_evens_array(length);
        clock_t start = clock();

        for (int j = 0; j < reps; j++) { // loops based on repetitions
          for (int k = 0; k < 2*length; k++) { // loop that searches
            linear_array_search(arr, length, k);
          } //inner for
        } //outer for

        clock_t end = clock();
        double total = ((double) end - start) / CLOCKS_PER_SEC;
        printf("%.4e\t",total);
        //printf("%.4f\t",total);
        free(arr);
    } //LINEAR ARRAY

    if(run_linear_list) {
    // run loops to time linear search in a list
        list_t* list = make_evens_list(length);
        clock_t start = clock();

        for (int j = 0; j < reps; j++) { // loops based on repetitions
          for (int k = 0; k < 2*length; k++) { // loop that searches
            linkedlist_search(list, length, k);
          } //inner for
        } //outer for

        clock_t end = clock();
        double total = ((double) end - start) / CLOCKS_PER_SEC;
        printf("%.4e\t",total);
        //printf("%.4f\t",total);

        list_free(list);
    } //LINEAR LIST

    if(run_binary_array) {
    // run loops to time binary search in an array
        int* arr = make_evens_array(length);
        clock_t start = clock();

        for (int j = 0; j < reps; j++) { // loops based on repetitions
          for (int k = 0; k < 2*length; k++) { // loop that searches
            binary_array_search(arr, length, k);
          } //inner for
        } //outer for

        clock_t end = clock();
        double total = ((double) end - start) / CLOCKS_PER_SEC;
        printf("%.4e\t",total);
        //printf("%.4f\t",total);

        free(arr);
    } //BINARY ARRAY

    if(run_binary_tree) {
    // run loops to time binary search in a tree
        bst_t* tree = make_evens_tree(length);
        clock_t start = clock();

        for (int j = 0; j < reps; j++) { // loops based on repetitions
          for (int k = 0; k < 2*length; k++) { // loop that searches
            binary_tree_search(tree, length, k);
          } //inner for
        } //outer for

        clock_t end = clock();
        double total = ((double) end - start) / CLOCKS_PER_SEC;
        printf("%.4e\t",total);
        //printf("%.4f\t",total);

        bst_free(tree);
    } //BINARY TREE

    length *= 2;
    printf("\n");

  }

  return 0;

}

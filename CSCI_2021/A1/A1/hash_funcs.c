#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// hash_funcs.c: utility functions for operating on hash maps. Most
// functions are used in hash_main.c which provides an application to
// work with the functions.

// PROVIDED: Compute a simple hash code for the given character
// string. The code is "computed" by casting the first 8 characters of
// the string to a long and returning it. The empty string has hash
// code 0. If the string is a single character, this will be the ASCII
// code for the character (e.g. "A" hashes to 65).  Longer strings
// will have numbers which are the integer interpretation of up to
// their first 8 bytes.  ADVANTAGE: constant time to compute the hash
// code. DISADVANTAGE: poor distribution for long strings; all strings
// with same first 8 chars hash to the same location.
long hashcode(char key[]) {
  union {
      char str[8];
      long num;
    } strnum;
    strnum.num = 0;

    for(int i=0; i<8; i++){
      if(key[i] == '\0'){
        break;
      }
      strnum.str[i] = key[i];
    }
    return strnum.num;
}

// Initialize the hash map 'hm' to have given size and item_count
// 0. Ensures that the 'table' field is initialized to an array of
// size 'table_size' and filled with NULLs.
void hashmap_init(hashmap_t *hm, int table_size) {
  hm->item_count = 0;
  hm->table_size = table_size;
  hm->table = malloc(table_size*sizeof(hashnode_t)); //allocating memory for array

  for (int i = 0; i < table_size; i++) { //ensuring table is filled with NULL
    hm->table[i] = NULL;
  }//for
}

// Adds given key/val to the hash map. 'hashcode(key) modulo
// table_size' is used to calculate the position to insert the
// key/val.  Searches the entire list at the insertion location for
// the given key. If key is not present, a new node is added. If key
// is already present, the current value is altered in place to the
// given value "val" (no duplicate keys are every introduced).  If new
// nodes are added, increments field "item_count".  Makes use of
// standard string.h functions like strcmp() to compare strings and
// strcpy() to copy strings. Lists in the hash map are arbitrarily
// ordered (not sorted); new items are always appended to the end of
// the list.  Returns 1 if a new node is added (new key) and 0 if an
// existing key has its value modified.
int hashmap_put(hashmap_t *hm, char key[], char val[]) {
  int index = hashcode(key) % hm->table_size; //calculating index
  hashnode_t *list = hm->table[index];
  hashnode_t *newNode = malloc(sizeof(hashnode_t));

  if (list == NULL) { //create new node if NULL
    strcpy(newNode->key, key);
    strcpy(newNode->val, val);
    newNode->next = NULL;

    hm->table[index] = newNode;
    hm->item_count++;
    return 1;
  }

  int found = 0; //not found
  while (found == 0 && list != NULL) { //!found, iterating through list
    if (strcmp(list->key, key) == 0) {
      strcpy(list->val, val); //changing old val to new val, no duplicates
      found = 1;
      printf("Overwriting previous key/val\n");
      return 0;
    }//if
    list = list->next;
  }//while

  hashnode_t *last = hm->table[index];
  while (last->next != NULL) { //finding last node
    last = last->next;
  }//while

  //no existing key, so new node created
  //appends new node to last node
  if (found == 0) {
    strcpy(newNode->key, key);
    strcpy(newNode->val, val);
    newNode->next = NULL;
    last->next = newNode;
    hm->item_count++;
    return 1;
  }//if
return 0;
}

// Looks up value associated with given key in the hashmap. Uses
// hashcode() and field "table_size" to determine which index in table
// to search.  Iterates through the list at that index using strcmp()
// to check for matching key. If found, returns a pointer to the
// associated value.  Otherwise returns NULL to indicate no associated
// key is present.
char *hashmap_get(hashmap_t *hm, char key[]) {
  int index = hashcode(key) % hm->table_size; //calculating index
  hashnode_t *list = hm->table[index];
  int found = 0; //not found
  char *valPointer = "not working"; //valPointer initialized

  while (found == 0 && list != NULL) {//iterating through list
    if (strcmp(list->key,key) == 0) {
      valPointer = list->val; //stores corresponding value in valPointer
      found = 1;
    }//if
    list = list->next;
  }//while

  if (found == 0) { //key entered does not exist
    return NULL;
  }
  return valPointer;
}

// De-allocates the hashmap's "table" field. Iterates through the
// "table" array and its lists de-allocating all nodes present
// there. Subsequently = hm;de-allocates the "table" field and sets all
// fields to 0 / NULL. Does NOT attempt to free 'hm' as it may be
// stack allocated.
void hashmap_free_table(hashmap_t *hm) {
  hashnode_t *list;
  for (int i = 0; i < hm->table_size; i++) { //iterating through array
    list = hm->table[i];
    hashnode_t *temp;
    while(temp != NULL) { //iterating through list
      temp = list;
      free(temp); //freeing each node pointer
      list = list->next;
    }//while
  }//for
  free(hm->table); //freeing the array of node pointers

  //clearing fields of hm
  hm->item_count = 0;
  hm->table_size = 0;
  hm->table = NULL;
}

// Displays detailed structure of the hash map. Shows stats for the
// hash map as below including the load factor (item count divided
// by table_size) to 4 digits of accuracy.  Then shows each table
// array index ("bucket") on its own line with the linked list of
// key/value pairs on the same line. The hash code for keys is appears
// prior to each key.  EXAMPLE:
//
// item_count: 6
// table_size: 5
// load_factor: 1.2000
//   0 : {(65) A : 1}
//   1 :
//   2 : {(122) z : 3} {(26212) df : 6}
//   3 : {(98) b : 2} {(25443) cc : 5}
//   4 : {(74) J : 4}
//
// NOTES:
// - Uses format specifier "%3d : " to print the table indices
// - Shows the following information for each key/val pair
//   {(25443) cc : 5}
//     |      |    |
//     |      |    +-> value
//     |      +-> key
//     +-> hashcode("cc"), print using format "%ld" for 64-bit longs
void hashmap_show_structure(hashmap_t *hm) {
  //STATS
  printf("item_count: %d\n", hm->item_count);
  printf("table_size: %d\n", hm->table_size);
  if (hm->table_size == 0)
    printf("load factor cannot be computed, table_size is 0\n");
  else {
    float load_factor = ((float)hm->item_count)/hm->table_size;
    printf("load_factor: %.4f\n", load_factor);
  }

  //TABLE
  for (int i = 0; i < hm->table_size; i++) {
    printf("%3d",i);
    printf("%3c", ':');
    hashnode_t *list = hm->table[i];
    long code;
    char key[128];
    char val[128];

    if (list == NULL)
      printf(" ");
    else {
      while (list != NULL) { //iterating through list
        code = hashcode(list->key);
        strcpy(key, list->key);
        strcpy(val, list->val);
        printf("%3c(%ld) %3s%3s%3s}", '{', code, key, " : ", val);
        list = list->next;
      }//while
    }//else
    printf("\n");
  }//for
}

void hashmap_print(hashmap_t *hm) { //own function to print
  char key[128];
  char val[128];
  for (int i = 0; i < hm->table_size; i++) { //iterating through array
    hashnode_t *list = hm->table[i];
    while(list != NULL) { //iterating through list
      strcpy(key, list->key);
      strcpy(val, list->val);
      printf("%12s : %s\n", key, val);
      list = list->next;
    }//while
  }//for
}

// Outputs all elements of the hash table according to the order they
// appear in "table". The format is
//
//       Peach : 3.75
//      Banana : 0.89
//  Clementine : 2.95
// DragonFruit : 10.65
//       Apple : 2.25
//
// with each key/val on a separate line. The format specifier
//   "%12s : %s\n"
//
// is used to achieve the correct spacing. Output is done to the file
// stream 'out' which is standard out for printing to the screen or an
// open file stream for writing to a file as in hashmap_save().
void hashmap_write_items(hashmap_t *hm, FILE *out) {
  for (int i = 0; i < hm->table_size; i++) { //iterating through array
    hashnode_t *list = hm->table[i];
    while(list != NULL) { //iterating through list
      fprintf(out, "%12s : %s\n", list->key, list->val);
      list = list->next;
    }//while
  }//for
}

// Writes the given hash map to the given 'filename' so that it can be
// loaded later.  Opens the file and writes its 'table_size' and
// 'item_count' to the file. Then uses the hashmap_write_items()
// function to output all items in the hash map into the file.
// EXAMPLE FILE:
//
// 5 6
//            A : 2
//            Z : 2
//            B : 3
//            R : 6
//           TI : 89
//            T : 7
//
// First two numbers are the 'table_size' and 'item_count' field and
// remaining text are key/val pairs.
void hashmap_save(hashmap_t *hm, char *filename) {
  FILE *file = fopen(filename, "w");

  //in case file cannot be created
  if(file == NULL) {
    printf("ERROR: could not create file %s\n", filename);
    return;
  }

  int size = hm->table_size;
  int items = hm->item_count;

  //writing table_ize and item_count
  fprintf(file, "%d ", size);
  fprintf(file, "%d\n", items);

  hashmap_write_items(hm, file);

  fclose(file);
}

// Loads a hash map file created with hashmap_save(). If the file
// cannot be opened, prints the message
//
// ERROR: could not open file 'somefile.hm'
//
// and returns 0 without changing anything. Otherwise clears out the
// current hash map 'hm', initializes a new one based on the size
// present in the file, and adds all elements to the hash map. Returns
// 0 on successful loading. This function does no error checking of
// the contents of the file so if they are corrupted, it may cause an
// application to crash or loop infinitely.
int hashmap_load(hashmap_t *hm, char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
      printf("ERROR: could not open file %s\n", filename);
      return 0;
    }//if

    int size, items;
    char key[128];
    char val[128];
    hashmap_free_table(hm); //clears out current hashmap hm

    fscanf(file, "%d", &size); //scans size of array
    fscanf(file, "%d", &items); //scans item count
    hashmap_init(hm, size); //initializes hash map to have array 'size'

    while (fscanf(file, "%12s : %s\n", key, val) != EOF) { //scans through rest of the file
      hashmap_put(hm, key, val); //adds node with scanned key-value pair to correct index in array
    }//while

  if (items == hm->item_count) {//compares file item count and new hash map's item count
    fclose(file);
    return 0;
  }
  fclose(file);
  return 1; //if fails
}

// If 'num' is a prime number, returns 'num'. Otherwise, returns the
// first prime that is larger than 'num'. Uses a simple algorithm to
// calculate primeness: check if any number between 2 and (num/2)
// divide num. If none do, it is prime. If not, tries next odd number
// above num. Loops this approach until a prime number is located and
// returns this. Used to ensure that hash table_size stays prime which
// theoretically distributes elements better among the array indices
// of the table.
int next_prime(int num) {
  int isPrime = 1; //prime
  while (isPrime == 1) {
    for (int i = 2; i <= num/2; i++) { //iterating from 2 to half of number
      if (num % i == 0) //checking if num is a multiple of i
        isPrime = 0; //not prime
    }//inner for
    if (isPrime == 0) {
      num++; //increments number to be checked
      isPrime = 1; //set back to 1, so can continue trying to find next prime number
    }//if
    else {
      return num;
    }//else
  }//outer while
  return 0; //if goes wrong
}

// Allocates a new, larger area of memory for the "table" field and
// re-adds all items currently in the hash table to it. The size of
// the new table is next_prime(2*table_size+1) which keeps the size
// prime.  After allocating the new table, all entries are initialized
// to NULL then the old table is iterated through and all items are
// added to the new table according to their hash code. The memory for
// the old table is de-allocated and the new table assigned to the
// hashmap fields "table" and "table_size".  This function increases
// "table_size" while keeping "item_count" the same thereby reducing
// the load of the hash table. Ensures that all memory associated with
// the old table is free()'d (linked nodes and array). Cleverly makes
// use of existing functions like hashmap_init(), hashmap_put(),
// and hashmap_free_table() to avoid re-writing algorithms
// implemented in those functions.
void hashmap_expand(hashmap_t *hm) {
  hashmap_t hm2; //temporary hash map
  hashnode_t *list;
  int oldSize = hm->table_size;

  int size = next_prime(2*oldSize+1); //size of new table
  hashmap_init(&hm2, size); //allocates memory for larger array

  for (int i = 0; i < oldSize; i++) { //iterates through old array
    list = hm->table[i];
    while(list != NULL) {//iterates through old list
      hashmap_put(&hm2, list->key, list->val); //adds node to new hashmap
      list = list->next;
    }//while
  }//for
  hashmap_free_table(hm); //de-allocates memory for old hash map

  //uses temp hashmap's data to reset original hashmap's fields
  hm->table = hm2.table;
  hm->table_size = hm2.table_size;
  hm->item_count = hm2.item_count;
}

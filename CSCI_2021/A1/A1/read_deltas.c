// Reads integers in text delta format from the file named by fname
// and returns an array of them. The first integer in the file gives
// the starting point and subsequent integers are changes from the
// previous total.
//
// Opens the file with fopen() and scans through its contents using
// fscanf() counting how many text integers exist in it.  Then
// allocates an array of appropriate size using malloc(). Uses
// rewind() to go back to the beginning of the file then reads
// integers into the allocated array. Closes the file after reading
// all ints.  Returns a pointer to the allocated array.
//
// The argument len is a pointer to an integer which is set to the
// length of the array that is allocated by the function.
//
// If the file cannot be opened with fopen() or if there are no
// integers in the file, sets len to -1 and returns NULL.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int *read_text_deltas(char *fname, int *len) {
  FILE *pFile;
  int count, data; //data is a temporary variable to store contents of fscanf
  pFile = fopen(fname, "r");

  //checking if file was opened properly
  if(pFile == NULL) {
    *len = -1;
    return NULL;
  }

  count = 0; //number of text integers in file

  //counting number of integers in file
  while(fscanf(pFile, "%d", &data) != EOF) {
    count++;
  }

  if (count == 0) { //checking for empty file
    *len = -1;
    fclose(pFile);
    return NULL;
  }

  *len = count; //dereferencing len
  int *fileData = malloc(count*sizeof(int)); //allocating for array that holds 'count' number of integers
  rewind(pFile);
  count = 0;

  while( count < *len ) {
    fscanf(pFile, "%d", &fileData[count]); //first number
    if (count != 0) {
      int delta = fileData[count];
      fileData[count] = fileData[count-1] + delta; //performing operations on subsequent numbers
    }
    count++;
  }

  fclose(pFile);

  return fileData;
}

// Reads integers in binary delta format from the file named by fname
// and returns an array of them.  The first integer in the file gives
// the starting point and subsequent integers are changes from the
// previous total.
//
// Integers in the file are in binary format so the size of the file
// in bytes indicates the quantity of integers. Uses the stat() system
// call to determine the file size in bytes which then allows an array
// of appropriate size to be allocated. DOES NOT scan through the file
// to count its size as this is not needed.
//
// Opens the file with fopen() and uses repeated calls to fread() to
// read binary integers into the allocated array. Does delta
// computations as integers are read. Closes the file after reading
// all ints.
//
// The argument len is a pointer to an integer which is set to
// the length of the array that is allocated by the function.
//
// If the file cannot be opened with fopen() or file is smaller than
// the size of 1 int, sets len to -1 and returns NULL.

int *read_int_deltas(char *fname, int *len) {
  struct stat sb; //struct to hold
  int result = stat(fname, &sb); //unix system call to determine size of named file
  if(result==-1 || sb.st_size < sizeof(int)){ //if something went wrong or bail if file is too small
    *len = -1;
    return NULL;
  }
  int total_bytes = sb.st_size; //size of file in bytes
  int size = total_bytes/sizeof(int); //number of integers
  *len = size; //dereferencing len

  int *fileData = malloc(size*sizeof(int)); //allocating for array that holds 'size' number of integers
  int index = 0;

  FILE *pFile;
  pFile = fopen(fname, "r");

  while (index < size) {
    fread(&fileData[index], sizeof(int), 1, pFile); //first number
    if (index != 0) {
      int delta = fileData[index];
      fileData[index] = fileData[index-1] + delta; //performing operations on subsequent numbers
    }
    index++;
  }

  fclose(pFile);

  return fileData;
}

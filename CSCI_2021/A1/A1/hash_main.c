#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"
//#include "hash_funcs.c"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

  printf("Hashmap Demo\n");
  printf("Commands:\n");
  printf("  hashcode <key>   : prints out the numeric hash code for the given key (does not change the hash map)\n");
  printf("  put <key> <val>  : inserts the given key/val into the hash map, overwrites existing values if present\n");
  printf("  get <key>        : prints the value associated with the given key or NOT FOUND\n");
  printf("  print            : shows contents of the hashmap ordered by how they appear in the table\n");
  printf("  structure        : prints detailed structure of the hash map\n");
  printf("  clear            : reinitializes hash map to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash map the given file\n");
  printf("  load <file>      : clears the current hash map and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hashmap to reduce its load factor\n");
  printf("  quit             : exit the program\n");

  char cmd[128];
  hashmap_t hash;
  int success;
  hashmap_init(&hash, 5); //initializes hashmap, chose 5 for example

  while(1){
    printf("HM> ");                   // prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    //SERIES OF COMMANDS IN IF-ELSE STATEMENTS
    //strcmp used to check which command user inputs
    //fscanf used to scan commands and necessary input for functionality
    if( strcmp("hashcode", cmd)==0 ){   //hashcode command
      fscanf(stdin,"%s",cmd);
      if(echo){
        printf("hashcode %s\n", cmd);
      }
      printf("%ld\n",hashcode(cmd));
    }

    else if( strcmp("put", cmd)==0 ){   //put command
      fscanf(stdin,"%s",cmd);
      char key[128];
      strcpy(key, cmd);
      fscanf(stdin,"%s",cmd);
      char val[128];
      strcpy(val, cmd);
      if(echo){
        printf("put %s %s\n", key, val);
      }
      hashmap_put(&hash, key, val);
    }

    else if( strcmp("get", cmd)==0 ){   //get command
      fscanf(stdin,"%s", cmd);
      if(echo){
        printf("get %s\n",cmd);
      }
      char* result = hashmap_get(&hash, cmd);
      if (result != NULL)
        printf("FOUND: %s\n", result);
      else
        printf("NOT FOUND\n");
    }

    else if( strcmp("structure", cmd)==0 ){   //structure command
      if(echo){
        printf("structure\n");
      }
      hashmap_show_structure(&hash);
    }

    else if( strcmp("next_prime", cmd)==0 ){    //next_prime command
      int num;
      fscanf(stdin,"%d", &num);
      if(echo){
        printf("next_prime %d\n",num);
      }
      printf("%d\n", next_prime(num));
    }

    else if( strcmp("clear", cmd)==0 ){   //clear command
      if(echo){
        printf("clear\n");
      }
      hashmap_free_table(&hash);
      hashmap_init(&hash, 5); //sets table size back to 5
    }

    else if( strcmp("print", cmd)==0 ){   //print command
      if(echo){
        printf("print\n");
      }
      hashmap_print(&hash);
    }

    else if( strcmp("save", cmd)==0 ){    //save command
      fscanf(stdin,"%s", cmd);
      if(echo){
        printf("save %s\n",cmd);
      }
      hashmap_save(&hash, cmd);
    }

    else if( strcmp("load", cmd)==0 ){    //load command
      fscanf(stdin,"%s", cmd);
      if(echo){
        printf("load %s\n",cmd);
      }
      hashmap_load(&hash, cmd);
    }

    else if( strcmp("expand", cmd)==0 ){    //expand command
      if(echo){
        printf("expand\n");
      }
      hashmap_expand(&hash);
    }

    else if( strcmp("quit", cmd)==0 ){    //quit command
      if(echo){
        printf("quit\n");
      }
      break;
    }

    else{                              // unknown command
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    }
  }//while
  free(hash.table); //de-allocates memory occupied by the hash table initialized earlier in this file

  return 0;
}//main

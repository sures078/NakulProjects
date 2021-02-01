#include "reducer.h"
#include <stdio.h>
#include <string.h>

// create a key value node
finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

// insert or update an key value
finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeFinalDS(finalKeyValueDS *root) {
	if(root == NULL) return;

	finalKeyValueDS *tempNode = root -> next;;
	while (tempNode != NULL){
		free(root);
		root = tempNode;
		tempNode = tempNode -> next;
	}
}

// reduce function
void reduce(char *key) {
	//open the mapper file
	FILE *mapfd;
	mapfd = fopen(key, "r");
	//error check
	if(mapfd == NULL) {
		fprintf(stderr, "Map file not found by reducer\n");
		exit(1);
	}

	int sum = 0;
	int temp;
	char word[50];		//assuming no words are larger than 50 characters

	//parse word and count
	fscanf(mapfd, "%s", word);
	while((temp = fgetc(mapfd)) != EOF) {
		if(isdigit(temp)) {
			sum++;
		}
	}

	//update data structure
	root = insertNewKeyValue(root, word, sum);

	//close file
	fclose(mapfd);
}

// write the contents of the final intermediate structure
// to output/ReduceOut/Reduce_reducerID.txt
//don't forget to freeFinalDS once done
void writeFinalDS(int reducerID) {
	//format file path/name
	char filename[50];
	sprintf(filename, "output/ReduceOut/Reducer_%d.txt", reducerID);

	//open file
	FILE *fd;
	fd = fopen(filename, "w");
	//error check
	if(fd == NULL) {
		fprintf(stderr, "Failed to create reducer file\n");
		exit(1);
	}

	//write data to file
	finalKeyValueDS *tempNode = root;
	while(tempNode != NULL) {
		fprintf(fd, "%s %d\n", tempNode->key, tempNode->value);
		tempNode = tempNode->next;
	}

	//free data structure
	freeFinalDS(root);

	//close file
	fclose(fd);
}

int main(int argc, char *argv[]) {
	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}

	// ###### DO NOT REMOVE ######
	// initialize
	int reducerID = strtol(argv[1], NULL, 10);

	// ###### DO NOT REMOVE ######
	// master will continuously send the word.txt files
	// alloted to the reducer
	char key[MAXKEYSZ];
	//start with empty ds
	root = NULL;
	while(getInterData(key, reducerID))
		reduce(key);

	// You may write this logic. You can somehow store the
	// <key, value> count and write to Reduce_reducerID.txt file
	// So you may delete this function and add your logic
	writeFinalDS(reducerID);

	return 0;
}

#include "mapper.h"

// combined value list corresponding to a word <1,1,1,1....>
valueList *createNewValueListNode(char *value){
	valueList *newNode = (valueList *)malloc (sizeof(valueList));
	strcpy(newNode -> value, value);
	newNode -> next = NULL;
	return newNode;
}

// insert new count to value list
valueList *insertNewValueToList(valueList *root, char *count){
	valueList *tempNode = root;
	if(root == NULL)
		return createNewValueListNode(count);
	while(tempNode -> next != NULL)
		tempNode = tempNode -> next;
	tempNode -> next = createNewValueListNode(count);
	return root;
}

// free value list
void freeValueList(valueList *root) {
	if(root == NULL) return;

	valueList *tempNode = root -> next;;
	while (tempNode != NULL){
		free(root);
		root = tempNode;
		tempNode = tempNode -> next;
	}
}

// create <word, value list>
intermediateDS *createNewInterDSNode(char *word, char *count){
	intermediateDS *newNode = (intermediateDS *)malloc (sizeof(intermediateDS));
	strcpy(newNode -> key, word);
	newNode -> value = NULL;
	newNode -> value = insertNewValueToList(newNode -> value, count);
	newNode -> next = NULL;
	return newNode;
}

// insert or update a <word, value> to intermediate DS
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count){
	intermediateDS *tempNode = root;
	if(root == NULL)
		return createNewInterDSNode(word, count);
	while(tempNode -> next != NULL) {
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value = insertNewValueToList(tempNode -> value, count);
			return root;
		}
		tempNode = tempNode -> next;

	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value = insertNewValueToList(tempNode -> value, count);
	} else {
		tempNode -> next = createNewInterDSNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeInterDS(intermediateDS *root) {
	if(root == NULL) return;

	intermediateDS *tempNode = root -> next;;
	while (tempNode != NULL){
		freeValueList(root -> value);
		free(root);
		root = tempNode;
		tempNode = tempNode -> next;
	}
}

// emit the <key, value> into intermediate DS
void emit(char *key, char *value) {
		ds = insertPairToInterDS(ds, key, value);
}

// maps data to data structure
void map(char *chunkData){
	// getWord retrieves words from chunkData one by one.
	int i = 0;
	char *buffer;
	while ((buffer = getWord(chunkData, &i)) != NULL){ // word stored inside of buffer
		emit(buffer, "1");
	}
}

// write intermediate data to separate word.txt files
// Each file will have only one line : word 1 1 1 1 1 ...
void writeIntermediateDS() {
	intermediateDS *tempDS = ds;

	while (tempDS != NULL) {
		char *word;
		word = tempDS -> key;

		char name[50]; // will contain entire path for .txt file, assuming will fit in 50 characters
		sprintf(name,"%s/%s.txt",mapOutDir,word);

		FILE *f;
		f = fopen(name, "w");
		if (f == NULL) // error checking for opening a file
			exit(1);

		fprintf(f,"%s ",word); // writes key name to file

		valueList *tempVals = tempDS -> value; // value list associated with the key

		//writes each "1" value
		while(tempVals != NULL) {
			fprintf(f,"%s ",tempVals -> value);
			tempVals = tempVals -> next;
		} // inner while

		fprintf(f, "\n");

		tempDS = tempDS -> next;

		fclose(f);
	} // outer while
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Less number of arguments.\n");
		printf("./mapper mapperID\n");
		exit(0);
	}
	// ###### DO NOT REMOVE ######
	mapperID = strtol(argv[1], NULL, 10); //converts mapper ID to long

	// ###### DO NOT REMOVE ######
	// create folder specifically for this mapper in output/MapOut
	// mapOutDir has the path to the folder where the outputs of
	// this mapper should be stored
	mapOutDir = createMapDir(mapperID);

	ds = NULL; // declared in mapper.h

	// ###### DO NOT REMOVE ######
	while(1) {

		// create an array of chunkSize=1024B and intialize all
		// elements with '\0'
		char chunkData[chunkSize + 1]; // +1 for '\0' (chunkSize defined in utils.h)
		memset(chunkData, '\0', chunkSize + 1); // memset is a library function that initializes values

		char *retChunk = getChunkData(mapperID); // retrieves chunks one by one
		if(retChunk == NULL) {
			break;
		}

		strcpy(chunkData, retChunk); // copies into chunkData (destination, source)
		free(retChunk);

		map(chunkData); // completes data structure
	}

	// ###### DO NOT REMOVE ######
	writeIntermediateDS(); // creates all .txt files

	freeInterDS(ds); //frees memory allocated for ds

	return 0;
}

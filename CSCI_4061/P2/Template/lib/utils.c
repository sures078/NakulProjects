#include "utils.h"

//Peter 
char *getChunkData(int mapperID) {
  key_t mapkey = ftok(".", MAPID);

  //opens message queue
  int mid = msgget(mapkey, PERM | IPC_CREAT);   

  if(mid < 0) {
    printf("getChunkData msgget error\n");
    exit(1);
  } //if

  //receives message
  struct mymsg_t message; int result;
  result = msgrcv(mid, &message, MSGSIZE, (long) mapperID, 0);

  if(result < 0) {
    printf("getChunkData msgrcv error\n");
    exit(1);
  } //if

  //checks for end message 
  if(strcmp("I'm done!", message.msg) == 0) 
    return NULL;
  

  char *ret = (char *) malloc(sizeof(char) * (MSGSIZE + 1));	
  memset(ret, '\0', MSGSIZE + 1);
  strcpy(ret, message.msg);
  return ret; 
}



//Morgan 
void sendChunkData(char *inputFile, int nMappers) {
	key_t mapkey = ftok(".",MAPID); 

	//opens mesage queue
	int qid = msgget(mapkey, PERM | IPC_CREAT);

	if (qid < 0) {
		printf("sendChunkData msgget error\n");
		exit(1); 
	} //if

	int fd; 
	fd = open(inputFile, O_RDONLY); //open the file 
	char chunk[chunkSize]; //chunk buffer

	struct mymsg_t chunkMsg; //message to be sent 
	int num = 1; //mapper ID 


	int size_read = 0; 
	struct stat* info = (struct stat*) malloc(sizeof(struct stat)); //stat buffer 
	stat(inputFile, info); //file information stored inside 'info'

	while (size_read < info->st_size) {
		memset(chunk, '\0', chunkSize);
		char tempChunk[chunkSize]; 
		memset(tempChunk, '\0', chunkSize); 
 		int backward = 0; 

 		//case where less than chunkSize+1 bytes left to read 
 		if (info->st_size - size_read < 1024) {
 			read(fd, chunk, info->st_size - size_read);
 			size_read = info->st_size; 
 		} //if
 		else {
 			read(fd, tempChunk, chunkSize); //initially reads to tempChunk 
 			int i = chunkSize - 1; int done = 0; 
 			while (i >= 0 && done == 0) {
	 			if (tempChunk[i] == ' ') {
	 				//copies necessary bytes into chunk
	 				strncpy(chunk, tempChunk, (sizeof(tempChunk) - backward));  
	 				size_read += (sizeof(tempChunk) - backward); 
	 				lseek(fd, 0 - backward, SEEK_CUR); //adjusts position 
	 				done = 1;  
	 			} //if
	 			backward++; 
	 			i--; 
	 		} //inner while  
 	    } //else   

		if (num > nMappers)
			num = 1; 
		memset(chunkMsg.msg, '\0', MSGSIZE); 
	    chunkMsg.mtype = (long) num; 
	    strcpy(chunkMsg.msg, chunk); 


	    //sends each chunk to getChunkData
		if ((msgsnd (qid, &chunkMsg, MSGSIZE, 0)) < 0) {
	       printf("sendChunkData msgsnd error\n");
	       exit(1);
	    }//if
		num++; 
	}//outer while 

  close(fd); //closes file 
  free(info); //frees stat buffer 

  //sends end message to each mapper 
  struct mymsg_t endMsg;
  for (int i = 1; i <= nMappers; i++) {
  	  memset(endMsg.msg, '\0', MSGSIZE);
      endMsg.mtype = (long) i; 
      strcpy(endMsg.msg, "I'm done!"); 

      if ((msgsnd (qid, &endMsg, MSGSIZE, 0)) < 0) {
        printf("sendChunkData msgsnd error\n");
        exit(1);
      } //if
  } //for 

} //sendChunkData



// hash function to divide the list of word.txt files across reducers
// http://www.cse.yorku.ca/~oz/hash.html
int hashFunction(char* key, int reducers){
	unsigned long hash = 0;
    int c;

    while ((c = *key++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

    return (hash % reducers);
}



//Peter
int getInterData(char *key, int reducerID) {
  key_t redkey = ftok(".", REDID);

  //opens message queue
  int mid = msgget(redkey, PERM | IPC_CREAT);   
  if(mid < 0) {
    printf("getInterData msgget error\n");
    exit(1);
  } //if

  //receives message
  struct mymsg_t message; int result;
  result = msgrcv(mid, &message, MSGSIZE, (long) reducerID, 0);

  if(result < 0) {
    printf("getInterData msgrcv error\n");
    exit(1);
  } //if

  //checks for end message 
  if(strcmp("I'm done!", message.msg) == 0) 
    return 0;
  
  strcpy(key, message.msg); //stores path in output parameter
  return 1;
} //getInterData



//Nakul
void shuffle(int nMappers, int nReducers) {
  int qid;
  key_t redkey = ftok(".", REDID);

  //open message queue
  if ((qid = msgget(redkey, PERM | IPC_CREAT)) < 0) {
	printf("shuffle msgget error\n");
	exit(1);
  } //if

  //traversing through each map directory and sending each file path to a reducer

  char path[50]; //will eventually contain "output/MapOut/Map_#"
  memset(path, '\0', 50); 
  char pathOne[] = "output/MapOut";
  strcpy(path, pathOne); 

  //pointer to MapOut directory
  DIR* mapout = opendir(path); 
  if (mapout == NULL) {
    printf("shuffle Unable to open MapOut Directory\n");
    exit(1); 
  } //if

  struct dirent *map; //will contain each entry in MapOut directory 

  while ((map = readdir(mapout)) != NULL) {
  	 if (strcmp(map->d_name, ".") == 0 || strcmp(map->d_name, "..") == 0) {
        continue;
      } //if

	 
     strcat(path, "/"); 
     strcat(path, map->d_name); //output/MapOut/Map_#

     //pointer to each Map_# directory
	 DIR* mapnum = opendir(path); 
	 if (mapnum == NULL) {
	 	printf("shuffle Unable to open Map_Num Directory\n");
	 	exit(1);
	 } //if

     struct dirent *wordfile; //will contain each entry in each Map_# folder 

	 while ((wordfile = readdir(mapnum)) != NULL) {
	 	if (strcmp(wordfile->d_name, ".") == 0 || strcmp(wordfile->d_name, "..") == 0) {
        	continue;
        } //if

		//select reducer using hash function
		int reducerId = hashFunction(wordfile->d_name, nReducers) + 1;

	    //create message and send message
		//message = file path for each word

		char wordpath[50]; //will contain entire path to specific .txt file 
		memset(wordpath, '\0', 50);
		strcpy(wordpath, path);
        strcat(wordpath, "/");
        strcat(wordpath, wordfile->d_name);

        struct mymsg_t shuffleMsg;  
        shuffleMsg.mtype = (long) reducerId; 
        memset(shuffleMsg.msg, '\0', MSGSIZE);
        strcpy(shuffleMsg.msg, wordpath); 

        //sends each path message to getInterData 
        if ((msgsnd (qid, &shuffleMsg, MSGSIZE, 0)) < 0) {
        	printf("shuffle msgsnd error\n");
        	exit(1);
      	   } //if
		} //inner while 
		closedir(mapnum); //closes each Map_# directory
		memset(path, '\0', 50); 
  		strcpy(path, pathOne);
	} //outer while
	closedir(mapout); //closes MapOut directory

  //sends end message to each reducer 
  for (int i = 1; i <= nReducers; i++) {
	  struct mymsg_t endMsg; 
	  endMsg.mtype = (long) i; 
	  memset(endMsg.msg, '\0', MSGSIZE);
	  strcpy(endMsg.msg, "I'm done!"); 

	  if ((msgsnd (qid, &endMsg, MSGSIZE, 0)) < 0) {
	    printf("shuffle msgsnd error\n");
	    exit(1);
	  } //if 
  } //for 

} //shuffle 



// check if the character is valid for a word
int validChar(char c){
	return (tolower(c) >= 'a' && tolower(c) <='z') ||
					(c >= '0' && c <= '9');
}



char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		// printf("%d\n", i);
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	if(strlen(buffer) > 0)
		return buffer;
	return NULL;
}



void createOutputDir(){
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}

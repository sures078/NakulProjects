• The purpose of your program
We implemented the functions shuffle(), getChunkData() and getInterData(), and sendChunkData(). The overall purpose is to read in a file and distribute chunks of size 1024 byes to the mappers in a round robin fashion. After this chunks are distributed each mapper_ID in the map phase will call getChunkData() to recieve those chunks. The shuffle() phase divides the txt files in the output/MapOut/Map_mapperID folders across nReducers and uses the hash function for the reducerID. Once the reducer is set it uses the getInterData to retrieve the file path from shuffle() for the words so it can do the final count in the reduce function. Overall, we uses one message queue to communicate the chunks for the mapping processes and we use another message queue to communicate file paths for the reducing processes

• How to compile the program
We first used cd to make sure that we were going to compile and run the correct programs in the correct directory. We then used 'make' and gcc to make sure we were able to compile the programs such as mapreduce, reducer and mapper. Then we are able to produce a single executable file to run and see the chunks of the input file be put into the MapperID/ReducerID files.

• What exactly your program does
In our utils.h we have our message struct named mymsg_t. It consists of the mapperID/reducerID to tag the messages and is named mtype. And 'msg' is the text that will be sent. We use chunkSize for the chunks sent but all other messages are MSGSIZE which is 1100, as specified in utils.h. To indicate the end of any phase so the process can move forward to the next phase, we use end message "I'm done!". Throughout our functions we use structs to open our message queue and use memset to initialize buffer to '\0'. We use msgget to open the message queue. In sendChunkData we use a while loop to create the chunks of data and then send end messages to each mapper and closes the file it read.  We also use msgrcv() in getChunkData to recieve the message. The shuffle phase opens the message queue and traverses the directory of each Mapper and send the word filepath to the reducer (used strcat and strcpy). After it goes through and creates the reducer ID it sends the message using msgsnd to the reducer and waits/uses the status variable for the END notification. Lastly, the getInterData opens the message queue, receives the message using msgrcv(). We close both the message queues in mapreduce after mapping and reducing processes are done respectively, using msgctl. 


• Any assumptions outside this document
Assumptions outside the write-up should be that we had to use memset to initalize buffer to '\0' and other functions such as strcat and strcpy throughout our program. Also The code that was given for some of the functions in mapper, mapreduce, reducer, and createOutputDir should be assumed that they work properly. Take note we close the queues in mapreduce.c

• Project group name, Team member names, x500
Group 56
Nakul sures078
Peter genat003
Morgan schen272

• Contribution by each member of the team
Shuffle() : Nakul
getChunkData() and getInterData() : Peter
sendChunkData : All
readme : Morgan

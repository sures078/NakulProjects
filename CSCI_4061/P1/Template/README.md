test machine: CSELAB in Keller 4-250
date: 10/07/20
names: Nakul Suresh, Morgan Schenk, Peter Genatempo
x500s: sures078, schen272, genat003

-The purpose of the program-

The purpose of our program takes three functions (Mapreducer, Mapper and Reducer) to process large datasets to go through a process to create a file(s) with a specific Mapper ID that contains the count of specific words from an original text file. The master (Mapreducer) program drives the other two phases and is in charge of sendChunkData() and Shuffle(). After it sends the split files it produces child-like processes and executes the programs to run for the mapper and then later the reducer. It then waits for the two programs to complete their purpose of the process. The purpose of using these three phases is to use the key, value pairs throughout the process to produce an intermediate pair and then merge those pairs to be read through and counted for and finally put into the final text file. In other words, the purpose is to process the initial text file to produce new files with word counts.

-How to compile the program-

We first used cd to make sure that we were going to compile and run the correct programs in the correct directory. For us the file path was csci4061P1/P1/Template/mapper or reducer. We then used 'make' and gcc to make sure we were able to compile the programs and produce a single executable file that would be created in the directory and folders labeled inside the MapOut and ReduceOut folders.

-What exactly the program does-

Our program takes 3 programs (Mapreducer, Mapper and Reducer) to create files with a specific Mapper ID that contains the count of specific words from an original text file. Our Mapreducer program (Master) drives the other phases of the process. It is given an input text file and calls bookeepingCode() to create that output that will be sent to the mapper program. It then splits the given text file into 1024 byte chunks and shares it uniformly with map, shuffle, and reduce. We use two for loops to call the execution of mapper and reducer and use the wait function until the text in the input file is looked over and counted for. The Master then calls shuffle to divide the files containing the pairs from reducer. The mapper has data provided from Mapreduce and uses getWord to release the pair into an intermediate data structure. This intermediate data structure is created and the goal of this phase is to write onto the new text file. We used sprintf to format the output to a string and then proceeded to open and write onto the file using the pointers pointing to the data in tempDS and tempVals. The data written from the mapper file will then be able to be sent to the shuffle and reducer phases. The reducer phase takes in the Reducer ID, which is established when exec is called in mapreduce. It opens the file and reads through the contents and does the final word counts for the contents written in the text file.

-Any assumptions outside the write-up-

Assumptions outside the write-up should be that we had to use malloc and free throughout the phases to make sure we had enough space to write and count the contents in the files. The code that was given for some of the functions like shuffle() and bookeepingCode() should be assumed that they work properly.

-Team members names and x500-

Nakul Suresh (sures078)
Morgan Schenk (schen272)
Peter Genatempo (genat003)

-Contribution-

Nakul Suresh: mapper.c
Morgan Schenk: mapreduce.c + README
Peter Genatempo: reducer.c

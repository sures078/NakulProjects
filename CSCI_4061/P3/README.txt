● Your group ID and each member’s name and X500.

Group ID: Group 56
Peter Genatempo - genat003
Nakul Suresh - sures078
Morgan Schenk - schen272

● How to compile and run your program.

We first use make to compile the program. We run the server by using ./web_server port path along with the arguments that we put in like num_dispatch num_workers and qlen. We can use "cat urls | xargs -n 1 -P 8 wget" to run multiple requests at once. Additionally we use wget which sends the request to the server. Then we are able to put in files ending in different file paths such as html,jpeg, and gif. The program logs the "web_server_log" file each time a request is fulfilled, with [threadId][reqNum][fd][Request string][bytes/error] as the output. 

● A brief explanation on how your program works.

Our program works with two types of threads. These are dispatcher threads and worker threads. Our request_t struct used two variables to show the full file (path) and the other to show the file extension for the request (request). We set up our program with a linked list that contains a head node which points to the first request and an end node which points to the last request in the queue. We use synchronization by using conditional variables/locks throughout our program using pthread_cond_t *full, pthread_cond_t *empty, and pthread_mutex_t file_lock and q_lock. The dispatcher functions receives the request from the client and adds to the queue. We use pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_signal, and pthread_cond_wait to block when neccesary. The worker removes requests from the queue, serves them by return_result or return_error and logs each request to a file called “web_server_log” and to the terminal (stdout). 

● Indicate which extra credit your group implements [Extra credit A/ Extra credit B/ Both/ None]

None.

● Contributions of each team member towards the project development.

Contributions for the project follows. Nakul worked on the function to retrieve the request from the queue (implemented as a linked list), process it and then return a result to the client and the signal handler. Peter worked on the function to get the content type from the request and the main which includes error checking. Morgan worked on the function to receive the request from the client and add to the queue. Peter, Nakul, and Morgan all contributed to working on the dispatcher. 




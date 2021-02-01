Test machine: Vole 
                                                                                           
                        
●  Your group ID and each member’s name and X500.
Group ID: 56
Peter Genatempo: genat003
Nakul Suresh: sures078
Morgan Schenk: schen272  

●  How to compile and run your program.
We first use make to compile the program. We run the server by using ./web_server port path along with the arguments that we put in like num_dispatch num_workers and qlen. We can use "cat urls | xargs -n 1 -P 8 wget" to run multiple requests at once. Additionally we use wget which sends the request to the server. Then we are able to put in files ending in different file paths such as html,jpeg, and gif. We use the “-S” flag on wget to print the server's response, and if we need to test return_error, we use the --content-on-error in wget to receive the error message. 

●  A brief explanation on how your program works.
From the file server.c written in Project 3, our program implements the networking utility functions using POSIX socket programming. We first use init to initialize a socket for receiving client connection requests from the port. When running each dispatch thread, it first needs to accept connection to the new client. If successfully it returns the socket descriptor and the function get_request is called. Get_request receives an HTTP GET request from the client and by using  memset and strncpy, copies the request file path into the file name. If each worker thread has a successful result, it goes into return_result to have a successful working file request be returned back to the client. If not successful, it instead goes into return_error and hands a different file request outline back to the client. On a side note, the main server socket fd is a global variable in utils.h and is closed in main when the server stops running. 
                                                                 
●  Indicate if your group implemented the extra credit.
Our group did not implement the extra credit                
                                                 
●  Contributions of each team member towards the project development.                                                
Init- Morgan
Accept_connection- Morgan/Nakul
Get_request- Nakul
Return_result- Peter
Return_error- All
README- Peter                                                         

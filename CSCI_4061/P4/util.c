#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

//lock for reading and writing 
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

/**********************************************
 * init
   - port is the number of the port you want the server to be
     started on
   - initializes the connection acception/handling system
   - YOU MUST CALL THIS EXACTLY ONCE (not once per thread,
     but exactly one time, in the main thread of your program)
     BEFORE USING ANY OF THE FUNCTIONS BELOW
   - if init encounters any errors, it will call exit().
************************************************/
void init(int port) {
  //creates socket 
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    printf("Socket error");
    exit(1);
  }

  //accounts for potential bind errors
  int enable = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
    printf("Can't set socket option");
    exit(1);
  }

  //struct used to bind 
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //sets up IP address
  my_addr.sin_port = htons(port); //sets up port number

  //binds IP address and port number to socket 
  if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
    perror("Could not bind");
    exit(1);
  }

  //Backlog 20 = how many pending connections queue will hold
  if (listen(sockfd, 20) == -1) { 
    perror("Could not listen");
    exit(1);  
  }
}

/**********************************************
 * accept_connection - takes no parameters
   - returns a file descriptor for further request processing.
     DO NOT use the file descriptor on your own -- use
     get_request() instead.
   - if the return value is negative, the request should be ignored.
***********************************************/
int accept_connection(void) {
   int client_fd;
   struct sockaddr_in client_addr;
   int addr_size = sizeof(client_addr);

   //accepts connection from client and obtains client's fd 
   if ((client_fd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &addr_size)) == -1) {
     perror("Failed to accept connection");
   }
   return client_fd;
}

/**********************************************
 * get_request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        from where you wish to get a request
      - filename is the location of a character buffer in which
        this function should store the requested filename. (Buffer
        should be of size 1024 bytes.)
   - returns 0 on success, nonzero on failure. You must account
     for failures because some connections might send faulty
     requests. This is a recoverable error - you must not exit
     inside the thread that called get_request. After an error, you
     must NOT use a return_request or return_error function for that
     specific 'connection'.
************************************************/
int get_request(int fd, char *filename) { 
  char buf[1024];
  int readsz = 0;
  int i = 0;

  if (pthread_mutex_lock(&file_mutex) < 0) {
   printf("Failed to lock! %s\n", strerror(errno));
   exit(1);
 }
  if ((readsz = read(fd, buf, 1023)) >= 0) { //reads request from client 
    if (pthread_mutex_unlock(&file_mutex) < 0) {
      printf("Failed to unlock! %s\n", strerror(errno));
      exit(1);
    }
    buf[readsz] = '\0';

    //error check for 'GET' request 
    char getCheck[5];
    memset(getCheck, '\0', 5);
    strncpy(getCheck,buf,4); 
    char get[] = "GET ";
    if (strcmp(getCheck, get) != 0) {
      printf("Not a GET request\n"); 
      return -1; 
    }

    strcpy(buf, strchr(buf, '/'));
    while (buf[i] != ' ') {
      //error check for '//'
      if (buf[i] == '/' && i < 1023) {
        if (buf[i+1] == '/') {
          printf("Requested file should not contain '//'\n");
          return -1;  
        }
      }
      //error check for '..'
      if (buf[i] == '.' && i < 1023) {
        if (buf[i+1] == '.') {
          printf("Requested file should not contain '..'\n");
          return -1;  
        }
      }
      filename[i] = buf[i]; //copies only file path from buf 
      i++;
      if (i > 1023) {
        printf("File path longer than 1023 bytes\n"); 
        return -1; 
      }
    }
    filename[i] = '\0';
    printf("REQUEST IS %s\n",filename);
    return 0;
  }
  else {
    if (pthread_mutex_unlock(&file_mutex) < 0) {
      printf("Failed to unlock! %s\n", strerror(errno));
      exit(1);
    }
    perror ("Server read problem\n");
  }
  return -1;
}

/**********************************************
 * return_result
   - returns the contents of a file to the requesting client
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the result of a request
      - content_type is a pointer to a string that indicates the
        type of content being returned. possible types include
        "text/html", "text/plain", "image/gif", "image/jpeg" cor-
        responding to .html, .txt, .gif, .jpg files.
      - buf is a pointer to a memory location where the requested
        file has been read into memory (the heap). return_result
        will use this memory location to return the result to the
        user. (remember to use -D_REENTRANT for CFLAGS.) you may
        safely deallocate the memory after the call to
        return_result (if it will not be cached).
      - numbytes is the number of bytes the file takes up in buf
   - returns 0 on success, nonzero on failure.
************************************************/
int return_result(int fd, char *content_type, char *buf, int numbytes) {
  char result[numbytes+200]; //result to be written to client 
  memset(result,'\0',numbytes+200);
  strcpy(result,"HTTP/1.1 200 OK\n");

  char ct[50]; 
  char cl[50]; 
  memset(ct,'\0',50); 
  memset(cl,'\0',50);
  sprintf(ct, "Content-Type: %s\n", content_type);
  sprintf(cl, "Content-Length: %d\n", numbytes);

  strcat(result,ct); 
  strcat(result,cl);
  strcat(result,"Connection: Close\n\n"); 
  strcat(result,buf); 

  if (pthread_mutex_lock(&file_mutex) < 0) {
   printf("Failed to lock! %s\n", strerror(errno));
   exit(1);
 }
  if (write(fd,result,numbytes+200) <= 0) { 
    printf("Could not write in return_result\n");
    close(fd); 
    if (pthread_mutex_unlock(&file_mutex) < 0) {
      printf("Failed to unlock! %s\n", strerror(errno));
      exit(1);
    }
    return -1; 
  }
  if (pthread_mutex_unlock(&file_mutex) < 0) {
   printf("Failed to unlock! %s\n", strerror(errno));
   exit(1);
 }
  close(fd); 
  return 0;
}

/**********************************************
 * return_error
   - returns an error message in response to a bad request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the error
      - buf is a pointer to the location of the error text
   - returns 0 on success, nonzero on failure.
************************************************/
int return_error(int fd, char *buf) {
  char error[200]; //error to be written to the client 
  memset(error,'\0',200);
  strcpy(error,"HTTP/1.1 404 Not Found\n");

  char ct[50]; 
  memset(ct,'\0',50); 
  sprintf(ct, "Content-Type: %s\n", "content_type");

  strcat(error,ct);
  strcat(error,"Content-Length: 25\n");
  strcat(error,"Connection: Close\n\n");
  strcat(error,buf);
  
  if (pthread_mutex_lock(&file_mutex) < 0) {
   printf("Failed to lock! %s\n", strerror(errno));
   exit(1);
 }
  if (write(fd,error,200) <= 0) { 
    printf("Could not write in return_error\n"); 
    close(fd); 
    if (pthread_mutex_unlock(&file_mutex) < 0) {
      printf("Failed to unlock! %s\n", strerror(errno));
      exit(1);
    }
    return -1; 
  }
  if (pthread_mutex_unlock(&file_mutex) < 0) {
   printf("Failed to unlock! %s\n", strerror(errno));
   exit(1);
 }
  close(fd); 
  return 0;
}

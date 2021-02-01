#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#define MAX_THREADS 100
#define MAX_queue_len 100
#define MAX_CE 100
#define INVALID -1
#define BUFF_SIZE 1024

typedef struct requests {
   int fd;          
   char *request; //specific file path 
   char *path; //entire path 
} request_t;

//used to maintain queue of requests 
typedef struct node {
  request_t req;     
  struct node *next; 
} node_t; 

int q_len; //fixed queue length given by user 
int curr_len; //current number of requests in queue 
node_t *head; //dummy node, points to the first request in queue
node_t *end; //last request in queue for worker
int worker_threads[100]; //array containing # of requests completed by each worker  

pthread_cond_t *full_condition; //condition variable for full queue 
pthread_cond_t *empty_condition; //condition variable for empty queue
pthread_mutex_t *q_lock; //lock dealing with adding/removing from queue 
pthread_mutex_t *file_lock; //lock dealing with reading from/writing to a file 

// typedef struct cache_entry {
//     int len;
//     char *request;
//     char *content;
// } cache_entry_t;

/* ******************** Dynamic Pool Code  [Extra Credit A] **********************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
//void * dynamic_pool_size_update(void *arg) {
  //while(1) {
    // Run at regular intervals
    // Increase / decrease dynamically based on your policy
  //}
//}
/**********************************************************************************/

/* ************************ Cache Code [Extra Credit B] **************************/

// Function to check whether the given request is present in cache
//int getCacheIndex(char *request){
  /// return the index if the request is present in the cache
//}

// Function to add the request and its file content into the cache
//void addIntoCache(char *mybuf, char *memory , int memory_size){
  // It should add the request at an index according to the cache replacement policy
  // Make sure to allocate/free memory when adding or replacing cache entries
//}

// clear the memory allocated to the cache
//void deleteCache(){
  // De-allocate/free the cache memory
//}

// Function to initialize the cache
//void initCache(){
  // Allocating memory and initializing the cache array
//}

/**********************************************************************************/

/************************************* Utilities **********************************/

//adds a request to the queue 
void addNode(node_t *newNode) {
  end->next = newNode; 
  end = newNode; 
  curr_len++;
}

//removes a request from the queue 
node_t *removeNode() {
  node_t *remNode = head->next; //node to be removed from the front 
  if (curr_len == 1) {
     end = head;
     head->next = NULL; //request queue is now empty 
  } else {
     head->next = remNode->next; //setting new start node
  }
  curr_len--;
  return remNode;
} 

//Peter
//gets the content type from the request
char *getContentType(char * mybuf) {
  char *extension = strchr(mybuf, '.'); //returns pointer to '.' 
  char *content;
  if(strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0) {
    content = "text/html";
  } else if(strcmp(extension, ".jpg") == 0) {
    content = "image/jpeg";
  } else if(strcmp(extension, ".gif") == 0) {
    content = "image/gif";
  } else {
    content = "text/plain";
  }
  return content;
}

//Morgan 
//opens and reads the file from disk into memory
int readFromDisk(char* path, void* buf, int filesize) {
    //buf is an output parameter the file will read into 
    int fd = open(path, O_RDONLY);
    if (fd == -1){
      return -1;
    }
    int result = read(fd, buf, filesize);
    if(result < 0) {
      close(fd);
      return -1;
    }
    close(fd);
    return 0; 
}

/**********************************************************************************/

//Morgan 
//receives the request from the client and adds to the queue
void *dispatch(void *givenPath) {
  //givenPath is a path given from the user to a specific part of the file system 
  while (1) {
    //accepts client connection
    int fd = accept_connection();
    
    //gets request from the client
    char filename[BUFF_SIZE];
    memset(filename, '\0', BUFF_SIZE);
    int status = get_request(fd, filename);
    
    //initializing entire path to request 
    char path[1024];
    memset(path, '\0', 1024);
    strcat(path, (char*) givenPath);
    strcat(path, filename);

    //sets request parameters 
    request_t req;
    req.fd = fd;
    req.request = filename; //unique extension 
    req.path = path; //path necessary for worker 
    
    //adding request into the queue
    if(status == 0 && fd >= 0) {
      node_t *newNode = (node_t *) malloc(sizeof(node_t));
      newNode->req = req;
      newNode->next = NULL;
      
      //CRITICAL SECTION  
      if(pthread_mutex_lock(q_lock) != 0) {
         printf("Lock error\n");
         return NULL;
      }
      while(curr_len >= q_len) { //cannot add request if queue is full 
        if(pthread_cond_wait(full_condition, q_lock) != 0) {
           printf("Cond_Wait error\n");
           return NULL;
        }
      }
      addNode(newNode);
      if(pthread_cond_signal(empty_condition) != 0) { //request added, signals thread waiting to remove
         printf("Cond_Signal error\n");
         return NULL;
      } 
      if(pthread_mutex_unlock(q_lock) != 0) {
         printf("Unlock error\n");
         return NULL;
      }
      //CRITICAL SECTION 
    }
  }
  return NULL;
}

/**********************************************************************************/

//Nakul 
//retrieves request from queue, processes it and returns result to client
void *worker(void *thread_place) {
   //thread_place is a pointer to a place in worker_threads array 
   //worker_threads containts the # of requests completed for each thread 
   while (1) {
    //CRITICAL SECTION 
    if(pthread_mutex_lock(q_lock) != 0) {
       printf("Lock error\n");
       return NULL;
    }
    while(curr_len <= 0) { //cannot remove request if queue is empty 
      if(pthread_cond_wait(empty_condition, q_lock) != 0) {
         printf("Cond_Wait error\n");
         return NULL;
      }
    }
    node_t* info = removeNode();
    if(pthread_cond_signal(full_condition) != 0) { //request removed, signals thread waiting to add 
       printf("Cond_Signal error\n");
       return NULL;
    }
    if(pthread_mutex_unlock(q_lock) != 0) {
       printf("Unlock error\n");
       return NULL;
    }
    //CRITICAL SECTION 
      
    if(info == NULL) {
      free(info); 
      return NULL;
    }
     
    //info needed for readFromDisk   
    request_t req = info->req; //the request to process 
    free(info); 
    char *path = req.path;
    struct stat *stat_buf = (struct stat*) malloc(sizeof(struct stat));
    stat(path, stat_buf);  
    int filesize = stat_buf->st_size; 
    free(stat_buf); 
    char *content = getContentType(path);
    void *file_buf = (void *) malloc(filesize); //pointer to contents of request file 
    
    //CRITICAL SECTION 
    //gets the data from disk
    if(pthread_mutex_lock(file_lock) != 0) {
       printf("Lock error\n");
       return NULL;
    }
    int result = readFromDisk(path, file_buf, filesize); 
    if(pthread_mutex_unlock(file_lock) != 0) {
       printf("Unlock error\n");
       return NULL;
    }
    //CRITICAL SECTION 
    
    //info needed to log request into log file and terminal
    char write_buf[2000];
    memset(write_buf, '\0', 2000);
    int thread_id = (int) pthread_self();
    int *i = (int*) thread_place;
    int num_reqs = ++(*i);

    //return the result
    if(result == -1) {
      int returnCheck = return_error(req.fd, file_buf);
      free(file_buf); 
      if(returnCheck != 0) {
        return NULL;
      }
      sprintf(write_buf, "[%d][%d][%d][%s][Request file not found.]\n", thread_id, num_reqs, req.fd, req.request); 
    } 
    else {
      int returnCheck = return_result(req.fd, content, file_buf, filesize);
      free(file_buf); 
      if(returnCheck != 0) {
        return NULL;
      }
      sprintf(write_buf, "[%d][%d][%d][%s][%d]\n", thread_id, num_reqs, req.fd, req.request, filesize);
    }
      
    int size = 0; //# of characters in write_buf to display 
    while(write_buf[size] != '\0') {
      size++;
    }
      
    //CRITICAL SECTION 
    if(pthread_mutex_lock(file_lock) != 0) {
       printf("Lock error\n");
       return NULL;
    }
    write(log_file_fd, write_buf, size); //updates log file 
    write(1, write_buf, size); //prints to terminal 
    if(pthread_mutex_unlock(file_lock) != 0) {
       printf("Unlock error\n");
       return NULL;
    }
    //CRITICAL SECTION 
  }
  return NULL;
}

/**********************************************************************************/

//Nakul 
//handles ^C signal: freeing + closing log file 
void handle_int(int signo) {
  //CRITICAL SECTION 
  if(pthread_mutex_lock(q_lock) != 0) {
     printf("Lock error\n");
     exit(1);
  }
  for(int i = 0; i < curr_len; i++) { //freeing remaining request nodes 
    node_t *temp = head->next;
    head->next = temp->next;
    free(temp);
  }
  free(head);
  if(pthread_mutex_unlock(q_lock) != 0) {
     printf("Unlock error\n");
     exit(1);
  }
  //CRITICAL SECTION 
  
  printf("\nNumber of pending requests in the request queue: %d\n", curr_len);
   
  //CRITICAL SECTION 
  if(pthread_mutex_lock(file_lock) != 0) {
     printf("Lock error\n");
     exit(1);
  }
  close(log_file_fd);
  if(pthread_mutex_unlock(file_lock) != 0) {
     printf("Unlock error\n");
     exit(1);
  }
  //CRITICAL SECTION 
   
  free(full_condition); 
  free(empty_condition);
  free(q_lock); 
  free(file_lock); 
   
  exit(0);
}

//Peter 
int main(int argc, char **argv) {
  //error check on # of arguments
  if(argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }

  //get input args
  int port = atoi(argv[1]);
  char *path = argv[2];
  int num_dispatcher = atoi(argv[3]);
  int num_workers = atoi(argv[4]);
  //don't worry about dynamic_flags
  q_len = atoi(argv[6]);
  //don't worry about cache_size

  //error checks on input arguments
  if(port < 1025 || port > 65535) {
    printf("Invalid port number\n");
    return -1;
  }
  if(num_dispatcher <= 0 || num_dispatcher > 100) {
    printf("Invalid number of dispatcher threads\n");
    return -1;
  }
  if(num_workers <= 0 || num_workers > 100) {
    printf("Invalid number of worker threads\n");
    return -1;
  }
  if(q_len <= 0 || q_len > MAX_queue_len) {
    printf("Invalid queue length\n");
    return -1;
  } else { //initialize queue 
    head = (node_t *) malloc(sizeof(node_t)); 
    head->next = NULL;
    end = head;
    curr_len = 0;
  }

  //changes SIGINT's action for graceful termination
  signal(SIGINT, handle_int); //prints # of pending requests + closes log file 

  //opens log file
  log_file_fd = open("web_server_log.txt", O_WRONLY | O_TRUNC | O_CREAT);
  if(log_file_fd < 0) {
    printf("Error opening log file\n");
    return -1;
  }

  //changes current working directory to server root directory
  if(chdir(path) < 0) {
    printf("Error changing working directory to server root\n");
    return -1;
  }

  //initialize cache (extra credit B)

  //start the server
  init(port);
  
  //initializes locks/condition variables 
  full_condition = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  empty_condition = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
  q_lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  file_lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
  pthread_cond_init(full_condition, NULL);
  pthread_cond_init(empty_condition, NULL);
  pthread_mutex_init(q_lock, NULL);
  pthread_mutex_init(file_lock, NULL);

  //creates dispatcher threads (all threads are detachable)
  int i;
  int error_check1;
  int error_check2;
  for(i = 0; i < num_dispatcher; i++) {
    pthread_t t1;
    error_check1 = pthread_create(&t1, NULL, dispatch, (void *) path);
    error_check2 = pthread_detach(t1);
    if(error_check1 != 0 || error_check2 != 0) {
       printf("Thread creation failed\n");
       return -1;
    }
  }
  
  //creates worker threads (all threads are detachable)
  memset(worker_threads, 0, num_workers);
  for(i = 0; i < num_workers; i++) {
    pthread_t t2;
    error_check1 = pthread_create(&t2, NULL, worker, (void *) &worker_threads[i]);
    error_check2 = pthread_detach(t2);
    if(error_check1 != 0 || error_check2 != 0) {
       printf("Thread creation failed\n");
       return -1;
    }
  }

  //creates dynamic pool manager thread (extra credit A)
   
  while(1); //ensures server keeps running until ^C 
   
  //removes cache (extra credit B)

  //terminates server gracefully
  raise(SIGINT);

  return 0;
}

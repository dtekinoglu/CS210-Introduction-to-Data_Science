#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150
int remainingMemory = 150;

struct node
{
	int id;
	int size;
};

queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size
int charValues[NUM_THREADS];





void my_malloc(int thread_id, int size)
{
    pthread_mutex_lock(&sharedLock);
	node newRequest;
	newRequest.id = thread_id;
	newRequest.size = size;
	myqueue.push(newRequest); //pushing request to the queue.
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void *)
{
    while(true){

        if (myqueue.size()!= 0) {
            pthread_mutex_lock(&sharedLock);
            node next;
            next = myqueue.front();
            myqueue.pop(); // get the request from queue.
            if (next.size > remainingMemory){ //check if there is enough memory.
                thread_message[next.id] = -1; // if not, send -1.
            }
        
            else{
                thread_message[next.id] = MEMORY_SIZE - remainingMemory; //send start address.
                remainingMemory-=next.size; //update remaning memory.
            }
            sem_t * sem = &semlist[next.id];
            sem_post(sem); //unblock the thread.
        	pthread_mutex_unlock(&sharedLock);
        }
        
    }
}

void * thread_function(void * id) 
{
	int size = rand()% MEMORY_SIZE/6 +1; //create a random int.
	int * idPointer = (int *)id;

	my_malloc(*idPointer,size); //push the request to the queue.
    sem_t * sem =  &semlist[*idPointer]; 


	sem_wait(&semlist[*idPointer]); //block yourself.
	pthread_mutex_lock(&sharedLock);
	

    if (thread_message[*idPointer] == -1) { //check your message
        cout <<"There is not enough memory for thread "<< *idPointer << endl;
    }
    
    else{
        int start = thread_message[*idPointer]; //update memory array.
        for (int i = 0 ; i < size; i++) {
          memory[start+i] = charValues[*idPointer];}
    }
    pthread_mutex_unlock(&sharedLock);


}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
  	for (int i = 0; i < NUM_THREADS; i++)	//initialize memory 
  	{char zero = '0'; charValues[i]= zero+i;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
    pthread_mutex_lock(&sharedLock);
	for (int i = 0; i < MEMORY_SIZE; i++)
		cout << memory[i] << " ";
	cout << endl;
	pthread_mutex_unlock(&sharedLock);
}

int main (int argc, char *argv[])
 {
    srand(time(NULL));
    pthread_t threadID[NUM_THREADS]; //thread aray.
    init();	// call init
    for (int i = 0; i < NUM_THREADS; i++){ //create and join threads.
        pthread_t newThread;
        pthread_create(&newThread, NULL, thread_function, (void*)&(i));
        threadID[i]  = newThread;
        pthread_join(threadID[i],NULL);

    }

    pthread_cancel(server); //terminate server when all threads are finished.



 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
   return 0;
 }
 
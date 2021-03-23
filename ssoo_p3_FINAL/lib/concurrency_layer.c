
#include "../include/concurrency_layer.h"


	//Producer-Consumer

	pthread_mutex_t queue_mutex;
	pthread_cond_t non_full;
	pthread_cond_t non_empty;
	//Writers-Readers
	pthread_mutex_t flagReader_mutex;
	int flagReaders=0;
	
	
	/*Auxiliary function to check whether the flag has been changed
		or not.
	Recieves the address of the exit variable and the mutex in charge
		of protecting it, returns the value of flag at the moment of reading it.
	Change the number of readers, if there is a least one
		blocks the exit mutex, this is considered also a critical
		section as more than 1 thread can try to change the value 
		of flagReaders at the same time, but doesnt block them from 
		reading the value of flag concurrently, when there are no more 
		readers unlock exit mutex*/
	int* checkFlag(int* flagDir ,pthread_mutex_t exit_mutex ){
		
		pthread_mutex_lock(&flagReader_mutex);
   		flagReaders++;
        if(flagReaders==1) pthread_mutex_lock(&exit_mutex);
        pthread_mutex_unlock(&flagReader_mutex);
            int* flag = flagDir;
        pthread_mutex_lock(&flagReader_mutex);
        flagReaders--;
        if(flagReaders==0) pthread_mutex_unlock(&exit_mutex);
        pthread_mutex_unlock(&flagReader_mutex);
			return flag;
	}	
	
  
void init_concurrency_mechanisms(){

	//Initialize all the mutex
	
	if(pthread_mutex_init(&queue_mutex,NULL)!=0){
		perror("queue mutex init");
	}
	if(pthread_mutex_init(&flagReader_mutex,NULL)!=0){
	    perror("flagReader mutex init");
	}
	if(pthread_cond_init(&non_full,NULL)!=0){
	    perror("non full cond init");
	}
	if(pthread_cond_init(&non_empty,NULL)!=0){
	    perror("non empty cond init");
	}


}

void destroy_concurrency_mechanisms(){

	//Destroy all the mutex

	if((pthread_mutex_destroy(&queue_mutex))!=0){
		perror("queue mutex destroy");	
	}
	if((pthread_mutex_destroy(&flagReader_mutex))!=0){
		perror("flag reader mutex destroy");	
	}
	if((pthread_cond_destroy(&non_full))!=0){
		perror("non full cond destroy");	
	}
	if((pthread_cond_destroy(&non_empty))!=0){
		perror("non empty cond destroy");	
	}
	
}
/*Function that is in charge of putting operations in the queue, there can be more
	than one running at the same time
Recives a structure as input and return nothing*/
void* broker(void* args){
	//Gather the info from the arguments
    broker_info* info = (broker_info*)args;
    char batch_file[256];
    memcpy(batch_file, info->batch_file, 256);
    stock_market* market = info->market;
    //Create a new iterator
    
    iterator* iteratorN= new_iterator(batch_file);
    char id[11];
		int type;
		int num_shares;
		int price;
		operation op1;
	//While there are operations in the iterator this will go on
	
	while(next_operation(iteratorN,id,&type,&num_shares,&price) >= 0){
					
	        new_operation(&op1,id,type,num_shares,price); //Generate data
	        //Block queue,check that the queue is not full , if so wait for signal and unlock 
	        //queue until is not full anymore
	        pthread_mutex_lock(&queue_mutex);
	        while(operations_queue_full(market->stock_operations)==1) {
	        	pthread_cond_wait(&non_full,&queue_mutex);
	        }
	        //Work 
	        printf("id: %s, type: %d, num_shares: %d, price: %d\n",id,type,num_shares,price);
	        enqueue_operation(market->stock_operations, &op1);
	        //Signals that the queue cannot be empty after enqueue an operation
	        pthread_cond_signal(&non_empty);
	        //Unlock
	        pthread_mutex_unlock(&queue_mutex);
	}
	destroy_iterator(iteratorN); 
	pthread_exit(NULL);
}
/*Function that is in charge running the operations in the queue, there can be more
	only one thread running this process
Recives a structure as input and return nothing*/

void* operation_executer(void * args){
	//Gather the information from the arguments
    exec_info* info = (exec_info*)args;
    stock_market* market = info->market;
    pthread_mutex_t exit_mutex = *info->exit_mutex;
    operation op1;
    //While the flag is not active or the queue is not empty
    while(*checkFlag(info->exit,exit_mutex)==0||
    !(operations_queue_empty(market->stock_operations)))  {
    	//Blocks queue and checks is not empty if so wait for signal and unlock
    	//queue until is not empty anymore
		pthread_mutex_lock(&queue_mutex);
        while(operations_queue_empty(market->stock_operations)==1) {
        	pthread_cond_wait(&non_empty,&queue_mutex);
        }
        //Work
       		dequeue_operation(market->stock_operations,&op1);
      		process_operation(market,&op1);
      	//Signals that the queue cannot be full after processing an operation
        pthread_cond_signal(&non_full);
        pthread_mutex_unlock(&queue_mutex);        	
    }	
    pthread_exit(NULL);
}

/*Function that is in charge printing the state of the market, there can be more
	than one running at the same time
Recives a structure as input and return nothing*/

void* stats_reader(void *args){
	//Gathers the information from the arguments
	reader_info* info = (reader_info*)args;
	stock_market * market =info->market;
	pthread_mutex_t exit_mutex = *info->exit_mutex;
	unsigned int frequency =info->frequency;
	//While the flag is not active
	while(*checkFlag(info->exit,exit_mutex)==0)  {
		//Blocks queue to read the status of the market
		//Unblocks when done
      	pthread_mutex_lock(&queue_mutex);
        	print_market_status(market);
        pthread_mutex_unlock(&queue_mutex);
        //Once finished goes to sleep a determined time
        usleep(frequency);
        
        
            
    }
	pthread_exit(NULL);
}



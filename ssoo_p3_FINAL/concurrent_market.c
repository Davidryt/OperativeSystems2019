#include "../include/concurrency_layer.h"
#include <stdio.h>
#include <pthread.h>
int main(int argc, char * argv[]){

	//There can be n threads and n readers in thsi program and one operations
	//executer, these parameters can be modified as you want as long they are
	//bigger than 0 for the brokers and 0 and above for the readers.
	int numberBrokers =1;
	int numberReaders =0;
	
	//Initialize k threads
	int totalThreads = numberBrokers+numberReaders+1;
	pthread_t tid[totalThreads];
	//Initialize the market
	stock_market market_madrid;
	//Creat the control for the flag
	pthread_mutex_t exit_mutex;
	int exit = 0;
	

	// Init market and concurrency mechanisms
	init_market(&market_madrid, "stocks.txt");

	init_concurrency_mechanisms();
	
	if(pthread_mutex_init(&exit_mutex,NULL)!=0){
		perror("exit mutex init");
	}
	
	
	//Create structures and create threads

	// Init broker_info structure for the broker thread
	// all the brokers recive the same batch operation in this loop
	// so they all will repeat the same operations, in order to change
	// this , when initializating the structure you can change the 
	//the batch_operations file for another file with batch_opertions
	broker_info info_broker;
	strcpy(info_broker.batch_file, "batch_operations.txt");
	info_broker.market = &market_madrid;
	for(int ii = 0;ii<numberBrokers;ii++){
		pthread_create(&(tid[ii]), NULL, &broker, (void*) &info_broker);
	}


	
	// Init exec_info structure for the operation_executer thread
	exec_info info_ex1;
	info_ex1.market = &market_madrid;
	info_ex1.exit = &exit;
	info_ex1.exit_mutex = &exit_mutex;
	pthread_create(&(tid[numberBrokers]), NULL, &operation_executer, (void*) &info_ex1);
	
	// Init reader_info for the stats_reader thread
	reader_info info_re1;
	info_re1.market = &market_madrid;
	info_re1.exit = &exit;
	info_re1.exit_mutex = &exit_mutex;	
	info_re1.frequency = 100000;
	for(int ii = 0;ii<numberReaders;ii++){
		pthread_create(&(tid[numberBrokers+1+ii]), NULL, &stats_reader, (void*) &info_re1);
	}
				
	
	

	// Join broker threads
	void * res;
	for(int ii = 0;ii<numberBrokers;ii++){
	
			if(pthread_join(tid[ii],&res)!=0){
				perror("join");
			}
	}

	
	// Put exit flag = 1 after brokers completion
	pthread_mutex_lock(&exit_mutex);
	exit = 1;
	pthread_mutex_unlock(&exit_mutex);
	
	// Join the rest of the threads
	
		for(int ii = numberBrokers;ii<totalThreads;ii++){
			if(pthread_join(tid[ii],&res)!=0){
				perror("join");
		}
	}
	
	// Print final statistics of the market
	print_market_status(&market_madrid);
	
	// Destroy market and concurrency mechanisms
	pthread_mutex_destroy(&exit_mutex);
	delete_market(&market_madrid);
	destroy_concurrency_mechanisms();
	
	return 0;
}

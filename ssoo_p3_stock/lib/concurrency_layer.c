
#include "../include/concurrency_layer.h"
#include "../include/parser.h"
#include "../include/stock_market_lib.h"

	operation_queue qu1;

void init_concurrency_mechanisms(){
	pthread_mutex_t opqueue_mutex;
    pthread_mutex_t reader_mutex;
	if(pthread_mutex_init(&opqueue_mutex,NULL)!=0){
		perror();
	}
	if(pthread_mutex_init(&reader_mutex,NULL)!=0){
		perror();
	}
	if(pthread_mutex_init(&exit_mutex),NULL)!=0){
	    perror();
	}
}

void destroy_concurrency_mechanisms(){
	if(pthread_mutex_destroy(&opqueue_mutex)!=0){
		perror();	
	}
	if(pthread_mutex_destroy(&reader_mutex)!=0){
		perror();	
	if(pthread_mutex_destroy(&exit_mutex)!=0){
		perror();	
	}	}
}
void* broker(broker_info* infox){
    
    char batch_file[256] = infox.batchfile;
    stock_market market = infox.market;
    //Comprobar que existe la ID??
    if(iterator* iteratorN= new_iterator(batch_file)==NULL){
        perror();
    }
    char id[11];
	int type;
	int num_shares;
	double price;
	int max_items=10;
	operation op1;
	qu1 =new_operations_queue(max_items);
	while(next_operation(iteratorN,id,&type,&num_shares,&price) >= 0){
	    printf("id: %s, type: %d, num_shares: %d, price: %lf\n",id,type,num_shares,price);
	    pthread_mutex_lock(&opqueue_mutex);
	        new_operation(&op1,id,type,num_shares,price);
	        enqueue_operation(&qu1, &qp1);
	        if(operations_queue_full(&qu1)==1){
	            while(operations_queue_full(&qu1)==1){
	                sleep(0.5);
	            }
	        }
	   pthread_mutex_unlock(&opqueue_mutex);
	   }
	   
	pthread_exit();
}


void* operation_executer(exec_info * args){
    
    stock_market = orgs.market;
    pthread_mutex_t cand1 = args.exit_mutex;
    phtread_mutex_lock(&cand1);
        int exitinfo = args.exit; 
    phtread_mutex_unlock(&cand1);
    operation op1;
    while(exitinfo==0)  {
        if(!operations_queue_empty(&qu1)){
            pthread_mutex_lock(&opqueue_mutex);
                   dequeue_operation(&qu1,&op1);
        pthread_mutex_lock(&reader_mutex);
            process_operation(&stock_market,&op1);
        pthread_mutex_unlock(&reader_mutex);
        pthread_mutex_unlock(&opqueue_mutex); 
            
        }
    }
    while(!operations_queue_empty(&qu1)){
        dequeue_operation(&qu1m,&op1);
            pthread_mutex_lock(&reader_mutex);
            process_operation(&stock_market,&op1);
        pthread_mutex_unlock(&reader_mutex);
    }
	pthread_exit();
    

}

void* stats_reader(reader_info *args){
     stock_market * market =args.market
     pthread_mutex_t *cand1 = args.exit_mutex
     unsigned int frequency =args.frequency
     phtread_mutex_lock(&cand1);
        int exitinfo = args.exit; 
    phtread_mutex_unlock(&cand1);
while(exitinfo==0)  {
        if(!operations_queue_empty(&qu1)){
            pthread_mutex_lock(&opqueue_mutex);
            print_market_status(&market);
            pthread_mutex_lock(&opqueue_mutex);
            usleep(frequency);
        }
    
    }



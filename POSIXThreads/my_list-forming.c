/*
 *   list-forming.c: 
 *     Each thread generates a data node, attaches it to a global list. This is reapeated for K times.
 *     There are num_threads threads. The value of "num_threads" is input by the student.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sched.h>

#define K 200  // genreate a data node for K times in each thread

struct Node
{
	int data;
        struct Node* next;
};

struct list
{
	struct Node * header;
        struct Node * tail;
};

pthread_mutex_t mutex_lock;

struct list *List;

struct Node* generate_data_node()
{
	struct Node *ptr;
        ptr = (struct Node *)malloc(sizeof(struct Node));    

	if( NULL != ptr ){
		ptr->next = NULL;
	}
	        else {
		        printf("Node allocation failed!\n");
		}
	return ptr;
}

void * producer_thread( void *arg)
{
        struct Node * ptr, tmp;
	struct list localList;
        localList.header = localList.tail = NULL;

	int counter = 0;  

	/* generate and attach K nodes to a local list */
        /* The local list is in a non-critical region and doesn't require a  lock */
	while( counter  < K )
	{
	        ptr = generate_data_node();
                ptr->data = 1; 	//generate data
	        if( localList.header == NULL ) //If the list is empty
	        {
			localList.header = localList.tail = ptr;
		}
		else{
			localList.tail->next = ptr;
			localList.tail = ptr;
		}
	counter ++;
	}

	/* Lock the global list then add the local nodes */
	pthread_mutex_lock(&mutex_lock);
	if ( List->header == NULL ){ 	//If the global list is empty
 		List->header = localList.header;
		List->tail = localList.tail;
	}
	else{
		List->tail->next = localList.header;
		List->tail = localList.tail;
	}
	pthread_mutex_unlock(&mutex_lock);
}


int main(int argc, char* argv[])
{
	int i, num_threads;

        int NUM_PROCS;//number of CPU
	int* cpu_array = NULL;

	struct Node  *tmp,*next;
	struct timeval starttime, endtime;

	if(argc == 1){
		printf("ERROR: please provide an input arg (the number of threads)\n");
	        exit(1);
	}
	num_threads = atoi(argv[1]); //read num_threads from user
        pthread_t producer[num_threads];
	NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF);//get number of CPU
	if( NUM_PROCS > 0)
	{
		cpu_array = (int *)malloc(NUM_PROCS*sizeof(int));
	        if( cpu_array == NULL )
	        {
		        printf("Allocation failed!\n");
		        exit(0);
		}
		else
		{
			for( i = 0; i < NUM_PROCS; i++)
			        cpu_array[i] = i;
		}
	}

	pthread_mutex_init(&mutex_lock, NULL);
	
	List = (struct list *)malloc(sizeof(struct list));
	if( NULL == List )
        {
 	       printf("End here\n");
	       exit(0);	
	}
	
	List->header = List->tail = NULL;
	
	gettimeofday(&starttime,NULL); //get program start time
	for( i = 0; i < num_threads; i++ )
	{
		pthread_create(&(producer[i]), NULL, (void *) producer_thread, &cpu_array[i%NUM_PROCS]); 
	}

	for( i = 0; i < num_threads; i++ )
	{
		if(producer[i] != 0)
		{
			pthread_join(producer[i],NULL);
        	}
	}

	gettimeofday(&endtime,NULL); //get the finish time

	if( List->header != NULL )
	{
		next = tmp = List->header;
	        while( tmp != NULL )
		{  
			next = tmp->next;
		        free(tmp);
		        tmp = next;
		}            
	}
	
	if( cpu_array!= NULL)
 	       free(cpu_array);
	/* calculate program runtime */
	printf("Total run time is %ld microseconds.\n", (endtime.tv_sec-starttime.tv_sec) * 1000000+(endtime.tv_usec-starttime.tv_usec));
        return 0; 
}

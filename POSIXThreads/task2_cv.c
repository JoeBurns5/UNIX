/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 2  : Project 2 = POSIX Thread Programming 
Composed By: Joseph Burns
Dated      : 12 October 2018
File Name  : task2_cv.c
\*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "task2_cv.h"

int main(int argc, char *argv[])
{
	int counter;       //Loop Counter
        int rc;            //Thread Return Code
	long tid1, tid2;   //Thread IDs
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;

	/* Initialize mutex and condition variables */
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&con_var, NULL);

	/* Initialize thread attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Create threads */	
	pthread_create(&threads[0], &attr, producer, (void *)tid1);
	pthread_create(&threads[1], &attr, consumer, (void *)tid2);

	/* Wait for all threads to complete */
	for (counter = 0; counter < NTHREADS; counter++)
	{
		pthread_join(threads[counter], NULL);
	}

	/*Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&con_var);
	pthread_exit (NULL);
}

/*---------------------------------------------------------------------------*\
 * Producer
 *---------------------------------------------------------------------------*/
void* producer(void *t)
{
	int temp_ptr = 0;  //Working pointer
	int cbyte;         //Characte byte
	eof_flag = FALSE;

	if ((fp=fopen("message.txt", "r"))==NULL){
		printf("ERROR: Can't open message.txt!\n");
		eof_flag = TRUE;
		return 0;
	}
	
	while(1)
	{
		pthread_mutex_lock(&mutex);           //Protect producer Variables
		pthread_cond_wait(&con_var, &mutex);  //Wait for signal from consumer

		temp_ptr = head_ptr + 1;
		if (temp_ptr > SIZE - 1)  //If reached end of buffer,
			temp_ptr = 0;     //cirlce back to the beginning

		if (temp_ptr != tail_ptr)  //If temp = tail a byte needs to be
                                           //consumed.
		{
			cbyte = fgetc(fp); //Read a byte
			if (!feof(fp))
			{
				buffer[head_ptr] = (char) cbyte;
				head_ptr = temp_ptr;
			}
			else
			{
				eof_flag = TRUE;
				fclose(fp);
			}
		}

		pthread_mutex_unlock(&mutex);

		if (eof_flag == TRUE)
			break;
	}
	pthread_exit(NULL);
}

/*---------------------------------------------------------------------------*\
 * Consumer 
 *---------------------------------------------------------------------------*/
void* consumer(void *t)
{
	int temp_ptr;  //Working pointer
	while(1)
	{
		pthread_mutex_lock(&mutex);    //Protect consumer variables
		pthread_cond_signal(&con_var); //Wake up producer
		if (head_ptr != tail_ptr)      //If head = tail, a byte needs 
                                               //to be produced.
		{
			printf("%c", buffer[tail_ptr]);
			temp_ptr = tail_ptr + 1;
			if (temp_ptr > SIZE - 1)  //If reached end of buffer,
				temp_ptr = 0;     //cirlce back to the beginning
			tail_ptr = temp_ptr;
		}
		pthread_mutex_unlock(&mutex);

		if (head_ptr == tail_ptr && eof_flag == TRUE)
			break;
	}
	printf("\n");
	pthread_exit(NULL);
}
	


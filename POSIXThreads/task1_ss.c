/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 2  : Project 2 = POSIX Thread Programming 
Composed By: Joseph Burns
Dated      : 12 October 2018
File Name  : task1_ss.c
\*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "task1_ss.h"

int main(int argc, char *argv[])
{
	long counter;  //Loop counter
	int rc;        //pthread return code

	/* Get String and Substring from file */
	readf(fp);
        
        printf("\nString = %sSubstring = %s\n", s1, s2);
	
	/* Declare Thread Variables */
	pthread_t WorkerThreads[NTHREADS];
	pthread_attr_t attr;
	pthread_mutex_init(&mutex, NULL);

	/* Initialize Theads */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex, NULL);

	/*Create Threads */
	for(counter = 0; counter<NTHREADS; counter++)
	{
		printf("Main: Creating thread %d\n", counter, NTHREADS);
		rc = pthread_create(&WorkerThreads[counter], &attr, WorkerThread, (void *)counter);
		if (rc)
		{
			printf("ERROR: Return Code from pthread_create() is %d.\n", rc);
			exit(-1);
		}
	}

	/*Wait for all the threads to Finish */
	for(counter = 0; counter<NTHREADS; counter++)
	{
		rc = pthread_join(WorkerThreads[counter], NULL);
		if (rc)
		{
			printf("ERROR: Return Code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	
	printf("\nTotal substrings found = %d\n", total);

	/*Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
        free(s1);
	free(s2);
	pthread_exit(NULL);	
}
//---------------------------------------------------------------------------

int readf(FILE *fp)

//Creates two buffers S1 and S2 then reads two lines representing a string 
//and search string from string.txt into the buffers. Finally the lenghth of 
//the strings are calculated and assinged to the variables n1 and n2.
//
{
	if ((fp=fopen("string.txt", "r"))==NULL){
		printf("ERROR: Can't open string.txt!\n");
		return 0;
	}
	
	s1=(char *)malloc(sizeof(char)*BUFSIZE);
	if (s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}

	s2=(char *)malloc(sizeof(char)*BUFSIZE);
        if (s2==NULL){
		printf("ERROR: Out of memory!\n");
	        return -1;
	}

        /*read s1 and s2 from the file*/
        s1=fgets(s1, BUFSIZE, fp);
        s2=fgets(s2, BUFSIZE, fp);
        n1=strlen(s1)-1;  /*length of s1*/
        n2=strlen(s2)-1;  /*length of s2*/

	if (s1==NULL || s2==NULL || n1<n2)
		return -1;
}
//---------------------------------------------------------------------------

int num_substring(long startPos)

//Calculates the number of times the string s2 appears in the string s1.

{
      	int i,j,k;
	int step = NTHREADS;
	int count;
	int numSubStrings = 0;

	for (i = startPos; i <= (n1-n2); i+=step){   
		count=0;
                printf("%d ", i);
		for(j = i,k = 0; k < n2; j++,k++){  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k)){
				break;
			}
			else
				count++;

			if(count==n2)
				numSubStrings++;   /*Found a substring */
		}
	}
	return numSubStrings;
}
//---------------------------------------------------------------------------

void* WorkerThread(void *val)

//Calculates the total number of substrings found from all the running threads.

{
	long startPos = (long)val;
	int count = 0;

	printf("Thread: %ld Searching String Positions: ", startPos);
        count = num_substring(startPos);
        printf("Found %d substrings.\n", count);

	pthread_mutex_lock(&mutex);
		total += count;			
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

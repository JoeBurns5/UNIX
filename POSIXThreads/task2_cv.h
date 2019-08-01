/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 2  : Project 2 = POSIX Thread Programming 
Composed By: Joseph Burns
Dated      : 12 October 2018
File Name  : task2_cv.h
\*---------------------------------------------------------------------------*/

/* Macros */
#define NTHREADS 2
#define SIZE 5
#define TRUE 1
#define FALSE 0

/* Global Variables */

pthread_mutex_t mutex;    //Thread Mutex
pthread_cond_t  con_var;  //Thread Condition Variable

char buffer[SIZE];        //Five Byte Buffer
int head_ptr = 0;         //Buffer head pointer
int tail_ptr = 0;         //Pointer to Next Character in the Buffer
int eof_flag = FALSE;     //End of File Flag
FILE *fp;                 //File descriptor for message input file

/*Function Prototypes */
void* consumer(void *);
void* producer(void *);




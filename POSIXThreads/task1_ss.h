/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 2  : Project 2 = POSIX Thread Programming 
Composed By: Joseph Burns
Dated      : 12 October 2018
File Name  : task1_ss.h
\*---------------------------------------------------------------------------*/

/* Macros */
#define BUFSIZE 1024
#define NTHREADS 4
#define TRUE 1
#define FALSE 0

/* Global Variables */
pthread_mutex_t mutex;
int total = 0;  /* Total Number of substrings found */
int n1, n2;     /* Lengths of String & Substring */
char *s1, *s2;  /* Character buffer pointers for String & SubString */
FILE *fp;       /* File descriptor for String input file */

/* Function Prototypes */
int readf(FILE *fp);
int num_substring(long);
void* WorkerThread(void *);

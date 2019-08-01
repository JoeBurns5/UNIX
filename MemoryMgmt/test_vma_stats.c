/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 3  : Memory Management
Composed By: Joseph Burns
Dated      : 11 November 2018
File Name  : test_vma_stats.c
\*---------------------------------------------------------------------------*/

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define _NR_vma_stats 340
#define TRUE 1
#define FALSE 0

//Function Prototypes
int is_valid_PID( char* number );

int main(int argc, char *argv[]){
    
    int pid; //Process ID from command line argument
    
    if (argc < 2) {
        printf("Missing Argument!  Usage: test_vma_stats [PID]\n");
        exit(-1);
    } else if (argc > 2) {
          printf("Too Many Arguments! Usage: test_vma_stats [PID]\n");
          exit(-1);
    } else if (!is_valid_PID(argv[1])) {
          printf("Invalid PID! PID must be a positive integer!\n");
          exit(-1);
    } else {

      pid = atoi(argv[1]);
    }  

    syscall(_NR_vma_stats, pid);

    printf("Syscall %d complete! To view output please view the kernel log file.\nUse: [dmesg | tail] from the command prompt.\n", _NR_vma_stats);

    return 0;
}

int is_valid_PID(char* number) {
    
    int i = 0;
 
    for (; number[i] != 0; i++){
        if (!isdigit(number[i]))
            return FALSE;
    }

    return TRUE;

}

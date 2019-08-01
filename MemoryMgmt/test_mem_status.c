/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 3  : Memory Management
Composed By: Joseph Burns
Dated      : 11 November 2018
File Name  : test_mem_status.c
\*---------------------------------------------------------------------------*/

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define _NR_mem_status 341
#define TRUE 1
#define FALSE 0

//Function Prototypes
int is_valid_ARG( char* number );

int main(int argc, char *argv[]){
    
    unsigned long address;  //Process Address 
    int pid;                //Process ID 
    
    if (argc < 3) {
        printf("Missing Argument!  Usage: test_mem_status [ADDRESS] [PID]\n");
        exit(-1);
    } else if (argc > 3) {
          printf("Too Many Arguments! Usage: test_mem_status [ADDRESS] [PID]\n");
          exit(-1);
    } else if (!is_valid_ARG(argv[1]) || !is_valid_ARG(argv[2])) {
          printf("Invalid argument. ADDRESS and PID must be positive integers!\n");
          exit(-1);
    } else {

          address = strtoul(argv[1],NULL,0);
          pid = atoi(argv[2]);
    } 

    syscall(_NR_mem_status, address,  pid);

    printf("Syscall %d complete! To view output please view the kernel log file.\nUse: [dmesg | tail] from the command prompt.\n", _NR_mem_status);

    return 0;
}

int is_valid_ARG(char* number) {
    
    int i = 0;
 
    for (; number[i] != 0; i++){
        if (!isdigit(number[i]))
            return FALSE;
    }

    return TRUE;

}

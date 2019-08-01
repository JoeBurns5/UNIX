/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 1  : System Calls and Processes
Composed By: Joseph Burns
Dated      : 12 September 2018
File Name  : test_syscall.c 
\*---------------------------------------------------------------------------*/

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define _NR_helloworld 337

int main(int argc, char *argv[]){
    
    syscall(_NR_helloworld);
    
    return 0;
}

/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 1  : System Calls and Processes
Composed By: Joseph Burns
Dated      : 12 September 2018
File Name  : test_print_self.c 
\*---------------------------------------------------------------------------*/

#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define _NR_print_self 338

int main(int argc, char *argv[]){
    
    syscall(_NR_print_self);

    printf("Sycall %d complete! To view output please view the kernel log file.\nUse: [dmesg | tail] from the command prompt.\n", _NR_print_self);

    return 0;
}

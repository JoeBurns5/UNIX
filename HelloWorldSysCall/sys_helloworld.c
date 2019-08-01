/*---------------------------------------------------------------------------*\
 Operating Systems I - CS4500 SEC001
 Project 1  : System Calls and Processes 
 Composed By: Joseph Burns
 Dated      : 12 September 2018 
 File Name  : sys_helloworld.c
\*---------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_helloworld (void)
{
    printk(KERN_EMERG "Hello World!\n");
    return 0;
}

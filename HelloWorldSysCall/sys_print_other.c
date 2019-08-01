/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 1  : System Calls and Processes
Composed By: Joseph Burns
Dated      : 12 September 2018
File Name  : sys_print_other.c_
\*---------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>

//Function Prototypes
char* get_process_state_string(int);//

asmlinkage int sys_print_other(int p_id)
{
    struct pid *pid_struct;    //Process Structure
    struct task_struct *task;  //Process Descriptor
    
    pid_struct = find_get_pid(p_id);  //Get Process Structure for p_id
    task = pid_task(pid_struct, PIDTYPE_PID); //Pointer to Process Descriptor

    printk(KERN_INFO "Current Process ID: [%d]\n", task->pid);
    printk(KERN_INFO "Current Process State: [%s]\n", get_process_state_string(task->state));
    printk(KERN_INFO "Current Process Name: [%s]\n\n", task->comm);
    printk(KERN_INFO "Process Hierarchy from Current Process to init()...\n\n");

    for(;task!=&init_task;task=task->parent)
    {
        printk(KERN_INFO "Name: [%s] ID: [%d] State: [%s]\n", task->comm, task->pid, get_process_state_string(task->state));
    }     

    return 0;
}

char* get_process_state_string(int state)
{

    char *process_state;

    switch(state)
    {
        case 0:
            process_state = "TASK_RUNNING";
            break;
        case 1:
            process_state = "TASK_INTERRUPTIBLE";
            break;
        case 2:
            process_state = "TASK_UNINTERRUPTIBLE";
            break;
        case 4:
             process_state = "TASK_ZOMBIE";
             break;
        case 8:
             process_state = "TASK_STOPPED";
              break;
         default:
             process_state = "UNKNOWN";
    }

    return process_state;

}

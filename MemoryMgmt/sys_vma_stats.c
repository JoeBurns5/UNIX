/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 3  : Memory Management 
Composed By: Joseph Burns
Dated      : 11 November 2018
File Name  : sys_vma_stats.c
\*---------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm_types.h>

//Function Prototypes
char* get_process_state(int);

asmlinkage int sys_vma_stats(int p_id)
{
    struct pid *pid_struct;      //Process Structure
    struct task_struct *task;    //Process Descriptor
    struct vm_area_struct *vma;  //Process Virtual Memory Areas

    int total_vmas = 0;      //Total number of the processes VMAs
    int count = 1;           //VMA number
    int total_vma_size = 0;  //Total Process VMA size
    int vma_size = 0;        //Individual VMA size
    
    pid_struct = find_get_pid(p_id);          //Get Process Structure for p_id
    task = pid_task(pid_struct, PIDTYPE_PID); //Pointer to Process Descriptor
    vma = task->mm->mmap;                     //Pointer to first VMA
    total_vmas = task->mm->map_count;         //Total number of VMAs
   
    printk("Current Process ID: [%i]\n", task->pid);
    printk("Current Process State: [%s]\n", get_process_state(task->state));
    printk("Current Process Name: [%s]\n", task->comm);
    printk("--------------------------------------------------------------------------------");
    printk("\nProcess [%s] has %i Virtual Memory Areas (VMA).\n", task->comm, total_vmas);

    for (; vma; vma = vma->vm_next)
    {
        vma_size = vma->vm_end - vma->vm_start;        

        printk("\nVMA #%i\n", count);
        printk("Starting Address: [0x%lx]\n", vma->vm_start);
        printk("Ending Address: [0x%lx]\n", vma->vm_end);
        printk("VMA Size: [%i bytes]\n",vma_size );

        total_vma_size += vma_size;
        count ++;
    }

    printk("\nTotal VMA Size for process [%s]: %i bytes\n\n", task->comm, total_vma_size);

    return 0;
}

char* get_process_state(int state)
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

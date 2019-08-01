/*---------------------------------------------------------------------------*\
Operating Systems I - CS4500 SEC001
Project 3  : Memory Management 
Composed By: Joseph Burns
Dated      : 11 November 2018
File Name  : sys_mem_status.c
\*---------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm_types.h>
#include <linux/highmem.h>

#define TRUE 1
#define FALSE 0

//Function Prototypes
char* get_state(int);

asmlinkage int sys_mem_status(unsigned long address, int p_id)
{
    struct pid *pid_struct;      //Process Structure
    struct task_struct *task;    //Process Descriptor
    struct mm_struct *mem;       //Memory Descriptor

    pgd_t *pgd;                  //Page Global Directory
    pud_t *pud;                  //Page Upper Directory
    pmd_t *pmd;                  //Page Middle Directory
    pte_t *ptep, pte;            //Page Table Entry

    spinlock_t *lock;            //Mutex lock used to get the PTE

    int status = 0;              //Address Physical Memory/Disk Status Variable
    int is_Valid_Mem = TRUE;     //Address validity boolean variable

    pid_struct = find_get_pid(p_id);          //Get Process Structure for p_id
    task = pid_task(pid_struct, PIDTYPE_PID); //Pointer to Process Descriptor
    mem = task->mm;                           //Pointer to Memory Descriptor

    //If the address is valid, get its page table entry. If not, unset validity variable.
    pgd = pgd_offset(mem, address);   
    if (!(pgd_none(*pgd) || pgd_bad(*pgd)))
    {
         pud  = pud_offset(pgd, address);
         if (!(pud_none(*pud) || pud_bad(*pud)))
         {
             pmd = pmd_offset(pud, address);
             if (!(pmd_none(*pmd) || pmd_bad(*pmd)))
             {
                 ptep = pte_offset_map_lock(mem, pmd, address, &lock);
                 pte = *ptep;
                 status = pte_present(pte);
                 pte_unmap_unlock(ptep, lock);
             }
             else
                 is_Valid_Mem = FALSE;
         }
         else
             is_Valid_Mem = FALSE;
    }
    else 
        is_Valid_Mem = FALSE;

    printk("Current Process ID: [%i]\n", task->pid);
    printk("Current Process Name: [%s]\n", task->comm);
    printk("Current Process State: [%s]\n", get_state(task->state));
    printk("Process memory address to be tested: [0x%lx]\n", address);
    printk("--------------------------------------------------------------------------------\n");
    
    if (is_Valid_Mem)
    {
        if (status)
            printk("Virtual memory address [0x%lx] is in physical memory.\n\n", address);
        else
            printk("Virtual memory address [0x%lx] is on disk.\n\n", address);
    }
    else
        printk("Virtual memory address [0x%lx] is invalid.\n\n", address);
 

    return 0;
}

char* get_state(int state)
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/mm.h>
#define DEVICE_NAME "panes_module"

MODULE_LICENSE("GPL");

static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);
int panes_print(char *buffer);                                    

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */
struct cred *cred;

static int __init panes_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Panes module load failed\n");
        return major;
    }

    printk(KERN_INFO "Panes has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");
    //print_process_list();
    return 0;
}

static void __exit panes_exit(void) {
    printk(KERN_INFO "Panes module successfully unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   char *buffer;
   printk(KERN_INFO "Panes module device opened\n");
   panes_print(buffer);
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, Panes is read only\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Panes device closed\n");
   return 0;
}
int imprimio=0;

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    if(imprimio==1)
        return 0;

    errors =panes_print(buffer);
    imprimio=1;
    return errors; //errors== 0 ? message_len : -EFAULT;
}

/*    Init Module    */

int panes_print(char *buffer)                    
{
    char message[255];
    int message_len=0;
    int errors = 0;

    long int rss_parent;
    long int rss_child;
    long int mem_total=0;

    /*    good practice to log when loading/removing modules    */
   printk(KERN_INFO "%s","LOADING MODULE\n");   

    /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
    for_each_process( task ){     

        get_task_struct(task);
        
        if(task->mm){
            rss_parent = get_mm_rss(task->mm) * 4;
            mem_total+= rss_parent;
        }
        else {
            rss_parent = 0;
        }
        
        //UID==0 ROOT ELSE USER
        printk(KERN_INFO "\n PARENT PID: %d PROCESS: %s STATE: %ld  NICE: %d UID: %d MEMORY: %ldkB",task->pid, task->comm, task->__state, task_nice(task), task->cred->uid, rss_parent);
        sprintf(message, "PARENT PID: %d PROCESS: %s STATE: %ld NICE: %d UID: %d MEMORY: %ldkB\n",task->pid, task->comm,task->__state, task_nice(task), task->cred->uid, rss_parent);

        message_len=strlen(message);
        errors=copy_to_user(buffer, message, message_len); 

         /*    list_for_each MACRO to iterate through task->children    */
        list_for_each(list, &task->children){         
        /*    using list_entry to declare all vars in task_child struct    */
            task_child = list_entry( list, struct task_struct, sibling );    
            
            get_task_struct(task);
        
        if(task_child->mm){
            rss_child = get_mm_rss(task_child->mm) * 4;
        }
        else {
            rss_child = 0;
        }
                      
            /*    log child of and child pid/name/state    */
            sprintf(message, "CHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld NICE:%d MEMORY: %ldkB \n",task->comm, task->pid, 
                task_child->pid, task_child->comm,task_child->__state,task_nice(task), rss_child);

            printk(KERN_INFO "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld NICE:%d MEMORY: %ldkB",task->comm, task->pid, 
                task_child->pid, task_child->comm, task_child->__state, task_nice(task), rss_child);
        }
        printk("-----------------------------------------------------"); 
    }
    printk ("Total Memory: %ldkB",mem_total);  
    return message_len;
}

module_init(panes_init);
module_exit(panes_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

#define DEVICE_NAME "td28_module"

MODULE_LICENSE("GPL");

static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);
int print_process_list(char *buffer);                                    

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */

static int __init td28_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Tamales de la 28 module load failed\n");
        return major;
    }

    printk(KERN_INFO "Tamales de la 28 has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");
    //print_process_list();
    return 0;
}

static void __exit td28_exit(void) {
    printk(KERN_INFO "Tamales de la 28 module successfully unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Tamales de la 28 module device opened\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, Tamales de la 28 is read only\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Tamales de la 28 device closed\n");
   return 0;
}
int imprimio=0;

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    if(imprimio==1)
        return 0;

    errors =print_process_list(buffer);
    imprimio=1;
    return errors; //errors== 0 ? message_len : -EFAULT;
}

/*    Init Module    */

int print_process_list(char *buffer)                    
{
    char message[255];
    int message_len=0;
    int errors = 0;
    /*    good practice to log when loading/removing modules    */
   printk(KERN_INFO "%s","LOADING MODULE\n");   

    /*    for_each_process() MACRO for iterating through each task in the os located in linux\sched\signal.h    */
    for_each_process( task ){     

        /*    log parent id/executable name/state    */ 

        sprintf(message, "PARENT PID: %d PROCESS: %s STATE: %ld\n",task->pid, task->comm,task->__state);

        printk(KERN_INFO "\nPARENT PID: %d PROCESS: %s STATE: %ld PRIORITY: %ld NICE:%ld",task->pid, task->comm, task->__state, task->prio, 0);
        message_len=strlen(message);
        errors=copy_to_user(buffer, message, message_len); 
        //imprimio=0;
         /*    list_for_each MACRO to iterate through task->children    */
        list_for_each(list, &task->children){                       
            /*    using list_entry to declare all vars in task_child struct    */
            task_child = list_entry( list, struct task_struct, sibling );    

            /*    log child of and child pid/name/state    */
            sprintf(message, "CHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld \n",task->comm, task->pid, 
                task_child->pid, task_child->comm,task_child->__state);

            printk(KERN_INFO "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld",task->comm, task->pid, 
                task_child->pid, task_child->comm, task_child->__state);
        }
        printk("-----------------------------------------------------"); 
    }  
    return message_len;


}

module_init(td28_init);
module_exit(td28_exit);

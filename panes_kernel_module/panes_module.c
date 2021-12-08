#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#define DEVICE_NAME "panes_module"

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


static int __init panes_module_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Super Panes load failed\n");
        return major;
    }

    printk(KERN_INFO "Super Panes has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return 0;
}

static void __exit panes_exit(void) {
    printk(KERN_INFO "panes kernel module successfully unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Super Panes device opened\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, Super Panes is read only\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Super Panes device closed\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    //errors = copy_to_user(buffer, message, message_len);
    errors = print_process_list(buffer);

    //return errors == 0 ? message_len : -EFAULT;
    return 0;
}


int print_process_list(char *buffer)
{
    char proc_string[255];
    int message_len = 0;
    int errors = 0;
    printk(KERN_INFO "%s","LOADING MODULE\n");   
    
    long int rss_parent;
    long int rss_child;
    long int mem_total=0;

    for_each_process( task ){
        
        get_task_struct(task);
        
        if(task->mm){
            rss_parent = get_mm_rss(task->mm) * 4;
            mem_total += (int) rss_parent;
        }
        else {
            rss_parent = 0;
        }
        
        sprintf(proc_string, "\nPARENT PID: %d PROCESS: %s STATE: %ld Nice: %d UID: %d Memory: %ld", task->pid, task->comm, task->__state, task_nice(task), task->cred->uid, rss_parent);
        printk(KERN_INFO "\nPARENT PID: %d PROCESS: %s STATE: %ld Nice: %d", task->pid, task->comm, task->__state, task_nice(task));
        message_len = strlen(proc_string);
        printk(KERN_INFO "Message len: %d", message_len);
        errors = copy_to_user(buffer, proc_string, message_len);

        list_for_each(list, &task->children){

            task_child = list_entry( list, struct task_struct, sibling );

            sprintf(proc_string, "\nCHILD OF %s[%d] PROCESS: %s STATE: %ld Nice: %d", task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->__state, task_nice(task));
            printk(KERN_INFO "\nCHILD OF %s[%d] PROCESS: %s STATE: %ld Nice: %d", task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->__state, task_nice(task));
            message_len = strlen(proc_string);
            errors = copy_to_user(buffer, proc_string, message_len);
        }
        printk("Total Memory: %ld", mem_total);
        sprintf("Total Memory: %ld", mem_total);
        printk("-----------------------------------------------------");
    }

    return errors == 0 ? message_len : -EFAULT;
}

module_init(panes_module_init);
module_exit(panes_exit);

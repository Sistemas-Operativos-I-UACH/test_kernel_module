#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
//#include <sys/sysinfo.h>    //  Struct with total RAM info

#define DEVICE_NAME "pedro_module"

MODULE_LICENSE("GPL");

static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);
int print_process_list(char *buffer);
//int sysinfo (struct sysinfo *info); // prototipo funcion


static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

struct task_struct *task;           /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;     /*    Structure needed to iterate through task children    */
struct list_head *list;             /*    Structure needed to iterate through the list in each task->children struct    */
//struct sysinfo info;

static int __init  pedro_module_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if(major < 0) {
        printk(KERN_ALERT "Pedro load failed");
        return major;
    }
    
    printk(KERN_INFO "Pedro kernel module has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return 0;
}

static void __exit pedro_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "pedro kernel module successfully unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Pedro device opened\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    printk(KERN_INFO "Pedro is read only\n");
    return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Pedro device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer,size_t len, loff_t *offset) {
    int errors = 0;

    errors = print_process_list(buffer);

    return 0;
}

int print_process_list(char *buffer){
    
    char proc_string[255];
    int message_len = 0;
    int errors = 0;
    long int rss_memory;
    long int rss_child;
    char *process_type; 
    
    /*    good practice to log when loading/removing modules    */
    printk(KERN_INFO "LOADING MODULE\n");       

    /*  for_each_process() MACRO for iterating through each task in the os located in 
        linux\sched\signal.h    */
    for_each_process(task){
        
        get_task_struct(task);
        
        if(task->mm){
            rss_memory = get_mm_rss(task->mm) * 4;
            process_type = "Usuario";
        }
        else {
            rss_memory = 0;
            process_type = "Kernel";
        }

        /*    log parent id/executable name/memory/niceness    */
        sprintf(proc_string, "\nPARENT PID: %d\nPROCESS: %s\nMEMORIA: %ld kB\n
                NICENESS: %d", task->pid, task->comm, rss_memory, task_nice(task));
        printk(KERN_INFO "\nPARENT PID: %d\nPROCESS: %s\nMEMORIA: %ld kB\n
                NICENESS: %d", task->pid, task->comm, rss_memory, task_nice(task));
        printk(KERN_INFO "PROCESS TYPE: %s", process_type);
        message_len = strlen(proc_string);
        printk(KERN_INFO "Message len: %d\n", message_len);
        errors = copy_to_user(buffer, proc_string, message_len);

        /*    list_for_each MACRO to iterate through task->children    */
        list_for_each(list, &task->children){
            /*    using list_entry to declare all vars in task_child struct    */
            task_child = list_entry(list, struct task_struct, sibling);
            get_task_struct(task_child);

            if(task_child->mm){
                rss_child = get_mm_rss(task_child->mm) * 4;
                process_type = "Usuario";
            }
            else{
                rss_child = 0;
                process_type = "Kernel";
            }

            /*    log child of and child pid/name/memory/niceness    */
            sprintf(proc_string, "\nCHILD OF %s[%d]\nPID: %d\nPROCESS: %s\nMEMORIA: %ld  kB\n
                    NICENESS: %d", task->comm, task->pid, task_child->pid, task_child->comm, 
                    rss_child, task_nice(task_child));
            printk(KERN_INFO "\nCHILD OF %s[%d]\nPID: %d\nPROCESS: %s\nMEMORIA: %ld  kB\n
                    NICENESS: %d", task->comm, task->pid, task_child->pid, task_child->comm, 
                    rss_child, task_nice(task_child));
            printk(KERN_INFO "PROCESS TYPE: %s", process_type);
            message_len = strlen(proc_string);
            errors = copy_to_user(buffer, proc_string, message_len);
        }
        
        printk("-----------------------------------------------------");
    }
    //sysinfo(&info);
    //printk(KERN_INFO "TOTAL MEMORY: %ld", info.totalram/1024);
    return errors == 0 ? message_len : -EFAULT;
}

module_init(pedro_module_init);
module_exit(pedro_exit);
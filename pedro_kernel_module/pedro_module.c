#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "pedro_module"

MODULE_LICENSE("GPL");

static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

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
    char *message = "never gonna give you up, never gonna let you down...";
    int message_len = strlen(message);

    errors = copy_to_user(buffer, message, message_len);

    return errors == 0 ? message_len : -EFAULT;
}
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "kernel_padwan"

static int __init super_module_init(void) {
    printk(KERN_INFO "Kernel Padwan has been loaded\n");
    return 0;
}

static void __exit super_module_exit(void) {
    printk(KERN_INFO "Kernel Padwan has been unloaded\n");
}

module_init(super_module_init);
module_exit(super_module_exit);

MODULE_LICENSE("GPL");

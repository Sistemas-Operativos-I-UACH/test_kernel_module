#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init  pedro_init(void) {
    printk(KERN_INFO "Loading pedro kernel module\n");
    return 0;
}

static void __exit pedro_exit(void) {
    printk(KERN_INFO "pedro kernel module successfully unloaded\n");
}

module_init(pedro_init);
module_exit(pedro_exit);

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init  panes_init(void) {
    printk(KERN_INFO "Loading panes kernel module\n");
    return 0;
}

static void __exit panes_exit(void) {
    printk(KERN_INFO "panes kernel module successfully unloaded\n");
}

module_init(panes_init);
module_exit(panes_exit);

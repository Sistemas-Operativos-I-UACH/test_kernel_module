#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init  td28_init(void) {
    printk(KERN_INFO "Loading tamales de la 28 kernel module\n");
    return 0;
}

static void __exit td28_exit(void) {
    printk(KERN_INFO "Tamales de la 28 kernel module successfully unloaded\n");
}

module_init(td28_init);
module_exit(td28_exit);

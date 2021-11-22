#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init  pepito_init(void) {
    printk(KERN_INFO "Loading pepito kernel module\n");
    return 0;
}

static void __exit pepito_exit(void) {
    printk(KERN_INFO "pepito kernel module successfully unloaded\n");
}

module_init(pepito_init);
module_exit(pepito_exit);

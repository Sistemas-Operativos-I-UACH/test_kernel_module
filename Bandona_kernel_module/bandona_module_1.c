#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init  bandona_init(void) {
    printk(KERN_INFO "Loading bandona kernel module\n");
    return 0;
}

static void __exit bandona_exit(void) {
    printk(KERN_INFO "bandona kernel module successfully unloaded\n");
}

module_init(bandona_init);
module_exit(bandona_exit);

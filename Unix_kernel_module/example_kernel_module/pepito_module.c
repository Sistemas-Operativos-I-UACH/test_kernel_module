#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>

#include <linux/uaccess.h>
#include <linux/sched/signal.h>

#include <linux/sched.h>
#define DEVICE_NAME "pepito_module"

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

struct task_struct *task;        /*   Esta es una estructura definida en sched.h para tareas / procesos bien chida y perrona.    */

struct task_struct *task_child;        /*    Estructura necesaria para iterar a través de lo antes visto asi bien facil bien ggs.    */

struct list_head *list;            /*    esta es una Estructura necesaria para recorrer la lista en cada tarea-> estructura secundaria pa que quede bien perrote    */


static int __init pepito_module_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Super Pepito load failed\n");
        return major;
    }

    printk(KERN_INFO "Super Pepito has been loaded\n");
    printk(KERN_INFO "=============================\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "=============================\n");

    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "==================================\n");

    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "=======================\n");

    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "=================================================\n");

    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "================\n");
    
    printk(KERN_INFO "Remove the device file and module when done.\n");
    printk(KERN_INFO "============================================\n");
    return 0;
}

static void __exit pepito_exit(void) {
    printk(KERN_INFO "modoulo kernel unix descargado con exito\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Dispositivo unix abierto\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Lo siento unix es de solo lectura\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Dispositivo unix cerrado\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    char *message = "never gonna give you up, never gonna let you down... ";

    //int message_len = strlen(message);

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
    printk(KERN_INFO "%s","Modulo de carga\n");    
    
    /*    excelente práctica para iniciar sesión al cargar / eliminar módulos y ademas para que se vea bien y no ser unos programadores cochinos    */

    /*   for_each_process () MACRO para iterar a través de cada tarea en el sistema operativo ubicado en linux \ sched \ signal.h cabe aclarar que es indispensable    */
    
    for_each_process( task ){
        
        /*    registrar id padre / nombre ejecutable / estado    */
        //task->on_cpu task->prio

        sprintf(proc_string, "\nPARENT PID: %d PROCESS: %s STATE: %ld", task->pid, task->comm, task->__state);

        printk(KERN_INFO "\nPARENT PID: %d PROCESS: %s STATE: %ld",task->pid, task->comm, task->__state);

        message_len = strlen(proc_string);

        printk(KERN_INFO "Message len: %d", message_len);

        errors = copy_to_user(buffer, proc_string, message_len);

        /*    list_for_each MACRO para iterar a través de la tarea-> niños    */

        list_for_each(list, &task->children){

            /*    usando list_entry para declarar todas las vars en la estructura task_child    */

            task_child = list_entry( list, struct task_struct, sibling );

            /*    registro hijo de y hijo pid / nombre / estado   */

            sprintf(proc_string, "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld",task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->__state);
            printk(KERN_INFO "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld",task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->__state);
            message_len = strlen(proc_string);
            errors = copy_to_user(buffer, proc_string, message_len);
        }
        printk("-----------------------------------------------------");    /*for aesthetics*/
    }

    return errors == 0 ? message_len : -EFAULT;
}

module_init(pepito_module_init);

module_exit(pepito_exit);
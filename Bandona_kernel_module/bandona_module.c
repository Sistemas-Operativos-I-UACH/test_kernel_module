#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

#define DEVICE_NAME "bandona_module"

MODULE_LICENSE("GPL");

static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

int print_process_list(char *buffer);
//static int process_sysctl_arg(char *param, char *val,const char *unused, void *arg);
//void meminfo_file(char*f);
//void proc_kernel(char *f);

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

struct task_struct *task;        /*    Structure defined in sched.h for tasks/processes    */
struct task_struct *task_child;        /*    Structure needed to iterate through task children    */
struct list_head *list;            /*    Structure needed to iterate through the list in each task->children struct    */

/*static int process_sysctl_arg(char *param, char *val,
			       const char *unused, void *arg)
{
	char *path;
	struct vfsmount **proc_mnt = arg;
	struct file_system_type *proc_fs_type;
	struct file *file;
	int len;
	int err;
	loff_t pos = 0;
	ssize_t wret;

	if (strncmp(param, "sysctl", sizeof("sysctl") - 1) == 0) {
		param += sizeof("sysctl") - 1;

		if (param[0] != '/' && param[0] != '.')
			return 0;

		param++;
	} else {
		param = (char *) sysctl_find_alias(param);
		if (!param)
			return 0;
	}

	if (!val)
		return -EINVAL;
	len = strlen(val);
	if (len == 0)
		return -EINVAL;

	/*
	 * To set sysctl options, we use a temporary mount of proc, look up the
	 * respective sys/ file and write to it. To avoid mounting it when no
	 * options were given, we mount it only when the first sysctl option is
	 * found. Why not a persistent mount? There are problems with a
	 * persistent mount of proc in that it forces userspace not to use any
	 * proc mount options.
	 
	if (!*proc_mnt) {
		proc_fs_type = get_fs_type("proc");
		if (!proc_fs_type) {
			pr_err("Failed to find procfs to set sysctl from command line\n");
			return 0;
		}
		*proc_mnt = kern_mount(proc_fs_type);
		put_filesystem(proc_fs_type);
		if (IS_ERR(*proc_mnt)) {
			pr_err("Failed to mount procfs to set sysctl from command line\n");
			return 0;
		}
	}

	path = kasprintf(GFP_KERNEL, "sys/%s", param);
	if (!path)
		panic("%s: Failed to allocate path for %s\n", __func__, param);
	strreplace(path, '.', '/');

	file = file_open_root_mnt(*proc_mnt, path, O_WRONLY, 0);
	if (IS_ERR(file)) {
		err = PTR_ERR(file);
		if (err == -ENOENT)
			pr_err("Failed to set sysctl parameter '%s=%s': parameter not found\n",
				param, val);
		else if (err == -EACCES)
			pr_err("Failed to set sysctl parameter '%s=%s': permission denied (read-only?)\n",
				param, val);
		else
			pr_err("Error %pe opening proc file to set sysctl parameter '%s=%s'\n",
				file, param, val);
		goto out;
	}
	wret = kernel_write(file, val, len, &pos);
	if (wret < 0) {
		err = wret;
		if (err == -EINVAL)
			pr_err("Failed to set sysctl parameter '%s=%s': invalid value\n",
				param, val);
		else
			pr_err("Error %pe writing to proc file to set sysctl parameter '%s=%s'\n",
				ERR_PTR(err), param, val);
	} else if (wret != len) {
		pr_err("Wrote only %zd bytes of %d writing to proc file %s to set sysctl parameter '%s=%s\n",
			wret, len, path, param, val);
	}

	err = filp_close(file, NULL);
	if (err)
		pr_err("Error %pe closing proc file to set sysctl parameter '%s=%s\n",
			ERR_PTR(err), param, val);
out:
	kfree(path);
	return 0;
}

void meminfo_file(char*f){
	file = file_open_root((*proc_mnt)->mnt_root, *proc_mnt, path, O_WRONLY, 0);
	char n[20];
	long tammem;
	float t;
	fscanf(fd,"%s""%ld",n,&tammem);
	t=tammem;
	printf("\n\nTamaño de memoria del sistema: %f Gb\n",(t/1024)/1024);


}

void proc_kernel(char *f){
	file = file_open_root((*proc_mnt)->mnt_root, *proc_mnt, path, O_WRONLY, 0);
	char n;
	
	if(fd==NULL){
		printf("archivo cmdline faltante");
	}else{
		if((n=fgetc(fd))==EOF){
			printf("Proceso de Kernel\n");
		}else{
			printf("Proceso de Usuario\n");
		}
	}
	fclose(fd);
}*/


static int __init bandona_module_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Super bandona load failed\n");
        return major;
    }

    printk(KERN_INFO "Super bandona has been loaded\n");

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return 0;
}

static void __exit bandona_exit(void) {
    printk(KERN_INFO "bandona kernel module successfully unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   char *buffer;
   printk(KERN_INFO "Super bandona device opened\n");
   print_process_list(buffer);
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, Super bandona is read only\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "Super bandona device closed\n");
   return 0;
}

int imp=0;
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    if(imp==1)
    	return 0;
    //char *message = "never gonna give you up, never gonna let you down... ";
    //int message_len = strlen(message);

    //errors = copy_to_user(buffer, message, message_len);
    errors = print_process_list(buffer);
    imp=1;
	
    //return errors == 0 ? message_len : -EFAULT;
    return errors;
}

int print_process_list(char *buffer)
{
    char proc_string[255];
    int message_len = 0;
    int errors = 0;
    printk(KERN_INFO "%s","LOADING MODULE\n");  
    printk("\nTAMAÑO DE MEMORIA DEL SISTEMA: 4.343704 Gb\n");
    /*char dirName[255];
    // memoria del sistema
	strcpy(dirName, "/proc/meminfo");
	meminfo_file(dirName); */

    
    for_each_process( task )
    {
        /*    log parent id/executable name/state    */
        //task->on_cpu task->prio
        printk("****************************************************************************************************************");
        
        printk(KERN_INFO "\nPARENT PID: %d PROCESS: %s STATE: %ld PRIORITY: %d ",task->pid, task->comm, task->state, task->prio);
        sprintf(proc_string, "\PARENT PID: %d PROCESS: %s STATE: %ld PRIORITY: %d \n",task->pid, task->comm, task->state, task->prio);
        message_len = strlen(proc_string);
        //printk(KERN_INFO "Message len: %d", message_len);
        errors = copy_to_user(buffer, proc_string, message_len);
	
        /*    list_for_each MACRO to iterate through task->children    */
        list_for_each(list, &task->children){

            /*    using list_entry to declare all vars in task_child struct    */
            task_child = list_entry( list, struct task_struct, sibling );

            /*    log child of and child pid/name/state    */
            sprintf(proc_string, "CHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld PRIORITY: %d\n",task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->state,task->prio);
            printk(KERN_INFO "\nCHILD OF %s[%d] PID: %d PROCESS: %s STATE: %ld PRIORITY: %d",task->comm, task->pid,
                task_child->pid, task_child->comm, task_child->state,task->prio);
            //message_len = strlen(proc_string);
            //errors = copy_to_user(buffer, proc_string, message_len);
        }
        printk("****************************************************************************************************************\n");
    }

    //return errors == 0 ? message_len : -EFAULT;
    return message_len;
    
}

module_init(bandona_module_init);
module_exit(bandona_exit);

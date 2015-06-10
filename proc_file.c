#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("eItra proc filesystem example");
MODULE_AUTHOR("Neepa Vasa");

#define MODULE_NAME "procfs_example"

static struct proc_dir_entry *example_dir, *file1_entry;
static char ei_str[100];

int ei_read_file( struct file *, char __user *, size_t, loff_t *);
int ei_write_file( struct file *, char __user *, size_t , loff_t *);

int ei_procfs_open(struct inode *node, struct file *fp)
{
        printk(KERN_NOTICE "ei-seq-file-example : open\n");
	return 0;
}

int ei_procfs_release(struct inode *node, struct file *fp)
{
        return 0;
}

int ei_read_file( struct file *fp, char __user *buf, size_t count, loff_t *fpos)
{
	int len ;
        printk(KERN_NOTICE "ei-procfs example: read called..\n");
        len=strlen(ei_str);

        if (count < len)
                return -EINVAL;

        if (*fpos != 0)
                return 0;

        if (!access_ok(VERIFY_WRITE, buf, count))
                return -EFAULT;

        if (copy_to_user(buf, ei_str, len))
                return -EINVAL;

        *fpos = len;
        return len;

}

int ei_write_file( struct file *fp, char __user *buf, size_t count, loff_t *fpos)
{
	int len = 100 ;
        printk(KERN_NOTICE "ei-procfs example: write called..\n");

	if (count > len)
                return -EINVAL;

        if (*fpos != 0)
                return 0;

        if (!access_ok(VERIFY_read, buf, count))
                return -EFAULT;

        if (copy_from_user(ei_str, buf, count))
                return -EINVAL;
	ei_str[count]='\0';

        printk(KERN_NOTICE "ei-procfs example: written value is %s ..\n", ei_str);
        return count;
}

static struct file_operations proc_fops =
{
        .owner = THIS_MODULE,
        .write = ei_write_file,
        .read = ei_read_file,
};

static int ei_procfs_init(void)
{

        printk(KERN_NOTICE "ei-procfs-example : creating proc entry\n");
	example_dir = proc_mkdir(MODULE_NAME, NULL);
	if(example_dir == NULL)
		return -ENOMEM;
		
	file1_entry = proc_create_data("file1", 0666, example_dir, &proc_fops, NULL);

	if (file1_entry == NULL)
		goto out;

	return 0;
out:
	remove_proc_entry(MODULE_NAME, NULL);
	return -ENOMEM;
}



static void ei_procfs_exit(void)
{
        printk(KERN_NOTICE "ei-proc-fs-example : removing proc file\n");
	remove_proc_entry("file1",example_dir);
        printk(KERN_NOTICE "ei-proc-fs-example : removing proc directory\n");
	remove_proc_entry(MODULE_NAME, NULL);
	printk(KERN_ALERT "Bye\n");
}

module_init(ei_procfs_init);
module_exit(ei_procfs_exit);


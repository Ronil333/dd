#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kdev_t.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("eItra sys filesystem example");
MODULE_AUTHOR("Neepa Vasa");

static char ei_str[100];
static ssize_t first_store(struct class *, struct attribute *, const char *, size_t);
static ssize_t first_show(struct class *, struct attribute *, char *);
		

static struct class_attribute myattr[] = {
        __ATTR(first, 0644, first_show, first_store) ,
        __ATTR_NULL,
};

static ssize_t first_show(struct class *cl, struct attribute *attr,
                              char *buf)
{
	printk(KERN_INFO "Inside show\n");
        return sprintf(buf, "%s\n",ei_str );
}

static ssize_t first_store(struct class *cl, struct attribute *attr,
                               const char *buf, size_t len)
{
	printk(KERN_INFO "Inside store\n");
        return snprintf(ei_str, len, "%s\n", buf);
}

static struct class *cl;
static int __init sysfsexample_module_init(void)
{
	strcpy(ei_str, "class and attribute create example\n");
	if(IS_ERR(cl = class_create(THIS_MODULE, "testcl")))
        {
                printk(KERN_ALERT "ei class creation failed\n");
                return -1;
        }

       	printk(KERN_ALERT "test class creation done\n");
	if(IS_ERR(class_create_file(cl, myattr)))
	{
                printk(KERN_ALERT "attribute creation failed\n");
                return -1;
	}
       	printk(KERN_ALERT "attribute creation done\n");
	return 0;

}

static void __exit sysfsexample_module_exit(void)
{
	class_remove_file(cl, myattr);
	class_destroy(cl);
}

module_init(sysfsexample_module_init);
module_exit(sysfsexample_module_exit);


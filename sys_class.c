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
        __ATTR(first, 0777, first_show, first_store) ,
        __ATTR_NULL,
};

static struct class test_class = {
	.name = "test",
	.owner = THIS_MODULE,
	.class_attrs =  myattr,
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


static int __init sysfsexample_module_init(void)
{
	strcpy(ei_str, "class and attribute create example\n");
	class_register(&test_class);	
       	printk(KERN_ALERT "test class creation done\n");
	return 0;

}

static void __exit sysfsexample_module_exit(void)
{
	class_unregister(&test_class);
}

module_init(sysfsexample_module_init);
module_exit(sysfsexample_module_exit);


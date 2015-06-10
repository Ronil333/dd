#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kdev_t.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("eItra sys filesystem example");
MODULE_AUTHOR("Neepa Vasa");

static int first, second;
		
static ssize_t first_show(struct kobject *, struct kobj_attribute *, char *);
static ssize_t first_store(struct kobject *, struct kobj_attribute *, const char *, size_t );

static ssize_t second_show(struct kobject *, struct kobj_attribute *, char *);
static ssize_t second_store(struct kobject *, struct kobj_attribute *, const char *, size_t );

static struct kobj_attribute my_first = 
	__ATTR(first, 0666, first_show, first_store);

static struct kobj_attribute my_second = 
	__ATTR(second, 0666, second_show, second_store);

static struct attribute * myattr[] = {
        &my_first.attr,
        &my_second.attr,
        NULL,
};

static struct attribute_group attr_group = {
	.attrs = myattr,
};


static ssize_t first_show(struct kobject *kobj, struct kobj_attribute *attr,
                              char *buf)
{
        return sprintf(buf, "%d\n", first);
}

static ssize_t first_store(struct kobject *kobj, struct kobj_attribute *attr,
                               const char *buf, size_t len)
{
        sscanf(buf, "%d", &first);
	printk(KERN_INFO "Value written is %d\n",first);
        return len;
}

static ssize_t second_show(struct kobject *kobj, struct kobj_attribute *attr,
                              char *buf)
{
        return sprintf(buf, "%d\n", second);
}

static ssize_t second_store(struct kobject *kobj, struct kobj_attribute *attr,
                               const char *buf, size_t len)
{
        sscanf(buf, "%d", &second);
	printk(KERN_INFO "Value written is %d\n",second);
        return len;
}

struct kobject *mykobj;
static int __init sysfsexample_module_init(void)
{
        int ret = -1;
	mykobj = kobject_create_and_add("kobj_ex", kernel_kobj);
	//mykobj = kobject_create_and_add("kobj_ex", NULL);
	//mykobj = kobject_create_and_add("kobj_ex", hypervisor_kobj);
	//mykobj = kobject_create_and_add("kobj_ex", firmware_kobj);
	//mykobj = kobject_create_and_add("kobj_ex", fs_kobj);
	if(!mykobj)
	{
		printk(KERN_INFO "Kobject creation failed\n");
		return -ENOMEM;
	}
	ret = sysfs_create_group(mykobj,&attr_group);
	if(ret)
		kobject_put(mykobj); 
        return ret;
}

static void __exit sysfsexample_module_exit(void)
{
	sysfs_remove_group(mykobj,&attr_group);
	if(mykobj)
	{
		kobject_put(mykobj);
		mykobj = NULL;
	}
}

module_init(sysfsexample_module_init);
module_exit(sysfsexample_module_exit);


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
//(1)Create structure of fake device 
struct fakedevice
{
char data[20];
struct semaphore sem;
}virtual_device;


//(2)To later register device we need cdev object and some variable

struct cdev *mdev;
int major_number;
int ret;
dev_t dev_num;

#define DEVICE_NAME "solidusdevice"
static int driver_entry(void)
{
//Register device with system 
//Two steps
//Step(1)Dynamic allocation of device
ret=alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
if(ret<0)
{
printk(KERN_INFO"Failed to allocate major number");

return ret;
}
major_number=MAJOR(dev_num);
printk(KERN_INFO"soliduscode:major number is %d\n",major_number);
//printk(KERN_INFO"\t use \"mknod /dev/%s c %d");
//Step-2-->Allocate char device structure and initialize properly
mcdev=cdev_alloc();
mcdev->ops=&fops;
mcdev->owner=THIS_MODULE;
//Now we have created cdev,we have to add to kernel
ret=cdev_add(mcdev,dev_num,1);
if(ret<0)
{
printk(KERN_ALERT"soliduscode:Unable to add to kernel");
return ret;
}

//Initialize semaphore
sema_init(virtual_device.sem,1);
return 0;
}

static void driver_exit(void)
{
//delete device and unregister char device region
cdev_del(mcdev);
unregister_chrdev_region(dev_num,1);
printk(KERN_ALERT"solidus code:unloaded module");
}





#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

#define module_version "1.0"
#define module_name "procfs_example"
#define foobar_len 8
struct fb_data_t {
char name[foobar_len + 1];
char value[foobar_len + 1];
};
static struct proc_dir_entry *example_dir, *foo_file,
*bar_file, *jiffies_file, *tty_device, *symlink;
struct fb_data_t foo_data, bar_data;
static int proc_read_jiffies(char *page, char **start,
off_t off, int count,
int *eof, void *data)
{
int len;
mod_inc_use_count;
len = sprintf(page, "jiffies = %ld\n",
jiffies);
mod_dec_use_count;
return len;
}
static int proc_read_foobar(char *page, char **start,
off_t off, int count,
int *eof, void *data)
{
int len;



struct fb_data_t *fb_data = (struct fb_data_t *)data;
mod_inc_use_count;
len = sprintf(page, "%s = ’%s’\n",
fb_data->name, fb_data->value);
mod_dec_use_count;
return len;

static int proc_write_foobar(struct file *file,
const char *buffer,
unsigned long count,
void *data)
{
int len;
struct fb_data_t *fb_data = (struct fb_data_t *)data;
mod_inc_use_count;
if(count > foobar_len)
len = foobar_len;
else
len = count;
if(copy_from_user(fb_data->value, buffer, len)) {
mod_dec_use_count;
return -efault;
}
fb_data->value[len] = ’\0’;
mod_dec_use_count;
return len;
}
static int __init init_procfs_example(void)
{
int rv = 0;
/* create directory */
example_dir = proc_mkdir(module_name, null);
if(example_dir == null) {


rv = -enomem;
goto out;
}
example_dir->owner = this_module;
/* create jiffies using convenience function */
jiffies_file = create_proc_read_entry("jiffies",
0444, example_dir,
proc_read_jiffies,
null);
if(jiffies_file == null) {
rv = -enomem;
goto no_jiffies;
}
jiffies_file->owner = this_module;
/* create foo and bar files using same callback
* functions
*/
foo_file = create_proc_entry("foo", 0644, example_dir);
if(foo_file == null) {
rv = -enomem;
goto no_foo;
}
strcpy(foo_data.name, "foo");
strcpy(foo_data.value, "foo");
foo_file->data = &foo_data;
foo_file->read_proc = proc_read_foobar;
foo_file->write_proc = proc_write_foobar;
foo_file->owner = this_module;
bar_file = create_proc_entry("bar", 0644, example_dir);
if(bar_file == null) {
rv = -enomem;
goto no_bar;
}
strcpy(bar_data.name, "bar");
strcpy(bar_data.value, "bar");
bar_file->data = &bar_data;
bar_file->read_proc = proc_read_foobar;
bar_file->write_proc = proc_write_foobar;
bar_file->owner = this_module;
/* create tty device */


tty_device = proc_mknod("tty", s_ifchr | 0666, example_dir, mkdev(5, 0));
if(tty_device == null) {
rv = -enomem;
goto no_tty;
}
tty_device->owner = this_module;
/* create symlink */
symlink = proc_symlink("jiffies_too", example_dir,
"jiffies");
if(symlink == null) {
rv = -enomem;
goto no_symlink;
}
symlink->owner = this_module;
/* everything ok */
printk(kern_info "%s %s initialised\n",
module_name, module_version);
return 0;
no_symlink:
remove_proc_entry("tty", example_dir);
no_tty:
remove_proc_entry("bar", example_dir);
no_bar:
remove_proc_entry("foo", example_dir);
no_foo:
remove_proc_entry("jiffies", example_dir);
no_jiffies:
remove_proc_entry(module_name, null);
out:
return rv;
}


static void __exit cleanup_procfs_example(void)
{
remove_proc_entry("jiffies_too", example_dir);
remove_proc_entry("tty", example_dir);
remove_proc_entry("bar", example_dir);
remove_proc_entry("foo", example_dir);
remove_proc_entry("jiffies", example_dir);
remove_proc_entry(module_name, null);
printk(kern_info "%s %s removed\n",
module_name, module_version);
}
module_init(init_procfs_example);
module_exit(cleanup_procfs_example);
module_author("erik mouw");
module_description("procfs examples");



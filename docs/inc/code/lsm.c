#include <linux/module.h>
#include <linux/lsm_hooks.h>

static int foobar_inode_mkdir(struct inode *dir, struct dentry *dentry, umode_t mask)
{
    	printk(KERN_INFO "mkdir");
    	return 0;
}

static struct security_hook_list foobar_hooks[] =
{
    	LSM_HOOK_INIT(inode_mkdir, foobar_inode_mkdir),
};

void __init foobar_add_hooks(void)
{
    	security_add_hooks(foobar_hooks, ARRAY_SIZE(foobar_hooks));
}

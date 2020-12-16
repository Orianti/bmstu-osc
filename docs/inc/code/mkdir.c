#include <linux/module.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

extern void *sys_call_table[];
int (*orig_mkdir)(const char *path);

int own_mkdir(const char *path)
{
	return 0;
}

int init_module(void)
{
	orig_mkdir = sys_call_table[SYS_mkdir];
	sys_call_table[SYS_mkdir] = own_mkdir;
	return(0);
}

void cleanup_module(void)
{
	sys_call_table[SYS_mkdir] = orig_mkdir;
}

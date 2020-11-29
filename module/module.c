#include <linux/module.h>

#include "hooks.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Novikov");
MODULE_DESCRIPTION("Linux Kernel Memory Allocation Hook");

static char *syscall_table_addr = NULL;
module_param(syscall_table_addr, charp, 0);
MODULE_PARM_DESC(syscall_table_addr, "Base address of the syscall table");

struct sys_hook *hook;

static uintptr_t addr_to_pointer(const char *str)
{
    uintptr_t addr = 0;

    for (; *str != '\0'; ++str) {
        addr *= 16;

        if (*str >= '0' && *str <= '9')
            addr += (*str - '0');
        else if (*str >= 'a' && *str <= 'f')
            addr += (*str - 'a') + 10;
        else
            return 0;
    }

    return addr;
}

static inline void add_hooks(void)
{
    sys_hook_add(hook, __NR_brk, (void *)brk_hook);
    sys_hook_add(hook, __NR_mmap, (void *)mmap_hook);
    sys_hook_add(hook, __NR_mremap, (void *)mremap_hook);
    sys_hook_add(hook, __NR_munmap, (void *)munmap_hook);
}

static int __init md_init(void)
{
    uintptr_t addr;

    if (syscall_table_addr == NULL || syscall_table_addr[0] == '\0') {
        printk(KERN_EMERG "lkmah: failed to get syscall table\n");
        return 1;
    }

    addr = addr_to_pointer(syscall_table_addr);
    if (addr == 0) {
        printk(KERN_EMERG "lkmah: invalid x64 syscall address\n");
        return 1;
    }

    hook = sys_hook_init(addr);
    if (hook == NULL) {
        printk(KERN_EMERG "lkmah: failed to initialize sys_hook\n");
        return 1;
    }

    add_hooks();

    printk(KERN_INFO "lkmah: module loaded\n");

    return 0;
}

static void __exit md_exit(void)
{
    sys_hook_free(hook);

    printk(KERN_INFO "lkmah: module unloaded\n");
}

module_init(md_init);
module_exit(md_exit);
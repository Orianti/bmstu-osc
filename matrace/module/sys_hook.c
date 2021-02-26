#include "sys_hook.h"

static uint64_t get_cr0(void)
{
    uint64_t ret;

    __asm__ volatile (
    "movq %%cr0, %[ret]"
    : [ret] "=r"(ret)
    );

    return ret;
}

static void set_cr0(uint64_t cr0)
{
    __asm__ volatile (
    "movq %[cr0], %%cr0"
    :
    : [cr0] "r"(cr0)
    );
}

struct sys_hook *sys_hook_init(uintptr_t addr)
{
    struct sys_hook *hook = kmalloc(sizeof(struct sys_hook), GFP_KERNEL);

    if (hook == NULL) {
        printk(KERN_EMERG "matrace: not enough memory\n");
        return NULL;
    }

    hook->syscall_table = (unsigned long long *)addr;
    hook->head = NULL;
    hook->tail = NULL;

    return hook;
}

void sys_hook_free(struct sys_hook *hook)
{
    struct sys_hook_ent *cur, *next;

    if (hook == NULL)
        return;

    set_cr0(get_cr0() & ~CR0_WRITE_PROTECT);

    for (cur = hook->head; cur != NULL;) {
        hook->syscall_table[cur->syscall] = (unsigned long long)cur->original;

        next = cur->next;
        kfree(cur);
        cur = next;
    }

    set_cr0(get_cr0() | CR0_WRITE_PROTECT);

    kfree(hook);
}

bool sys_hook_add(struct sys_hook *hook, unsigned int syscall, void *hook_func)
{
    struct sys_hook_ent *entry = kmalloc(sizeof(struct sys_hook_ent), GFP_KERNEL);

    if (entry == NULL) {
        printk(KERN_EMERG "matrace: not enough memory\n");
        return false;
    }

    entry->syscall = syscall;
    entry->original = hook->syscall_table[syscall];
    entry->hooked = (uintptr_t)hook_func;
    entry->next = NULL;

    set_cr0(get_cr0() & ~CR0_WRITE_PROTECT);
    hook->syscall_table[syscall] = (unsigned long long)entry->hooked;
    set_cr0(get_cr0() | CR0_WRITE_PROTECT);

    if (hook->head == NULL)
        hook->head = hook->tail = entry;
    else {
        hook->tail->next = entry;
        hook->tail = entry;
    }

    return true;
}

uintptr_t sys_hook_get_original(struct sys_hook *hook, unsigned int syscall)
{
    struct sys_hook_ent *cur = hook->head;

    for (; cur != NULL; cur = cur->next) {
        if (cur->syscall == syscall)
            return cur->original;
    }

    return 0;
}

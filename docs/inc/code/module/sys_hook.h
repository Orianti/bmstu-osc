#ifndef SYS_HOOK_H
#define SYS_HOOK_H

#include <linux/syscalls.h>

#define CR0_WRITE_PROTECT (1 << 16)

struct sys_hook {
    unsigned long long *syscall_table;

    struct sys_hook_ent *head;
    struct sys_hook_ent *tail;
};

struct sys_hook_ent {
    unsigned int syscall;

    uintptr_t original;
    uintptr_t hooked;

    struct sys_hook_ent *next;
};

struct sys_hook *sys_hook_init(uintptr_t);
void sys_hook_free(struct sys_hook *);

bool sys_hook_add(struct sys_hook *, unsigned int, void *);

uintptr_t sys_hook_get_original(struct sys_hook *, unsigned int);

#endif
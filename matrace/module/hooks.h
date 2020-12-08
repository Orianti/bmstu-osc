#ifndef HOOKS_H
#define HOOKS_H

#include <linux/syscalls.h>

#include "sys_hook.h"
#include "log.h"

typedef asmlinkage long (*sys_brk_t)(unsigned long);
typedef asmlinkage long (*sys_mmap_t)(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, off_t);
typedef asmlinkage long (*sys_munmap_t)(unsigned long, size_t);
typedef asmlinkage long (*sys_mremap_t)(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);

asmlinkage long brk_hook(unsigned long);
asmlinkage long mmap_hook(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, off_t);
asmlinkage long munmap_hook(unsigned long, size_t);
asmlinkage long mremap_hook(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);

#endif
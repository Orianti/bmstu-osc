#include "hooks.h"

extern struct sys_hook *hook;

asmlinkage long brk_hook(unsigned long brk)
{
    sys_brk_t sys_brk = (sys_brk_t)sys_hook_get_original(hook, __NR_brk);
    long ret = sys_brk(brk);

    log_vma(current, "brk");

    return ret;
}

asmlinkage long mmap_hook(unsigned long addr, unsigned long len, unsigned long prot,
                         unsigned long flags, unsigned long fd, off_t pgoff)
{
    sys_mmap_t sys_mmap = (sys_mmap_t)sys_hook_get_original(hook, __NR_mmap);
    long ret = sys_mmap(addr, len, prot, flags, fd, pgoff);

    log_vma(current, "mmap");

    return ret;
}

asmlinkage long munmap_hook(unsigned long addr, size_t len)
{
    sys_munmap_t sys_munmap = (sys_munmap_t)sys_hook_get_original(hook, __NR_munmap);
    long ret = sys_munmap(addr, len);

    log_vma(current, "munmap");

    return ret;
}

asmlinkage long mremap_hook(unsigned long addr, unsigned long old_len, unsigned long new_len,
                            unsigned long flags, unsigned long new_addr)
{
    sys_mremap_t sys_mremap = (sys_mremap_t)sys_hook_get_original(hook, __NR_mremap);
    long ret = sys_mremap(addr, old_len, new_len, flags, new_addr);

    log_vma(current, "mremap");

    return ret;
}
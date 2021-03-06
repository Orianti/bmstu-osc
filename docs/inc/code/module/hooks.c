#include "hooks.h"

extern struct sys_hook *hook;

/* Функция-перехватчик вызова brk */
asmlinkage long brk_hook(unsigned long brk) 
{
    sys_brk_t sys_brk = (sys_brk_t)sys_hook_get_original(hook, __NR_brk);
    long ret = sys_brk(brk);

    log_vma(current, "brk");

    return ret;
}

/* Функция-перехватчик вызова mmap */
asmlinkage long mmap_hook(unsigned long addr, unsigned long len, unsigned long prot,
                         unsigned long flags, unsigned long fd, off_t pgoff)
{
    sys_mmap_t sys_mmap = (sys_mmap_t)sys_hook_get_original(hook, __NR_mmap);
    long ret = sys_mmap(addr, len, prot, flags, fd, pgoff);

    log_vma(current, "mmap");

    return ret;
}

/* Функция-перехватчик вызова munmap */
asmlinkage long munmap_hook(unsigned long addr, size_t len)
{
    sys_munmap_t sys_munmap = (sys_munmap_t)sys_hook_get_original(hook, __NR_munmap);
    long ret = sys_munmap(addr, len);

    log_vma(current, "munmap");

    return ret;
}

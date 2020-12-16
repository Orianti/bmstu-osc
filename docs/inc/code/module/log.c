#include "log.h"

inline void log_vma(struct task_struct *task, const char *func)
{
    unsigned long heap_size = 0; /* Размер кучи */
    unsigned long stack_size = 0; /* Размер стека */
    unsigned long anon_size = 0; /* Размер анонимных отображений */

    struct mm_struct *mm = task->mm;
    struct vm_area_struct *vma = mm->mmap;

    for (; vma != NULL; vma = vma->vm_next) { /* Цикл по списку областей памяти */
        unsigned long size = vma->vm_end - vma->vm_start;

        if (vma->vm_start <= mm->brk && vma->vm_end >= mm->start_brk) {
            heap_size += size;
        } else if (vma->vm_start <= mm->start_stack && vma->vm_end >= mm->start_stack) {
            stack_size += size;
        } else if (vma->anon_vma != NULL && vma->vm_file == NULL) {
            anon_size += size;
        }
    }

    printk(KERN_INFO "matrace: %d: %s %lu %lu %lu\n", task->pid, func, heap_size, stack_size, anon_size);
}

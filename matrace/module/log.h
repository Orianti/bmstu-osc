#ifndef LOG_H
#define LOG_H

#include <linux/sched.h>
#include <linux/mm_types.h>

inline void log_vma(struct task_struct *, const char *);

#endif

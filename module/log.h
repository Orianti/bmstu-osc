#ifndef LOG_H
#define LOG_H

#include <linux/sched.h>

inline void log_vma(struct task_struct *, const char *);

#endif
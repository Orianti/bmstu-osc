obj-m := matrace.o

matrace-objs += module.o
matrace-objs += sys_hook.o
matrace-objs += hooks.o
matrace-objs += log.o

ADDR = $(word 1, $(shell cat /proc/kallsyms | grep " sys_call_table"))

all default:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

install: default
	insmod ./matrace.ko syscall_table_addr="$(ADDR)"

uninstall:
	rmmod matrace

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

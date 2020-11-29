obj-m := lkmah.o

lkmah-objs += module.o
lkmah-objs += sys_hook.o
lkmah-objs += hooks.o
lkmah-objs += log.o

ADDR = $(word 1, $(shell cat /proc/kallsyms | grep " sys_call_table"))

all default:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install: default
	insmod ./lkmah.ko syscall_table_addr="$(ADDR)"

uninstall:
	rmmod lkmah

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

NAME = net_probe
obj-m += $(NAME).o 
PWD := $(CURDIR) 
CLUTTER = *.o *.cmd *.mod *.mod.c *.order *.symvers .*.cmd *.d *.o.d
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
	rm -f $(CLUTTER)
full:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
	rm -f $(CLUTTER)
	insmod $(NAME).ko
	lsmod | grep $(NAME)
	ping -c 1 8.8.8.8
r:
	rmmod $(NAME)
i:
	insmod $(NAME).ko
t:
	ping -c 1 8.8.8.8
d:
	dmesg -T | grep lomond
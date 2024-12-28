NAME = net_probe
obj-m += $(NAME).o 
PWD := $(CURDIR) 
CLUTTER = *.o *.cmd *.mod *.mod.c *.order *.symvers .*.cmd *.d
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
	rm -f $(CLUTTER)
full:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
	rm -f $(CLUTTER)
	insmod $(NAME).ko
	lsmod | grep $(NAME)
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
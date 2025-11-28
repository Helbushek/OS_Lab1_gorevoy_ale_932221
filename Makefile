obj-m += lab3.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install: all
	sudo cp lab3.ko /lib/modules/$(shell uname -r)/extra/
	sudo depmod -a

load: all
	sudo insmod lab3.ko

unload:
	sudo rmmod lab3

reload: unload load

.PHONY: all clean install load unload reload
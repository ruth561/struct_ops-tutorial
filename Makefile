PWD	:= $(shell pwd)
obj-m	+= my_ops.o


all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

install:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

insmod:
	sudo dmesg -C
	sudo insmod my_ops.ko
	sudo dmesg

rmmod:
	sudo dmesg -C
	sudo rmmod my_ops
	sudo dmesg

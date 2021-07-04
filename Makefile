obj-m  = m2.o
kernel-release = 5.4.0-1-generic
all: 
	make -C /lib/modules/$(kernel-release)/build M=$(shell pwd) modules
	@echo "we are doing bash after make!!!"
	sh afterMake.sh
clean:
	make -C /lib/modules/$(kernel-release)/build M=$(shell pwd) clean

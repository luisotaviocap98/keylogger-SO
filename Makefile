obj-m += modulo.o
 
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd | sed 's/ /\\ /g')
 
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
 
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
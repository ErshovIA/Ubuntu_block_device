TARGET := kBlk_ErshovIA
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
$(TARGET)-objs += Blk_ErshovIA.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean

obj-m += rvgpu.o

rvgpu-objs += rvgpu_module.o

ccflags-y += -Werror

all:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean

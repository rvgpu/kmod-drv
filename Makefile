obj-m += rvgpu.o

rvgpu-objs += rvgpu_module.o
rvgpu-objs += rvgpu_drm.o
rvgpu-objs += rvgpu_dma_buf.o

ccflags-y += -Werror

all:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean

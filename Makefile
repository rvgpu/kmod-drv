obj-m += rvgpu.o

rvgpu-objs += rvgpu_drv.o rvgpu_drm.o rvgpu_device.o rvgpu_dma_buf.o

ccflags-y += -Werror

all:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean

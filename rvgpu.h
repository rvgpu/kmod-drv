#ifndef __RVGPU_H__
#define __RVGPU_H__

#include "drm/drm_device.h"

#include "rvgpu_drv.h"

struct rvgpu_cli {
    struct rvgpu_device     *rdev;
    char                    prog_name[TASK_COMM_LEN];
    int                     prog_pid;
};

struct rvgpu_register {
    resource_size_t         base;
    resource_size_t         size;
    void __iomem            *mmio;
};

struct rvgpu_device {
    struct device           *dev;
    struct pci_dev          *pdev;
    struct drm_device       ddev;

    struct rvgpu_register   regs;
};

static inline struct rvgpu_device *drm_to_rdev(struct drm_device *ddev)
{
    return container_of(ddev, struct rvgpu_device, ddev);
}

static inline struct drm_device *rdev_to_drm(struct rvgpu_device *rdev)
{
    return &rdev->ddev;
}


/* DRM Interface */
int rvgpu_driver_open(struct drm_device *dev, struct drm_file *file_priv);
void rvgpu_driver_postclose(struct drm_device *dev, struct drm_file *file_priv);
void rvgpu_driver_release(struct drm_device *dev);

/* RVGPU Device */
int rvgpu_device_init(struct rvgpu_device *rdev, uint32_t flags);
void rvgpu_device_fini(struct rvgpu_device *rdev);

static inline void rvgpu_device_write_register(struct rvgpu_device *rdev, uint64_t regid, uint32_t data) {
    void *addr = (void *)(rdev->regs.mmio + regid);
    writel(data, addr);
}

static inline uint32_t rvgpu_device_read_register(struct rvgpu_device *rdev, uint64_t regid) {
    void *addr = (void *)(rdev->regs.mmio + regid);
    return readl(addr);
}

#endif

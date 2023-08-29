#ifndef __RVGPU_H__
#define __RVGPU_H__

#include "rvgpu_drv.h"

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

#endif

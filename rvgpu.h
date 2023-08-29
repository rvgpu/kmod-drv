#ifndef __RVGPU_H__
#define __RVGPU_H__

#include "rvgpu_drv.h"

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

#endif

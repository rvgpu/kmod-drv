#ifndef __RVGPU_H__
#define __RVGPU_H__

#include "drm/drm_file.h"
#include "drm/drm_buddy.h"
#include "drm/drm_device.h"
#include "drm/ttm/ttm_device.h"

#include "rvgpu_drv.h"

struct rvgpu_cli {
    struct rvgpu_device     *rdev;
    char                    prog_name[TASK_COMM_LEN];
    int                     prog_pid;

    struct list_head        head;
};

struct rvgpu_register {
    resource_size_t         base;
    resource_size_t         size;
    void __iomem            *mmio;
};

struct rvgpu_vram_info {
    u64                     base;
    u64                     size;
};

struct rvgpu_ttm {
    struct ttm_device       bdev;   // bo dev ?
 
    struct mutex            io_reserve_mutex;
    struct list_head        io_reserve_lru;

    int mttr;
};

struct rvgpu_device {
    struct device           *dev;
    struct pci_dev          *pdev;
    struct drm_device       ddev;

    // Register space
    struct rvgpu_register   regs;
    
    // VRAM space
    struct rvgpu_vram_info  vraminfo;
    struct rvgpu_ttm        ttm;
    struct ttm_resource_manager manager;
    struct drm_buddy        mm;

    // CLients
    struct list_head        clients;
    struct mutex            clients_lock;
};

static inline struct rvgpu_device *drm_to_rdev(struct drm_device *ddev)
{
    return container_of(ddev, struct rvgpu_device, ddev);
}

static inline struct drm_device *rdev_to_drm(struct rvgpu_device *rdev)
{
    return &rdev->ddev;
}

static inline struct rvgpu_cli *rvgpu_cli(struct drm_file *file_priv) {
    return file_priv ? file_priv->driver_priv : NULL;
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

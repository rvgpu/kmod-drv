#ifndef __RVGPU_BO_H__
#define __RVGPU_BO_H__

#include <drm/drm_gem.h>
#include <drm/ttm/ttm_bo_driver.h>

#include "rvgpu.h"

struct rvgpu_bo {
    struct ttm_buffer_object bo;
    struct ttm_placement placement;

    struct list_head head;

    /* protected by ttm_bo_reserve() */
    struct drm_file *reserved_by;
};

extern struct ttm_device_funcs rvgpu_bo_driver;

struct rvgpu_bo *rvgpu_bo_alloc(struct rvgpu_cli *cli, u64 *size, int *align, u32 domain, u32 flags);

#endif // __RVGPU_BO_H__

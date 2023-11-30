#ifndef __RVGPU_BO_H__
#define __RVGPU_BO_H__

#include <drm/drm_gem.h>
#include <drm/ttm/ttm_bo_driver.h>

#include "rvgpu.h"

struct rvgpu_bo {
    struct ttm_buffer_object bo;
    struct ttm_placement placement;
    u32 valid_domains;
    struct ttm_place placements[3];
    struct ttm_place busy_placements[3];

    struct list_head head;
    struct list_head io_reserve_lru;

    /* protected by ttm_bo_reserve() */
    struct drm_file *reserved_by;
};

static inline struct rvgpu_bo *rvgpu_bo(struct ttm_buffer_object *bo)
{
    return container_of(bo, struct rvgpu_bo, bo);
}

extern struct ttm_device_funcs rvgpu_bo_driver;

struct rvgpu_bo *rvgpu_bo_alloc(struct rvgpu_cli *cli, u64 *size,
        int *align, u32 domain, u32 flags);

int rvgpu_bo_init(struct rvgpu_bo *rbo, u64 size, int align, u32 domain, 
        struct sg_table *sg, struct dma_resv *robj);

#endif // __RVGPU_BO_H__

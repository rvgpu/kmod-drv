#include <drm/ttm/ttm_tt.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_device.h>
#include <drm/ttm/ttm_bo_api.h>

#include "rvgpu.h"
#include "rvgpu_drm.h"
#include "rvgpu_ttm.h"
#include "rvgpu_bo.h"

static void rvgpu_bo_del_ttm(struct ttm_buffer_object *bo)
{
    struct rvgpu_device *rdev = rvgpu_bdev(bo->bdev);
    struct rvgpu_bo *rbo = rvgpu_bo(bo);

    WARN_ON(rbo->bo.pin_count > 0);

    mutex_lock(&rdev->ttm.io_reserve_mutex);
    list_move_tail(&rbo->io_reserve_lru, &rdev->ttm.io_reserve_lru);
    mutex_unlock(&rdev->ttm.io_reserve_mutex);

    /* If rvgpu_bo_new() allocated this buffer, the GEM object was never initialized,
     * so don't attempt to release it.
     */
    if (bo->base.dev) {
        drm_gem_object_release(&bo->base);
    } else {
        dma_resv_fini(&bo->base._resv);
    }

    kfree(rbo);
}

static void set_placement_list(struct ttm_place *pl, unsigned *n, uint32_t domain)
{
    *n = 0;

    if (domain & RVGPU_GEM_DOMAIN_VRAM) {
        pl[*n].mem_type = TTM_PL_VRAM;
        pl[*n].flags = 0;
        (*n)++;
    }

    if (domain & RVGPU_GEM_DOMAIN_GART) {
        pl[*n].mem_type = TTM_PL_TT;
        pl[*n].flags = 0;
        (*n)++;
    }

    if (domain & RVGPU_GEM_DOMAIN_CPU) {
        pl[*n].mem_type = TTM_PL_SYSTEM;
        pl[(*n)++].flags = 0;
    }
}

static void set_placement_range(struct rvgpu_bo *rbo, uint32_t domain)
{
    struct rvgpu_device *dev = rvgpu_bdev(rbo->bo.bdev);
    u64 vram_size = dev->vraminfo.size;
    unsigned i, fpfn, lpfn;

    if ((domain & RVGPU_GEM_DOMAIN_VRAM) && (rbo->bo.base.size < vram_size / 4)) {
        fpfn = 0;
        lpfn = (vram_size / 2) >> PAGE_SHIFT;

        for (i=0; i<rbo->placement.num_placement; ++i) {
            rbo->placements[i].fpfn = fpfn;
            rbo->placements[i].lpfn = lpfn;
        }

        for (i=0; i<rbo->placement.num_busy_placement; i++) {
            rbo->busy_placements[i].fpfn = fpfn;
            rbo->busy_placements[i].lpfn = lpfn;
        }
    }
}

void rvgpu_bo_placement_set(struct rvgpu_bo *rbo, uint32_t domain, uint32_t busy)
{
    struct ttm_placement *pl = &rbo->placement;

    pl->placement = rbo->placements;
    set_placement_list(rbo->placements, &pl->num_placement, domain);

    pl->busy_placement = rbo->busy_placements;
    set_placement_list(rbo->busy_placements, &pl->num_busy_placement, domain | busy);

    set_placement_range(rbo, domain);
}

int rvgpu_bo_init(struct rvgpu_bo *rbo, u64 size, int align, u32 domain,
        struct sg_table *sg, struct dma_resv *robj) 
{
    int type = sg ? ttm_bo_type_sg : ttm_bo_type_device;
    int ret = 0;

    rvgpu_bo_placement_set(rbo, domain, 0);
    INIT_LIST_HEAD(&rbo->io_reserve_lru);

    ret = ttm_bo_init_validate(rbo->bo.bdev, &rbo->bo, type, &rbo->placement,
                align >> PAGE_SHIFT, false, sg, robj, rvgpu_bo_del_ttm);
    if (ret) {
        printk("ttm_bo_init_validate error\n");
        return ret;
    }

    return ret;
}

struct rvgpu_bo *
rvgpu_bo_alloc(struct rvgpu_cli *cli, u64 *size, int *align, u32 domain, u32 flags) {
    struct rvgpu_device *rdev = cli->rdev;
    struct rvgpu_bo *rbo;

    if (!*size) {
        printk("skipped size %016llx\n", *size);
        return ERR_PTR(-EINVAL);
    }

    rbo = kzalloc(sizeof(struct rvgpu_bo), GFP_KERNEL);
    if (!rbo) {
        return ERR_PTR(-ENOMEM);
    }

    rbo->bo.bdev = &rdev->ttm.bdev;
    return rbo;
}

static int
rvgpu_bo_move(struct ttm_buffer_object *bo, bool evict,
              struct ttm_operation_ctx *ctx,
              struct ttm_resource *new_reg,
              struct ttm_place *hpp) {
    printk("rvgpu_bo_move TODO\n");
    return 0;
}

static int
rvgpu_ttm_io_mem_reserve(struct ttm_device *bdev, struct ttm_resource *reg)
{
    printk("rvgpu_ttm_io_mem_reserve TODO\n");
    return 0;
}

static void
rvgpu_ttm_io_mem_free(struct ttm_device *bdev, struct ttm_resource *reg)
{
    printk("rvgpu_ttm_io_mem_free TODO\n");
}

static void
rvgpu_bo_evict_flags(struct ttm_buffer_object *bo, struct ttm_placement *pl)
{
    printk("rvgpu_bo_evict_flags TODO\n");
}

static struct ttm_tt *
rvgpu_ttm_tt_create(struct ttm_buffer_object *bo, uint32_t page_flags)
{
    printk("rvgpu_ttm_tt_create TODO\n");
    return NULL;
}

static int
rvgpu_ttm_tt_populate(struct ttm_device *bdev, struct ttm_tt *ttm, struct ttm_operation_ctx *ctx)
{
    printk("rvgpu_ttm_tt_populate TODO\n");

    return 0;
}

static void
rvgpu_ttm_tt_unpopulate(struct ttm_device *bdev, struct ttm_tt *ttm)
{
    printk("rvgpu_ttm_tt_unpopulate TODO\n");
}

static void
rvgpu_ttm_tt_destroy(struct ttm_device *bdev, struct ttm_tt *ttm)
{
    printk("rvgpu_ttm_tt_destroy TODO\n");
}

static void
rvgpu_bo_delete_mem_notify(struct ttm_buffer_object *bo)
{
    printk("rvgpu_bo_delete_mem_notify TODO\n");
}

struct ttm_device_funcs rvgpu_bo_driver = {
    .ttm_tt_create =        &rvgpu_ttm_tt_create,
    .ttm_tt_populate =      &rvgpu_ttm_tt_populate,
    .ttm_tt_unpopulate =    &rvgpu_ttm_tt_unpopulate,
    .ttm_tt_destroy =       &rvgpu_ttm_tt_destroy,
    .eviction_valuable =    ttm_bo_eviction_valuable,
    .evict_flags =          rvgpu_bo_evict_flags,
    .delete_mem_notify =    rvgpu_bo_delete_mem_notify,
    .move =                 rvgpu_bo_move,
    .io_mem_reserve =       &rvgpu_ttm_io_mem_reserve,
    .io_mem_free =          &rvgpu_ttm_io_mem_free,
};

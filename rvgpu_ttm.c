#include "rvgpu.h"
#include "rvgpu_ttm.h"
#include "rvgpu_bo.h"

static void rvgpu_manager_del(struct ttm_resource_manager *man, struct ttm_resource *reg)
{
    printk("%s TODO\n", __func__);
}

static bool rvgpu_manager_intersects(struct ttm_resource_manager *man,
            struct ttm_resource *res,
            const struct ttm_place *place,
            size_t size)
{
    printk("%s TODO\n", __func__);
    return false;
}

static bool rvgpu_manager_compatible(struct ttm_resource_manager *man,
            struct ttm_resource *res,
            const struct ttm_place *place,
            size_t size)
{
    printk("%s TODO\n", __func__);
    return false;
}

static int rvgpu_vram_manager_new(struct ttm_resource_manager *man,
            struct ttm_buffer_object *bo,
            const struct ttm_place *place,
            struct ttm_resource **res)
{
    // struct rvgpu_bo *rbo = rvgpu_bo(bo);
    struct rvgpu_device *rdev = rvgpu_bdev(bo->bdev);
    int ret = 0;

    if (rdev->vraminfo.size == 0) {
        return -ENOMEM;
    }

    printk("%s TODO\n", __func__);
    return ret;
}

const struct ttm_resource_manager_func rvgpu_vram_manager = {
    .alloc = rvgpu_vram_manager_new,
    .free = rvgpu_manager_del,
    .intersects = rvgpu_manager_intersects,
    .compatible = rvgpu_manager_compatible,
};

static int rvgpu_ttm_init_vram(struct rvgpu_device *rdev)
{
    struct ttm_resource_manager *man = kzalloc(sizeof(*man), GFP_KERNEL);
    if (!man) {
        return -ENOMEM;
    }

    man->func = &rvgpu_vram_manager;

    ttm_resource_manager_init(man, &rdev->ttm.bdev, rdev->vraminfo.size >> PAGE_SHIFT);
    ttm_set_driver_manager(&rdev->ttm.bdev, TTM_PL_VRAM, man);
    ttm_resource_manager_set_used(man, true);

    return 0;
}

int rvgpu_ttm_init(struct rvgpu_device *rdev)
{
    int ret = 0;
    struct drm_device *ddev = rdev_to_drm(rdev);

    // ttm_device_init
    // bdev: A pointer to a struct ttm_device_to_initialize.
    // funcs: Function table for the device.
    // dev: The core kernel device pointer for DMA mappings and allocations.
    // mapping: The address space to use for this bo.
    // vma_manager: A pointer to a vma manager.
    // use_dma_alloc: If cherent DMA allocation API should be used.
    // use_dma32: If we should use GFP_DMA32 for device memory allocations.
    ret = ttm_device_init(&rdev->ttm.bdev,
                          &rvgpu_bo_driver,  
                          rdev->dev,
                          ddev->anon_inode->i_mapping,
                          ddev->vma_offset_manager,
                          0,
                          0);

    if (ret) {
        printk("error to initialising bo driver, %d\n", ret);
        return ret;
    }
    
    /* VRAM init */
    arch_io_reserve_memtype_wc(rdev->vraminfo.base, rdev->vraminfo.size);
    ret = rvgpu_ttm_init_vram(rdev);
    if (ret) {
        printk("VRAM mm init failed, %d\n", ret);
        return ret;
    } 

    rdev->ttm.mttr = arch_phys_wc_add(rdev->vraminfo.base, rdev->vraminfo.size);

    // GART init TODO
    mutex_init(&rdev->ttm.io_reserve_mutex);
    INIT_LIST_HEAD(&rdev->ttm.io_reserve_lru);

    printk("VRAM: %d MiB\n", (u32)(rdev->vraminfo.size >> 20));
    return 0;
}

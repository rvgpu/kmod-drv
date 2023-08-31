#include "rvgpu.h"
#include "rvgpu_ttm.h"
#include "rvgpu_bo.h"

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

    return 0;
}

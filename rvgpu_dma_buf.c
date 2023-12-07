#include <linux/dma-buf.h>

#include "rvgpu_debug.h"
#include "rvgpu_dma_buf.h"

/**
 * rvgpu_gem_prim_import - &drm_driver.gem_prim_import implementation
 * @dev: DRM Device
 * @dma_buf: Shared DMA buffer
 * 
 * Import a dma_buf into a the driver and potentially create a new GEM object.
 *
 * Returns:
 * GEM BO representing the shared DMA buffer for the given device.
 */
struct drm_gem_object *rvgpu_gem_prime_import(struct drm_device *dev, struct dma_buf *dma_buf)
{
    RVGPU_FUNC_TODO();

    return NULL;
}

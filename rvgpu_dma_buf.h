#ifndef __RVGPU_DMA_BUF_H__
#define __RVGPU_DMA_BUF_H__

#include <drm/drm_gem.h>

struct drm_gem_object *rvgpu_gem_prime_import(struct drm_device *dev, struct dma_buf *dma_buf);

#endif // __RVGPU_DMA_BUF_H__

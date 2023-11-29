#include <linux/dma-buf.h>

#include "rvgpu_drv.h"
#include "rvgpu_gem.h"

struct sg_table *rvgpu_gem_prime_get_sg_table(struct drm_gem_object *obj)
{
    printk("%s TODO\n", __func__);
    return NULL;
}

int rvgpu_gem_prime_pin(struct drm_gem_object *obj)
{
    printk("%s TODO\n", __func__);
    return 0;
}

void rvgpu_gem_prime_unpin(struct drm_gem_object *obj)
{
    printk("%s TODO\n", __func__);
}



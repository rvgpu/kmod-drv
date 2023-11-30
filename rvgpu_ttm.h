#ifndef __RVGPU_TTM_H__
#define __RVGPU_TTM_H__

#include "rvgpu.h"

static inline struct rvgpu_device* rvgpu_bdev(struct ttm_device *bd) {
    return container_of(bd, struct rvgpu_device, ttm.bdev);
}

int rvgpu_ttm_init(struct rvgpu_device *rdev);

#endif

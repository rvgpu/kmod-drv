#ifndef __RVGPU_TTM_H__
#define __RVGPU_TTM_H__

#include "drm/ttm/ttm_tt.h"

#include "rvgpu.h"

struct rvgpu_ttm_tt {
    struct ttm_tt               ttm;
    struct drm_gem_object       *gobj;
};

static inline struct rvgpu_device* rvgpu_bdev(struct ttm_device *bd) {
    return container_of(bd, struct rvgpu_device, ttm.bdev);
}

int rvgpu_ttm_init(struct rvgpu_device *rdev);

#endif

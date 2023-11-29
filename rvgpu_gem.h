#ifndef __RVGPU_GEM_H__
#define __RVGPU_GEM_H__

#include "rvgpu_bo.h"

int rvgpu_gem_new(struct rvgpu_cli *cli, u64 size, int align, u32 domain, u32 flags, struct rvgpu_bo **prbo);

int rvgpu_ioctl_gem_new(struct drm_device *ddev, void *data, struct drm_file *fpriv);

#endif // __RVGPU_GEM_H__

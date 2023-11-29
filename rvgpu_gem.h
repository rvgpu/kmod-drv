#ifndef __RVGPU_GEM_H__
#define __RVGPU_GEM_H__

#include "rvgpu_bo.h"

int rvgpu_gem_new(struct rvgpu_cli *cli, u64 size, int align, u32 domain, u32 flags, struct rvgpu_bo **prbo);

int rvgpu_ioctl_gem_new(struct drm_device *ddev, void *data, struct drm_file *fpriv);


// Prime function
extern int rvgpu_gem_prime_pin(struct drm_gem_object *);
extern void rvgpu_gem_prime_unpin(struct drm_gem_object *);
extern struct sg_table *rvgpu_gem_prime_get_sg_table(struct drm_gem_object *);
extern struct drm_gem_object *rvgpu_gem_prime_import_sg_table(
        struct drm_device *, struct dma_buf_attachment *, struct sg_table);

#endif // __RVGPU_GEM_H__

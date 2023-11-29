#include <drm/drm_gem_ttm_helper.h>
#include <drm/drm_device.h>

#include "rvgpu.h"
#include "rvgpu_drm.h"
#include "rvgpu_gem.h"

static vm_fault_t rvgpu_ttm_fault(struct vm_fault *vmf)
{
    return 0;
}

static const struct vm_operations_struct rvgpu_ttm_vm_ops = {
    .fault = rvgpu_ttm_fault,
    .open = ttm_bo_vm_open,
    .close = ttm_bo_vm_close,
    .access = ttm_bo_vm_access,
};

void rvgpu_gem_object_del(struct drm_gem_object *gem)
{
    printk("%s TODO\n", __func__);
}

int rvgpu_gem_object_open(struct drm_gem_object *gem, struct drm_file *file_priv)
{
    printk("%s TODO\n", __func__);
    return 0;
}

void rvgpu_gem_object_close(struct drm_gem_object *gem, struct drm_file *file_priv)
{
}

const struct drm_gem_object_funcs rvgpu_gem_object_funcs = {
    .free = rvgpu_gem_object_del,
    .open = rvgpu_gem_object_open,
    .close = rvgpu_gem_object_close,
    .pin = rvgpu_gem_prime_pin,
    .unpin = rvgpu_gem_prime_unpin,
    .get_sg_table = rvgpu_gem_prime_get_sg_table,
    .vmap = drm_gem_ttm_vmap,
    .vunmap = drm_gem_ttm_vunmap,
    .mmap = drm_gem_ttm_mmap,
    .vm_ops = &rvgpu_ttm_vm_ops,
};

int
rvgpu_gem_new(struct rvgpu_cli *cli, u64 size, int align, u32 domain, u32 flags, struct rvgpu_bo **prbo)
{
    struct rvgpu_bo *rbo;
    int ret = 0;

    rbo = rvgpu_bo_alloc(cli, &size, &align, domain, flags);
    if (IS_ERR(rbo)) {
        return PTR_ERR(rbo);
    }

    rbo->bo.base.funcs = &rvgpu_gem_object_funcs;
    return ret;
}

int 
rvgpu_ioctl_gem_new(struct drm_device *ddev, void *data, struct drm_file *file_priv)
{
    struct rvgpu_cli *cli = rvgpu_cli(file_priv);
    struct drm_rvgpu_gem_new *req = data;
    struct rvgpu_bo *rbo = NULL;
    int ret = 0;

    ret = rvgpu_gem_new(cli, req->in.size, req->in.alignment, req->in.domains, req->in.flags, &rbo);
    return ret;
}

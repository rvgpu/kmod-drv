#include <drm/drm_gem_ttm_helper.h>
#include <drm/drm_device.h>

#include "rvgpu.h"
#include "rvgpu_debug.h"
#include "rvgpu_drm.h"
#include "rvgpu_gem.h"

void rvgpu_gem_object_del(struct drm_gem_object *gem)
{
    RVGPU_FUNC_TODO();
}

int rvgpu_gem_object_open(struct drm_gem_object *gem, struct drm_file *file_priv)
{
    RVGPU_FUNC_TODO();
    return 0;
}

void rvgpu_gem_object_close(struct drm_gem_object *gem, struct drm_file *file_priv)
{
    RVGPU_FUNC_TODO();
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
    .print_info = drm_gem_ttm_print_info,
};

int
rvgpu_gem_new(struct rvgpu_cli *cli, u64 size, int align, u32 domain, u32 flags, struct rvgpu_bo **prbo)
{
    struct rvgpu_device *rdev = cli->rdev;
    struct rvgpu_bo *rbo;
    int ret = 0;

    RVGPU_FUNC_BEGIN();
    rbo = rvgpu_bo_alloc(cli, &size, &align, domain, flags);
    if (IS_ERR(rbo)) {
        return PTR_ERR(rbo);
    }

    rbo->bo.base.funcs = &rvgpu_gem_object_funcs;

    /* Initialize the embedded gem-object.
     * We return a single gem-reference to the caller, instead of a normal rvgpu_bo ttm_reference.
     */
    ret = drm_gem_object_init(&rdev->ddev, &rbo->bo.base, size);
    if (ret) {
        drm_gem_object_release(&rbo->bo.base);
        kfree(rbo);
        return ret;
    }

    ret = rvgpu_bo_init(rbo, size, align, domain, NULL, NULL);

    *prbo = rbo;

    RVGPU_FUNC_END();
    return ret;
}

int 
rvgpu_ioctl_gem_new(struct drm_device *ddev, void *data, struct drm_file *file_priv)
{
    struct rvgpu_cli *cli = rvgpu_cli(file_priv);
    struct drm_rvgpu_gem_new *req = data;
    struct rvgpu_bo *rbo = NULL;
    int ret = 0;

    RVGPU_FUNC_BEGIN();
    ret = rvgpu_gem_new(cli, req->in.size, req->in.alignment, req->in.domains, req->in.flags, &rbo);
    if (ret) {
        return ret;
    }

    ret = drm_gem_handle_create(file_priv, &rbo->bo.base, &req->out.handle);
    if (ret == 0) {
        req->out.offset = drm_vma_node_offset_addr(&rbo->bo.base.vma_node);
    }

    drm_gem_object_put(&rbo->bo.base);

    RVGPU_FUNC_END();
    return ret;
}

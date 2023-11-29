#include <drm/drm_device.h>

#include "rvgpu.h"
#include "rvgpu_drm.h"
#include "rvgpu_gem.h"

int
rvgpu_gem_new(struct rvgpu_cli *cli, u64 size, int align, u32 domain, u32 flags, struct rvgpu_bo **prbo)
{
    struct rvgpu_bo *rbo;
    int ret = 0;

    rbo = rvgpu_bo_alloc(cli, &size, &align, domain, flags);
    if (IS_ERR(rbo)) {
        return PTR_ERR(rbo);
    }

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

#include <sys/ioctl.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "xf86drm.h"
#include "rvgpu_drm.h"

struct rvgpu_bo {
    struct nouveau_device *device;
    uint32_t handle;
    uint64_t size;
    uint32_t flags;
    uint64_t offset;
    void *map; 
};

static int rvgpu_bo_new(int fd, uint32_t flags, uint32_t align, uint64_t size, struct rvgpu_bo *bo)
{
    int ret = 0;
    struct drm_rvgpu_gem_new req = {};
    struct drm_rvgpu_gem_info *info = &req.info;

    info->domain = flags;
    info->size = size;
    req.align = align;

    ret = drmCommandWriteRead(fd, DRM_RVGPU_CMD_GEM_NEW, &req, sizeof(req));
    if (ret != 0) {
        printf("gem new error\n");
        return ret;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    drmVersionPtr version;
    const char *device = NULL;
    int err, fd;
    fd = drmOpenWithType("rvgpu", NULL, DRM_NODE_RENDER);
    if (fd < 0) {
        fprintf(stderr, "Opening rvgpu render node failed with %i\n", fd);
        return fd;
    }

    version = drmGetVersion(fd);
    if (version) {
        printf("Version: %d.%d.%d\n", version->version_major,
                version->version_minor, version->version_patchlevel);
        printf("  Name: %s\n", version->name);
        printf("  Date: %s\n", version->date);
        printf("  Description: %s\n", version->desc);
        drmFreeVersion(version);
    }

    struct rvgpu_bo bo;
    err = rvgpu_bo_new(fd, RVGPU_GEM_BO_VRAM, 0, 4096, &bo);
#if 0
	err = nouveau_device_wrap(fd, 0, &nvdev);
	if (!err)
		err = nouveau_device_wrap(fd2, 0, &nvdev2);
	if (err < 0)
		return 1;

	err = nouveau_bo_new(nvdev2, RVGPU_BO_GART, 0, 4096, NULL, &bo);
	if (!err)
		err = nouveau_bo_set_prime(bo, &import_fd);

	if (!err) {
		pthread_create(&t1, NULL, openclose, nvdev);
		pthread_create(&t2, NULL, openclose, nvdev);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	close(import_fd);
	nouveau_bo_ref(NULL, &bo);

	nouveau_device_del(&nvdev2);
	nouveau_device_del(&nvdev);
	if (device) {
		close(fd2);
		close(fd);
	} else {
		drmClose(fd2);
		drmClose(fd);
	}

	if (failed)
		fprintf(stderr, "DRM_IOCTL_GEM_CLOSE failed with EINVAL,\n"
				"race in opening/closing bo is likely.\n");

	return failed;
#endif
}

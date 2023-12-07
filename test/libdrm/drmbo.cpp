#include <sys/mman.h>
#include <sys/ioctl.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "xf86drm.h"
#include "rvgpu_drm.h"

static void *rvgpu_bo_new(int fd, uint32_t size, uint32_t align, uint32_t domains, uint32_t flags)
{
    int ret = 0;
    struct drm_rvgpu_gem_new req = {};

    req.in.size = size;
    req.in.alignment = align;
    req.in.domains = domains;
    req.in.flags = flags;

    ret = drmCommandWriteRead(fd, DRM_RVGPU_CMD_GEM_NEW, &req, sizeof(req));
    if (ret != 0) {
        printf("gem new error\n");
        return NULL;
    }

    void *addr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, req.out.offset);
    if (addr == MAP_FAILED) {
        return NULL;
    }

    return addr;
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

    uint32_t size = 4096;
    void *addr = rvgpu_bo_new(fd, size, 0, RVGPU_GEM_DOMAIN_VRAM, 0);
    for (uint32_t i=0; i<size/4; i++) {
        uint32_t *buf = (uint32_t *)addr;
        buf[i] = i;
        if (buf[i] != i) {
            printf("error to write vram\n");
        }
    }
}

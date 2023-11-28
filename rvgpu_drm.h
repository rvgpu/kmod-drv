#ifndef __RVGPU_DRM_H__
#define __RVGPU_DRM_H__

/** Bo Flags **/
#define RVGPU_GEM_BO_VRAM                   0x00000001
#define RVGPU_GEM_BO_GART                   0x00000002
#define RVGPU_GEM_BO_APER                   (RVGPU_GEM_BO_VRAM | RVGPU_GEM_BO_GART)
#define RVGPU_GEM_BO_RD                     0x00000100
#define RVGPU_GEM_BO_WR                     0x00000200
#define RVGPU_GEM_BO_RDWR                   (RVGPU_GEM_BO_RD | RVGPU_GEM_BO_WR)
#define RVGPU_GEM_BO_NOBLOCK                0x00000400
#define RVGPU_GEM_BO_LOW                    0x00001000
#define RVGPU_GEM_BO_HIGH                   0x00002000
#define RVGPU_GEM_BO_OR                     0x00004000
#define RVGPU_GEM_BO_MAP                    0x80000000
#define RVGPU_GEM_BO_CONTIG                 0x40000000
#define RVGPU_GEM_BO_NOSNOOP                0x20000000
#define RVGPU_GEM_BO_COHERENT               0x10000000

/** DRM Ioctl Commands **/
#define DRM_RVGPU_CMD_GETPARAM              0x00
#define DRM_RVGPU_CMD_SETPARAM              0x01
#define DRM_RVGPU_CMD_CHANNEL_ALLOC         0x02
#define DRM_RVGPU_CMD_CHANNEL_FREE          0x03
#define DRM_RVGPU_CMD_GROBJ_ALLOC           0x04
#define DRM_RVGPU_CMD_NOTIFIEROBJ_ALLOC     0x05
#define DRM_RVGPU_CMD_GPUOBJ_FREE           0x06
#define DRM_RVGPU_CMD_NVIF                  0x07
#define DRM_RVGPU_CMD_SVM_INIT              0x08
#define DRM_RVGPU_CMD_SVM_BIND              0x09
#define DRM_RVGPU_CMD_GEM_NEW               0x40
#define DRM_RVGPU_CMD_GEM_PUSHBUF           0x41
#define DRM_RVGPU_CMD_GEM_CPU_PREP          0x42
#define DRM_RVGPU_CMD_GEM_CPU_FINI          0x43
#define DRM_RVGPU_CMD_GEM_INFO              0x44

/** Interface Structuract **/
struct drm_rvgpu_gem_new {
    /* in */
    struct {
        __u64   size;
        __u64   alignment;
        __u64   domains;
        __u64   flags;
    } in;
    /* out */
    struct {
        __u32   handle;
        __u32   _pad;
    } out;
}; 

#define IOCTLCMD_WR(cmd, type)              DRM_IOWR(DRM_COMMAND_BASE + cmd, type)
#define DRM_IOCTL_RVGPU_CMD_GEM_NEW         IOCTLCMD_WR(DRM_RVGPU_CMD_GEM_NEW, struct drm_rvgpu_gem_new)

#endif // __RVGPU_DRM_H__

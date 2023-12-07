#ifndef __RVGPU_DEBUG_H__
#define __RVGPU_DEBUG_H__

#define RVGPU_DEBUG 0

#if RVGPU_DEBUG
#define RVGPU_DEBUG_INFO(format, arg...)    printk(format, ##arg)
#else
#define RVGPU_DEBUG_INFO(format, arg...)
#endif

#define RVGPU_FUNC_BEGIN()                  RVGPU_DEBUG_INFO("%s begin\n", __func__)
#define RVGPU_FUNC_END()                    RVGPU_DEBUG_INFO("%s end\n", __func__)
#define RVGPU_FUNC_TODO()                   printk("%s TODO !\n", __func__)

#define RVGPU_ERROR(format, arg...)         printk(format, ##arg)

#endif

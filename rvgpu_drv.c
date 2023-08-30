#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_gem.h>

#include "rvgpu.h"
#include "rvgpu_dma_buf.h"
#include "rvgpu_drv.h"

static const struct drm_ioctl_desc rvgpu_ioctls[] = {
};

DEFINE_DRM_GEM_FOPS(rvgpu_driver_fops);

static const struct drm_driver rvgpu_drm_driver = {
    .driver_features = 
        DRIVER_GEM |
        DRIVER_RENDER,
    .open = rvgpu_driver_open,
    .postclose = rvgpu_driver_postclose,
    .lastclose = NULL,
    .ioctls = rvgpu_ioctls,
    .num_ioctls = ARRAY_SIZE(rvgpu_ioctls),
    .fops = &rvgpu_driver_fops,
    .release = &rvgpu_driver_release,
    .prime_handle_to_fd = drm_gem_prime_handle_to_fd,
    .prime_fd_to_handle = drm_gem_prime_fd_to_handle,
    .gem_prime_import  = rvgpu_gem_prime_import,
    .gem_prime_mmap = drm_gem_prime_mmap,
    
    .name = "rvgpu",
    .desc = "rvgpu DRM",
    .date = "20230829",
    .major = 1,
    .minor = 0,
};

static int rvgpu_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    struct drm_device *ddev;
    struct rvgpu_device *rdev;
    unsigned long flags = id->driver_data;
    int ret = 0;

    rdev = devm_drm_dev_alloc(&pdev->dev, &rvgpu_drm_driver, typeof(*rdev), ddev);
    if (IS_ERR(rdev)) {
        return PTR_ERR(rdev);
    }

    rdev->dev = &pdev->dev;
    rdev->pdev = pdev;
    ddev = rdev_to_drm(rdev);

    ret = pci_enable_device(pdev);
    if (ret) {
        return ret;
    }

    pci_set_drvdata(pdev, ddev);
    rvgpu_device_init(rdev, flags);

    ret = drm_dev_register(ddev, flags);
    if (ret == -EAGAIN) {
        DRM_INFO("register drm dev error\n");
        goto err_pci;
    } 

    printk("rvgpu pci probe done\n");

    return 0;
err_pci:
    pci_disable_device(pdev);
    return ret;
}

static void rvgpu_pci_remove(struct pci_dev *pdev) {
    struct drm_device *ddev = pci_get_drvdata(pdev);
    struct rvgpu_device *rdev = drm_to_rdev(ddev);

    drm_dev_unplug(ddev);

    rvgpu_device_fini(rdev);

    pci_disable_device(pdev);
    pci_wait_for_pending_transaction(pdev);
    printk("rvgpu pci remove done\n");
}

static struct pci_device_id rvgpu_id_tbl[] = {
    {PCI_DEVICE(RVGPU_HW_VENDOR_ID, RVGPU_HW_DEVICE_ID)},
    {0,},
};

MODULE_DEVICE_TABLE(pci, rvgpu_id_tbl);

static struct pci_driver rvgpu_pci_driver = {
    .name = "rvgpu",
    .id_table = rvgpu_id_tbl,
    .probe = rvgpu_pci_probe,
    .remove = rvgpu_pci_remove,
};

drm_module_pci_driver(rvgpu_pci_driver);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Kernel module to drive the rvgpu device");
MODULE_AUTHOR("Qi Hangkong <qihangkong@outlook.com>");

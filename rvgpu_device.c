#include <linux/pci.h>
#include <drm/drm_print.h>
#include <drm/drm_device.h>

#include "rvgpu.h"
#include "rvgpu_ttm.h"

/**
 * rvgpu_device_init - initialize the driver
 *
 * @rdev: rvgpu_device pointer
 * @flags: driver flags
 *
 * Initializes the driver info and hw (all asics).
 * Returns 0 for success or an error on failure.
 * Called at driver startup.
 */
int rvgpu_device_init(struct rvgpu_device *rdev, uint32_t flags)
{
    // struct drm_device *ddev = rdev_to_drm(rdev);
    struct pci_dev *pdev = rdev->pdev;

    int ret = 0;

    // Registers mapping
    rdev->regs.base = pci_resource_start(pdev, 2);
    rdev->regs.size = pci_resource_len(pdev, 2);
    rdev->regs.mmio = ioremap(rdev->regs.base, rdev->regs.size);
    if (rdev->regs.mmio == NULL) {
        return -ENOMEM;
    }

    DRM_INFO("register mmio base: 0x%08x, size: %u\n", 
            (uint32_t)rdev->regs.base, (unsigned)rdev->regs.size);

    // initialize VRAM
    rdev->vraminfo.base = pci_resource_start(pdev, 0);
    rdev->vraminfo.size = pci_resource_len(pdev, 0);
    rvgpu_ttm_init(rdev);

    return ret;
}

static void rvgpu_device_unmap_mmio(struct rvgpu_device *rdev)
{
    /* Clear all CPU mapping pointing to this device */
    iounmap(rdev->regs.mmio);
    rdev->regs.mmio = NULL;
}

/**
 * rvgpu_device_fini - tear down the driver
 * 
 * @rdev: rvgpu_device pointer
 * 
 * Tear down the driver info.
 * Called at driver shutdown.
 */
void rvgpu_device_fini(struct rvgpu_device *rdev)
{
    dev_info(rdev->dev, "rvgpu: finishing device.\n");

    rvgpu_device_unmap_mmio(rdev);
}

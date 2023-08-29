#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "rvgpu_module.h"


static struct pci_device_id rvgpu_id_tbl[] = {
    {PCI_DEVICE(RVGPU_HW_VENDOR_ID, RVGPU_HW_DEVICE_ID)},
    {},
};

MODULE_DEVICE_TABLE(pci, rvgpu_id_tbl);

static int rvgpu_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    printk("rvgpu_probe\n");
    return 0;
}

static void rvgpu_remove(struct pci_dev *pdev) {
    printk("rvgpu_remove\n");
}

static struct pci_driver rvgpu_pci_driver = {
    .name = "rvgpu",
    .id_table = rvgpu_id_tbl,
    .probe = rvgpu_probe,
    .remove = rvgpu_remove,
};

module_pci_driver(rvgpu_pci_driver);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Kernel module to drive the rvgpu device");
MODULE_AUTHOR("Qi Hangkong <qihangkong@outlook.com>");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "rvgpu_module.h"


static struct class *rvgpu_class;
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

static void rvgpu_module_exit(void) {
    pci_unregister_driver(&rvgpu_pci_driver);
    class_destroy(rvgpu_class);
    printk("rvgpu_module_exit finished successfully\n");
}

static int __init rvgpu_module_init(void) {
    int err;

    rvgpu_class = class_create(THIS_MODULE, "rvgpu");
    if (IS_ERR(rvgpu_class)) {
        pr_err("class_create error\n");
        err = PTR_ERR(rvgpu_class);
        return err;
    }

    err = pci_register_driver(&rvgpu_pci_driver);
    if (err) {
        pr_err("pci_register_driver error\n");
        goto err_pci;
    }

    printk("rvgpu_module_init finished successfully\n");
    return 0;

err_pci:
    class_destroy(rvgpu_class);
    pr_err("rvgpu_module_init finished err=%d\n", err);

    return err;
}

module_init(rvgpu_module_init);
module_exit(rvgpu_module_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Kernel module to drive the rvgpu device");
MODULE_AUTHOR("Qi Hangkong <qihangkong@outlook.com>");

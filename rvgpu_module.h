#ifndef _RVGPU_MODULE_H__
#define _RVGPU_MODULE_H__

#define RVGPU_HW_VENDOR_ID 0x1b36
#define RVGPU_HW_DEVICE_ID 0x1100
#define RVGPU_HW_REVISION  0x01

struct rvgpu_bar {
    u64 start;
    u64 end;
    u64 len;
    void __iomem *mmio;
};

struct rvgpu_dev {
    struct pci_dev *pdev;
    struct rvgpu_bar regs;
    struct rvgpu_bar vram;

    dev_t major;
    dev_t minor;
};

#endif // _RVGPU_MODULE_H__

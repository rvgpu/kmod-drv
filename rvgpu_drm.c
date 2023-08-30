#include <linux/sched.h>

#include <drm/drm_file.h>

#include "rvgpu.h"
/*
 * rvgpu_driver_open - drm callback for open
 * @dev: drm dev pointer
 * @file_priv: drm file
 * 
 * On device open, init device 
 * Returns 0 on success, error on failure.
 */
int rvgpu_driver_open(struct drm_device *dev, struct drm_file *fpriv)
{
    struct rvgpu_device *rdev = drm_to_rdev(dev);
    struct rvgpu_cli *cli;
    int ret = -ENOMEM;

    if (!(cli = kzalloc(sizeof(*cli), GFP_KERNEL))) {
        ret = -ENOMEM;
        goto done;
    }

    get_task_comm(cli->prog_name, current);
    cli->prog_pid = pid_nr(fpriv->pid);
    cli->rdev = rdev;
    
    printk("rvgpu_driver_open: %s[%d]\n", cli->prog_name, cli->prog_pid);
    fpriv->driver_priv = cli;

    return 0;
done:
    return ret;
}

/*
 * rvgpu_driver_postclose - drm callback for postclose
 * @dev: drm dev pointer
 * @file_priv: drm file
 * 
 * On device post close, tear down the device.
 */
void rvgpu_driver_postclose(struct drm_device *dev, struct drm_file *file_priv)
{
    // struct rvgpu_device *rdev = drm_to_rdev(dev);
    printk("rvgpu_driver_postclose\n");
}

void rvgpu_driver_release(struct drm_device *dev)
{
    printk("rvgpu_driver_release\n");
}

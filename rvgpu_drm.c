#include <linux/sched.h>

#include <drm/drm_file.h>
#include <drm/drm_drv.h>

#include "rvgpu.h"
#include "rvgpu_debug.h"
#include "rvgpu_drm.h"

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
    
    RVGPU_DEBUG_INFO("open driver: [%d] %s\n", cli->prog_pid, cli->prog_name);
    fpriv->driver_priv = cli;

    mutex_lock(&rdev->clients_lock);
    list_add(&cli->head, &rdev->clients);
    mutex_unlock(&rdev->clients_lock);

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
void rvgpu_driver_postclose(struct drm_device *dev, struct drm_file *fpriv)
{
    struct rvgpu_cli *cli = fpriv->driver_priv;
    int dev_index;

    if (!drm_dev_enter(dev, &dev_index)) {
        return ;
    }

    RVGPU_DEBUG_INFO("close driver: [%d] %s\n", cli->prog_pid, cli->prog_name);
    kfree(cli);

    drm_dev_exit(dev_index);

    // struct rvgpu_device *rdev = drm_to_rdev(dev);
}

void rvgpu_driver_release(struct drm_device *dev)
{
    RVGPU_FUNC_TODO();
}

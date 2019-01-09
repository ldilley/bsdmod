/* bsdmod_dev.c */

/*
 * An example FreeBSD kernel module to demonstrate I/O via a device
 */

/* The order of these headers is significant */
#include <sys/types.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/conf.h>
#include <sys/uio.h>
/*#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/sysent.h>
#include <sys/malloc.h>*/

#define MODULE_NAME "bsdmod_dev"
#define DEVICE_NAME "bsdmod"
#define MSG_SIZE 64

MODULE_VERSION(bsdmod_dev, 1);

/* Function prototypes for device file operations */
static d_open_t device_open;
static d_close_t device_close;
static d_read_t device_read;
static d_write_t device_write;

static struct cdev *bsdmod_dev;
static char msg[MSG_SIZE + 1];
static size_t len;

/* Structure of device file operations tied to functions */
static struct cdevsw bsdmod_cdevsw =
{
  .d_version = D_VERSION,
  .d_open = device_open,
  .d_close = device_close,
  .d_read = device_read,
  .d_write = device_write,
  .d_name = DEVICE_NAME,
};

static int bsdmod_dev_handler(struct module *m, int what, void *arg)
{
  int err = 0;
  struct make_dev_args dev_args;
  /*struct timeval tp;*/

  switch(what)
  {
    case MOD_LOAD:
      make_dev_args_init(&dev_args);
      dev_args.mda_devsw = &bsdmod_cdevsw;
      dev_args.mda_uid = UID_ROOT;
      dev_args.mda_gid = GID_WHEEL;
      dev_args.mda_mode = 0644;
      err = make_dev_s(&dev_args, &bsdmod_dev, DEVICE_NAME);
      if(err == 0)
        printf("%s->%s(): Device /dev/%s created.\n", MODULE_NAME, __func__, DEVICE_NAME);
      else
        printf("%s->%s(): Unable to create /dev/%s device.\n", MODULE_NAME, __func__, DEVICE_NAME);
      printf("%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
      break;
    case MOD_UNLOAD:
      destroy_dev(bsdmod_dev);
      printf("%s->%s(): Device /dev/%s removed.\n", MODULE_NAME, __func__, DEVICE_NAME);
      printf("%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
      break;
    default:
      err = EOPNOTSUPP;
      break;
  }
  return err;
}

static int device_open(struct cdev *dev, int flags, int dev_type, struct thread *td)
{
  printf("%s->%s(): /dev/%s opened.\n", MODULE_NAME, __func__, DEVICE_NAME);
  return 0;
}

static int device_close(struct cdev *dev, int flags, int dev_type, struct thread *td)
{
  printf("%s->%s(): /dev/%s closed.\n", MODULE_NAME, __func__, DEVICE_NAME);
  return 0;
}

static int device_read(struct cdev *dev, struct uio *uio, int flags)
{
  size_t bytes, res;
  int err;

  if(uio->uio_offset >= len + 1)
    res = 0;
  else
    res = len + 1 - uio->uio_offset;
  bytes = MIN(uio->uio_resid, res);
  printf("%s->%s(): uiomove() of %zu bytes completed.\n", MODULE_NAME, __func__, bytes);
  if((err = uiomove(msg, bytes, uio)) != 0)
    printf("%s->%s(): uiomove() of %zu bytes failed!\n", MODULE_NAME, __func__, bytes);
  return err;
}

static int device_write(struct cdev *dev, struct uio *uio, int flags)
{
  size_t bytes;
  int err;

  if(uio->uio_offset != 0 && (uio->uio_offset != len))
    return EINVAL;
  if(uio->uio_offset == 0)
    len = 0;
  bytes = MIN(uio->uio_resid, (MSG_SIZE - len));
  err = uiomove(msg + uio->uio_offset, bytes, uio);
  len = uio->uio_offset;
  msg[len] = '\0';
  if(err == 0)
    printf("%s->%s(): uiomove() of %zu bytes completed.\n", MODULE_NAME, __func__, bytes);
  else
    printf("%s->%s(): uiomove() of %zu bytes failed!\n", MODULE_NAME, __func__, bytes);
  return err;
}

DEV_MODULE(bsdmod_dev, bsdmod_dev_handler, NULL);

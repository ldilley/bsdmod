/* bsdmod_syscall.c */

/*
 * An example FreeBSD kernel module to demonstrate implementing a system call
 */

/* The order of these headers is significant */
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>

#define MODULE_NAME "bsdmod_syscall"

MODULE_VERSION(MODULE_NAME, 1);

static int offset = NO_SYSCALL;

static int testcall(struct thread *td, void *arg)
{
  printf("%s->%s called.\n", MODULE_NAME, __func__);
  return 0;
}

static struct sysent bsdmod_sysent =
{
  0,       /* sy_narg */
  testcall /* sy_call */
};

static int bsdmod_sysctl_handler(struct module *m, int what, void *arg)
{
  int err = 0;

  switch(what)
  {
    case MOD_LOAD:
      printf("%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
      break;
    case MOD_UNLOAD:
      printf("%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
      break;
    default:
      err = EOPNOTSUPP;
      break;
  }
  return err;
}

SYSCALL_MODULE(bsdmod_syscall, &offset, &bsdmod_sysent, bsdmod_sysctl_handler, NULL);

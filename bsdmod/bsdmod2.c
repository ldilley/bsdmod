/* bsdmod2.c */

/*
 * An example FreeBSD kernel module to demonstrate module dependency
 */

/* The order of these headers is significant */
#include <sys/types.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include "bsdmod.h"

#define MODULE_NAME "bsdmod2"

MODULE_VERSION(bsdmod2, 1);
MODULE_DEPEND(bsdmod2, bsdmod, 1, 1, 5);

static int bsdmod2_handler(struct module *m, int what, void *arg)
{
  int err = 0;

  switch(what)
  {
    case MOD_LOAD:
      printf("%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
      bsdmod_sum(2, 3);
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

static moduledata_t bsdmod2_mod =
{
  MODULE_NAME,
  bsdmod2_handler,
  NULL
};

DECLARE_MODULE(bsdmod2, bsdmod2_mod, SI_SUB_KLD, SI_ORDER_ANY);

/* bsdmod.c */

/*
 * An example FreeBSD kernel module
 */

/* The order of these headers is significant */
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/kernel.h>

#define MODULE_NAME "bsdmod"

static int bsdmod_handler(struct module *m, int what, void *arg)
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
  return(err);
}

static moduledata_t bsdmod_mod =
{
  MODULE_NAME,
  bsdmod_handler,
  NULL
};

DECLARE_MODULE(bsdmod, bsdmod_mod, SI_SUB_KLD, SI_ORDER_ANY);

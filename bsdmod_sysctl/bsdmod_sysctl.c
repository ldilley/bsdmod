/* bsdmod_sysctl.c */

/*
 * An example FreeBSD kernel module to demonstrate a sysctl key/value
 */

/* The order of these headers is significant */
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/module.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>

#define MODULE_NAME "bsdmod_sysctl"
#define SYSCTL_ROOT "bsdmod" /* root node */
#define SYSCTL_LEAF "test"   /* node under SYSCTL_ROOT */

MODULE_VERSION(MODULE_NAME, 1);

static int initial_data = 0;
static struct sysctl_ctx_list ctx_list;
static struct sysctl_oid *root_oid;

static int bsdmod_sysctl_handler(struct module *m, int what, void *arg)
{
  int err = 0;

  switch(what)
  {
    case MOD_LOAD:
      sysctl_ctx_init(&ctx_list);
      root_oid = SYSCTL_ADD_ROOT_NODE(&ctx_list, OID_AUTO, SYSCTL_ROOT, CTLFLAG_RW, 0, "root node");
      /* Use the following to add to an existing root such as "kern" or "debug" */
      /*root_oid = SYSCTL_ADD_NODE(&ctx_list, SYSCTL_STATIC_CHILDREN(_kern), OID_AUTO, MODULE_NAME, CTLFLAG_RW, 0, "root node");*/
      if(root_oid == NULL)
      {
        printf("%s->%s(): EINVAL encountered during SYSCTL_ADD_NODE()!\n", MODULE_NAME, __func__);
        return(EINVAL);
      }
      SYSCTL_ADD_INT(&ctx_list, SYSCTL_CHILDREN(root_oid), OID_AUTO, SYSCTL_LEAF, CTLFLAG_RW, &initial_data, 0, "test key accepting int data");
      printf("%s->%s(): sysctl key %s.%s added.\n", MODULE_NAME, __func__, SYSCTL_ROOT, SYSCTL_LEAF);
      printf("%s->%s(): Kernel module loaded.\n", MODULE_NAME, __func__);
      break;
    case MOD_UNLOAD:
      /*sysctl_remove_oid(root_oid, 1, 1);*/
      sysctl_ctx_free(&ctx_list);
      printf("%s->%s(): sysctl key %s.%s removed.\n", MODULE_NAME, __func__, SYSCTL_ROOT, SYSCTL_LEAF);
      printf("%s->%s(): Kernel module unloaded.\n", MODULE_NAME, __func__);
      break;
    default:
      err = EOPNOTSUPP;
      break;
  }
  return(err);
}

static moduledata_t bsdmod_sysctl_mod =
{
  MODULE_NAME,
  bsdmod_sysctl_handler,
  NULL
};

DECLARE_MODULE(bsdmod_sysctl, bsdmod_sysctl_mod, SI_SUB_KLD, SI_ORDER_ANY);

/* syscall_test.c */

/*
 * Tests a specific system call loaded as a module
 * See modinfo(2) and modstat(2) for more details.
 */

#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/module.h>

const char *module_path = "sys/bsdmod_syscall"; /* system call module path */
const char *syscall_name = "testcall()";        /* system call name */

int main()
{
  int mod_id;                  /* ID # of loaded module */
  int syscall_num;             /* system call # */
  struct module_stat mod_stat; /* contains module information */

  mod_stat.version = sizeof(mod_stat); /* version field gets set to sizeof result per module.h */
  if((mod_id = modfind(module_path)) == -1)
    err(1, "modfind(): Unable to find module %s", module_path); /* No such file or directory */
  if(modstat(mod_id, &mod_stat) != 0)
    err(1, "modstat(): Unable to stat module %s", module_path);

  /*
   * The data member is a modspecific_t union containing the syscall ID # in intval.
   * The ID # is determined and appended to the table of known syscalls after the module is loaded.
   */
  syscall_num = mod_stat.data.intval;
  printf("%s system call ID #: %d\n", module_path, syscall_num);
  __syscall(syscall_num);
  printf("System call performed. Check dmesg for output of %s->%s.\n", module_path, syscall_name);
  return 0;
}

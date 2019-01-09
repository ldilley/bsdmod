# BSDMod
This repo contains a variety of FreeBSD kernel modules that are for educational purposes.

See the equivalent Linux kernel module project: [LinMod](https://github.com/ldilley/linmod)

These modules demonstrate:

* How device I/O works
* How to work with sysctl keys and values
* How to load, unload, and display loaded modules
* How to view information on a module
* How to build FreeBSD kernel modules
* Module interdependence
* Logging to dmesg

### Instructions
1. Issue `make` at the top level to build everything or in an individual directory to only build that component.
2. To load a module: `kldload -v ./<module_name>.ko`
3. To view loaded modules: `kldstat -v`
4. To view module output after loading: `dmesg`
5. To unload a module: `kldunload -v <module_name>`
6. To view module output after unloading: `dmesg`

### Examples
##### Device
```
root@fbsdev:/usr/local/devel/bsdmod_dev # kldload -v ./bsdmod_dev.ko
Loaded ./bsdmod_dev.ko, id=5
root@fbsdev:/usr/local/devel/bsdmod_dev # ls -la /dev/bsdmod
crw-r--r--  1 root  wheel  0x65 Jan  9 01:50 /dev/bsdmod
root@fbsdev:/usr/local/devel/bsdmod_dev # echo "foo bar baz" > /dev/bsdmod
root@fbsdev:/usr/local/devel/bsdmod_dev # cat /dev/bsdmod
foo bar baz
root@fbsdev:/usr/local/devel/bsdmod_dev # echo "foo" >> /dev/bsdmod
root@fbsdev:/usr/local/devel/bsdmod_dev # cat /dev/bsdmod
foo
root@fbsdev:/usr/local/devel/bsdmod_dev # kldunload -v bsdmod_dev
Unloading bsdmod_dev.ko, id=5
root@fbsdev:/usr/local/devel/bsdmod_dev # cat /dev/bsdmod
cat: /dev/bsdmod: No such file or directory
root@fbsdev:/usr/local/devel/bsdmod_dev # dmesg | tail -n 18
bsdmod_dev->bsdmod_dev_handler(): Device /dev/bsdmod created.
bsdmod_dev->bsdmod_dev_handler(): Kernel module loaded.
bsdmod_dev->device_open(): /dev/bsdmod opened.
bsdmod_dev->device_write(): uiomove() of 12 bytes completed.
bsdmod_dev->device_close(): /dev/bsdmod closed.
bsdmod_dev->device_open(): /dev/bsdmod opened.
bsdmod_dev->device_read(): uiomove() of 13 bytes completed.
bsdmod_dev->device_read(): uiomove() of 0 bytes completed.
bsdmod_dev->device_close(): /dev/bsdmod closed.
bsdmod_dev->device_open(): /dev/bsdmod opened.
bsdmod_dev->device_write(): uiomove() of 4 bytes completed.
bsdmod_dev->device_close(): /dev/bsdmod closed.
bsdmod_dev->device_open(): /dev/bsdmod opened.
bsdmod_dev->device_read(): uiomove() of 5 bytes completed.
bsdmod_dev->device_read(): uiomove() of 0 bytes completed.
bsdmod_dev->device_close(): /dev/bsdmod closed.
bsdmod_dev->bsdmod_dev_handler(): Device /dev/bsdmod removed.
bsdmod_dev->bsdmod_dev_handler(): Kernel module unloaded.
```

##### Module Dependency
```
root@fbsdev:/usr/local/devel/bsdmod # kldload -v ./bsdmod2.ko
Loaded ./bsdmod2.ko, id=6
root@fbsdev:/usr/local/devel/bsdmod # dmesg | tail -n3
bsdmod->bsdmod_handler(): Kernel module loaded.
bsdmod2->bsdmod2_handler(): Kernel module loaded.
bsdmod->bsdmod_sum(2, 3): 2 + 3 = 5
root@fbsdev:/usr/local/devel/bsdmod # kldstat -v | sed -n '/bsdmod2.ko/,//p'
 6    1 0xffffffff8261c000 41e      bsdmod2.ko (./bsdmod2.ko)
        Contains modules:
                Id Name
                506 bsdmod2
                505 bsdmod
```

##### Syscall
```
root@fbsdev:/usr/local/devel/bsdmod_syscall # kldload -v ./bsdmod_syscall.ko
Loaded ./bsdmod_syscall.ko, id=6
root@fbsdev:/usr/local/devel/bsdmod_syscall # cd ../syscall_test/
root@fbsdev:/usr/local/devel/syscall_test # ./syscall_test
sys/bsdmod_syscall system call ID #: 210
System call performed. Check dmesg for output of sys/bsdmod_syscall->testcall().
root@fbsdev:/usr/local/devel/syscall_test # dmesg | tail -n2
bsdmod_syscall->bsdmod_syscall_handler(): Kernel module loaded.
bsdmod_syscall->testcall() called.
```

##### Sysctl
```
root@fbsdev:/usr/local/devel/bsdmod_sysctl # kldload -v ./bsdmod_sysctl.ko
Loaded ./bsdmod_sysctl.ko, id=6
root@fbsdev:/usr/local/devel/bsdmod_sysctl # sysctl -a | grep bsdmod
bsdmod.test: 0
root@fbsdev:/usr/local/devel/bsdmod_sysctl # sysctl bsdmod.test=1
bsdmod.test: 0 -> 1
root@fbsdev:/usr/local/devel/bsdmod_sysctl # sysctl -a | grep bsdmod
bsdmod.test: 1
root@fbsdev:/usr/local/devel/bsdmod_sysctl # kldunload -v bsdmod_sysctl.ko
Unloading bsdmod_sysctl.ko, id=6
root@fbsdev:/usr/local/devel/bsdmod_sysctl # sysctl -a | grep bsdmod
root@fbsdev:/usr/local/devel/bsdmod_sysctl #
```

#### Notes:
* The `-v` argument in the above instructions is optional and generates verbose output.
* `make clean` can be used at the top level to remove build data for everything or in an individual directory to only remove build data for that component.
* `make load` can be used to quickly build and load a particular component module while in its directory.
* `make unload` can be used to unload a particular component module while in its directory.
* The FreeBSD kernel source is required to build the modules contained in this project. It can be obtained via (replace with a closer mirror and your FreeBSD version): `svnlite checkout https://svn0.us-east.freebsd.org/base/releng/11.1 /usr/src`
* FreeBSD 11.1 was used for the development and testing of this project.

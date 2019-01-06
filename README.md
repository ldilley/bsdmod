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

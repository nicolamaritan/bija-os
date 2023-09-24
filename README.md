# BijaOS
Simple bootloader and kernel.
# Build
A GCC cross-compiler for ```i686-elf``` is needed (https://wiki.osdev.org/GCC_Cross-Compiler). The build script assumes the ```opt``` directory is placed in ```~/Development/opt```.
To build, run
```
$ source build.sh
```
# Debug
To debug, use ```gdb```. As the kernel is loaded at address 0x100000, add the following lines at the beginning of your ```.gdbinit```:
```
add-symbol-file build/kernelfull.o 0x100000
```
To use QEMU, add
```
target remote | qemu-system-i386 -S -gdb stdio -hda bin/os.bin
```

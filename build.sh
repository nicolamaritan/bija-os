#/bin/bash
export PREFIX="$HOME/Development/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir bin

mkdir build
mkdir build/idt
mkdir build/io
mkdir build/memory
mkdir build/pic
mkdir build/tty
mkdir build/memory/heap
mkdir build/memory/paging
mkdir build/disk
mkdir build/keyboard

make all

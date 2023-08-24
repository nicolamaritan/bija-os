#/bin/bash
export PREFIX="$HOME/Development/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir bin

mkdir build/idt
mkdir build/io
mkdir build/memory
mkdir build/pic
mkdir build/tty

make all

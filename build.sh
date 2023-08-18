#/bin/bash
export PREFIX="$HOME/Development/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
make all

FILES = build/kernel.asm.o build/kernel.o build/idt/idt.asm.o build/idt/idt.o build/memory/memory.o build/tty/tty.o build/io/io.asm.o build/pic/pic.o
INCLUDES = -I kernel
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: bin/boot.bin bin/kernel.bin
	rm -rf bin/os.bin
	dd if=bin/boot.bin >> bin/os.bin
	dd if=bin/kernel.bin >> bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> bin/os.bin

bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T kernel/linker.ld -o bin/kernel.bin -ffreestanding -O0 -nostdlib build/kernelfull.o

bin/boot.bin: kernel/boot/boot.asm
	nasm -f bin kernel/boot/boot.asm -o bin/boot.bin

build/kernel.asm.o: kernel/kernel.asm
	nasm -f elf -g kernel/kernel.asm -o build/kernel.asm.o

build/kernel.o: kernel/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./kernel/kernel.c -o ./build/kernel.o

build/idt/idt.asm.o: kernel/idt/idt.asm
	nasm -f elf -g kernel/idt/idt.asm -o build/idt/idt.asm.o

build/idt/idt.o: kernel/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I ./kernel/idt $(FLAGS) -std=gnu99 -c ./kernel/idt/idt.c -o ./build/idt/idt.o

build/memory/memory.o: kernel/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I kernel/memory $(FLAGS) -std=gnu99 -c ./kernel/memory/memory.c -o ./build/memory/memory.o

build/tty/tty.o: kernel/tty/tty.c
	i686-elf-gcc $(INCLUDES) -I kernel/tty $(FLAGS) -std=gnu99 -c ./kernel/tty/tty.c -o ./build/tty/tty.o

build/io/io.asm.o: kernel/io/io.asm
	nasm -f elf -g kernel/io/io.asm -o build/io/io.asm.o

build/pic/pic.o: kernel/pic/pic.c
	i686-elf-gcc $(INCLUDES) -I kernel/pic $(FLAGS) -std=gnu99 -c kernel/pic/pic.c -o build/pic/pic.o

clean:
	rm -rf bin/boot.bin
	rm -rf bin/kernel.bin
	rm -rf bin/os.bin
	rm -rf $(FILES)
	rm -rf build/kernelfull.o
SOURCES= boot.o kernel.o vga.o descriptorTables.o isr.o dt.o interrupt.o string.o kb.o
CFLAGS= -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS= -m elf_i386 -T ../linker.ld
ASFLAGS=-felf32

all: $(SOURCES) link clean run

clean:
	-rm *.o

link:
	../i686-elf-tools/bin/i686-elf-ld $(LDFLAGS) -o R0KRNL.0xe $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $< 

.c.o:
	../i686-elf-tools/bin/i686-elf-gcc -c $(CFLAGS) $< -o $@

run:
	qemu-system-i386 -kernel R0KRNL.0xe
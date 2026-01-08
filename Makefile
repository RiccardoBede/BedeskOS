ISO_IMG = bkernel.iso

# File output
KERNEL_ELF = bkernel.bin
KERNEL_OBJ = bkernel.o
MULTIBOOT_OBJ = multiboot_header.o

GCC = gcc
GCC_FLAGS = -ffreestanding -m32 -c -Wno-implicit-function-declaration -fno-stack-protector -nostdlib

NASM = nasm
NASM_FLAGS = -f elf32

LD = ld
LD_SCRIPT = linker.ld

GRUBMKRESCUE = grub-mkrescue
GRUBMKRESCUE_FLAGS = -o

all: $(ISO_IMG)

# Compilazione del kernel
$(KERNEL_OBJ): bkernel.c
	$(GCC) $(GCC_FLAGS) $< -o $@

# Compilazione dell'header Multiboot
$(MULTIBOOT_OBJ): multiboot_header.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

$(KERNEL_ELF): $(MULTIBOOT_OBJ) $(KERNEL_OBJ)
	$(LD) -m elf_i386 -T $(LD_SCRIPT) -Ttext 0x1000 -o $@ $^

$(ISO_IMG): $(KERNEL_ELF)
	mkdir -p iso/boot/grub
	cp $(KERNEL_ELF) iso/boot/bkernel.bin
	echo 'set timeout=1' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "BKernel" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/bkernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	$(GRUBMKRESCUE) $(GRUBMKRESCUE_FLAGS) $(ISO_IMG) iso

clean:
	rm -f $(KERNEL_ELF) $(KERNEL_OBJ) $(MULTIBOOT_OBJ) $(ISO_IMG)
	rm -rf iso

.PHONY: all clean


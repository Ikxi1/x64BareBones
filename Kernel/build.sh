#!/bin/sh
set -e
set -x

cd src

ELF_OUT="../../Image/kernel.elf"
BIN_OUT="../../Image/kernel.bin"

asm_files=$(find . -type f -name '*.asm')
c_files=$(find . -type f -name '*.c')

# Assemble ASM
for src in $asm_files; do
    obj="${src%.asm}.o"
    echo "Assembling $src -> $obj"
    nasm -g -F dwarf -f elf64 "$src" -o "$obj" -I asm/
done

# Compile C
for src in $c_files; do
    obj="${src%.c}.o"
    echo "Compiling $src -> $obj"
    x86_64-linux-gnu-gcc -m64 \
      -fno-exceptions -fno-asynchronous-unwind-tables \
      -mno-mmx -mno-sse -mno-sse2 \
      -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc \
      -mno-red-zone \
      -Wall -ffreestanding -nostdlib \
      -fno-common -std=c99 \
      -g -O0 \
      -I../include \
      -c "$src" -o "$obj"
done

obj_files=$(find . -type f -name '*.o')
loader_obj="../loader.o"

# Assemble loader
echo "Assembling loader.asm -> $loader_obj"
nasm -g -F dwarf -f elf64 ../loader.asm -o "$loader_obj"

# Link ELF (for GDB)
echo "Linking -> $ELF_OUT"
x86_64-linux-gnu-ld \
  --warn-common \
  -z max-page-size=0x1000 \
  -T ../kernel.ld \
  -o "$ELF_OUT" \
  "$loader_obj" $obj_files

# Create raw binary (for bootloader)
echo "Extracting raw binary -> $BIN_OUT"
objcopy -O binary "$ELF_OUT" "$BIN_OUT"

cd ..
echo "Build complete: $ELF_OUT and $BIN_OUT"
#!/bin/sh
set -e
set -x

cd src

OUT="../kernel.bin"

asm_files=$(find . -type f -name '*.asm')
c_files=$(find . -type f -name '*.c')

for src in $asm_files; do
    obj="${src%.asm}.o"
    echo "Assembling $src -> $obj"
    nasm -g -F dwarf -f elf64 "$src" -o "$obj" -I asm/
done

for src in $c_files; do
    obj="${src%.c}.o"
    echo "Compiling $src -> $obj"
    x86_64-linux-gnu-gcc -m64 -fno-exceptions -fno-asynchronous-unwind-tables \
      -mno-mmx -mno-sse -mno-sse2 -fno-builtin-malloc -fno-builtin-free \
      -fno-builtin-realloc -mno-red-zone -Wall -ffreestanding -nostdlib \
      -fno-common -std=c99 -g -I../include -c "$src" -o "$obj" \
      -F dwarf
done

obj_files=$(find . -type f -name '*.o')
loader_obj="../loader.o"

echo "Assembling loader.asm -> $loader_obj"
nasm -g -F dwarf -f elf64 ../loader.asm -o "$loader_obj"

echo "Linking -> $OUT"
x86_64-linux-gnu-ld --warn-common -z max-page-size=0x1000 -T ../kernel.ld \
  -o "$OUT" "$loader_obj" $obj_files

cd ..
echo "Build complete: $OUT"

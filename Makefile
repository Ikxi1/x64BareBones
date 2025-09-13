
all:  bootloader kernel image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; ./build.sh

image: kernel bootloader
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; ./clean.sh

.PHONY: bootloader image collections kernel all clean

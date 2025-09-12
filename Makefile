
all:  bootloader kernel image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

image: kernel bootloader
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean

.PHONY: bootloader image collections kernel all clean

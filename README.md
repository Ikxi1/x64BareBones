This is basic OS that currently can't do much.
It's based on a prior repo and uses the Pure64 bootloader (modified).

Environment setup:
1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc x86_64-linux-gnu-gcc make

2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

  user@linux:$ cd Toolchain
  user@linux:$ make all

3- Build the Kernel

From the x64BareBones project directory run:

  user@linux:$ make all

4- Run the kernel

From the x64BareBones project directory run:

  user@linux:$ ./run.sh

Author: Ikxi

Original Author: Rodrigo Rearden (RowDaBoat)

Original Collaborator: Augusto Nizzo McIntosh

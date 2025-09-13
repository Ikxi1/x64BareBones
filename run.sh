#!/bin/bash
qemu-system-x86_64 -smp sockets=1,cores=10,threads=1 \
 -hda Image/x64BareBonesImage.qcow2 -m 512 \
 -S -s -d int,cpu_reset,guest_errors

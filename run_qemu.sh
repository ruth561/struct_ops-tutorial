#!/bin/bash

ROOTFS="rootfs.img"

if [ ! -e "$ROOTFS" ]; then
	echo "Error: $ROOTFS not found." >&2
	echo "First, refer to the 'How to debug with QEMU' section in README.md to create $ROOTFS." >&2
	exit 1
fi

sudo mount rootfs.img mnt

sudo cp my_ops.ko mnt/root/
sudo cp bpf/target/debug/bpf mnt/root/

sudo umount mnt

qemu-system-x86_64 -m 4G -smp 4 -nographic \
	-kernel /boot/vmlinuz-$(uname -r) \
	-drive file=rootfs.img,format=raw,if=none,id=drive0 \
	-device virtio-blk-pci,drive=drive0 \
	-append "console=ttyS0 root=/dev/vda rw nokaslr" \
	-s

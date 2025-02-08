# Prerequisites

The kernel version v6.12-rc1 or higher is required.
If you attempt to build this project with Linux Kernel v6.11 or earlier, the build will fail because certain kernel functions related to BPF (e.g., `bpf_base_func_proto`) are not exported to loadable kernel modules (LKMs).

# How to build

```
$ make
```

# How to use

You can insert the kernel module `my_ops` by running the following command:

```
$ make insmod
```

You can remove the inserted `my_ops` module by running the following command:

```
$ make rmmod
```

Once you insert the `my_ops` module, you can attach a custom BPF program to the `gops` defined in it. For more details, refer to [bpf/README.md](bpf/README.md).

Or simply execute `run.sh` script:
```
$ ./run.sh
```

# How to run in QEMU

First, create a root filesystem image file using the following commands:
```
$ truncate -s 8G rootfs.img
$ mkfs.ext4 rootfs.img
$ mkdir mnt
$ sudo mount rootfs.img mnt
$ sudo debootstrap noble mnt
$ sudo umount mnt
```

To enable login, you need to set a password:
```
$ sudo mount rootfs.img mnt
$ sudo chroot mnt
# passwd
New password: 
Retype new password: 
passwd: password updated successfully
# exit
exit
$ sudo umount mnt
```

Then, you can boot the kernel in QEMU using the `run_qemu.sh` script:
```
$ ./run_qemu.sh
```

In QEMU, you will find `my_ops.ko` and `bpf` in the /root directory. You can insert the kernel module and load the eBPF program.

You can also debug using GDB and QEMU. Run the following commands:

```
$ gdb -q
(gdb) target remote :1234
```

# Q&A

If you encounter the following warning message during the build:

```
Skipping BTF generation for /home/ruth/Documents/lkms/struct_ops-tutorial/my_ops.ko due to unavailability of vmlinux
```

you can address the issue by running the following commands:

```
$ sudo apt install dwarves
$ sudo cp /sys/kernel/btf/vmlinux /usr/lib/modules/$(uname -r)/build/
```

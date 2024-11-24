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

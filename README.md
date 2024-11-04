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

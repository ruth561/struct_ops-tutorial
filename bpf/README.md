# BPF Program Attached to `struct my_ops`

This directory contains an example BPF program designed to attach to `struct my_ops`.

# Prerequisites

Before building, you must install `libbpf-cargo`:

```
$ cargo install libbpf-cargo
```

Additionally, you must load *my_ops.ko* beforehand, as we need to retrieve the data structure used in *my_ops.ko* from `/sys/kernel/btf/my_ops`. Switch to the parent directory and run the following command:

```
$ cd ..
$ make insmod
```

# How to build

To build the program, run:

```
$ make
```

# How to use

After building this crate, the binary will be available at *target/debug/bpf*. To load the example BPF program, use the following commands:

```
$ sudo target/debug/bpf
```

or

```
$ make run
```

# Demo

```
$ make
sudo bpftool btf dump file /sys/kernel/btf/my_ops format c > src/bpf/my_ops.h
bear -- cargo libbpf build
cargo libbpf gen
Warning: unrecognized map: license
cargo build
   Compiling bitflags v2.6.0
   Compiling libc v0.2.161
   Compiling libbpf-sys v1.5.0+v1.5.0
   Compiling vsprintf v2.0.0
   Compiling libbpf-rs v0.24.6
   Compiling bpf v0.1.0 (/home/ruth/Documents/lkms/struct_ops-tutorial/bpf)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 1.39s
$ make run &
[1] 119135
sudo target/debug/bpf
Successfully attached bpf program!
$ sudo cat /sys/kernel/my_ops/ctl
ctl_show: val=1729
```

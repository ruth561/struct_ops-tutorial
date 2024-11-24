#!/bin/bash

# This shell script performs the following tasks:
#   1. Builds and inserts the kernel module `my_ops`
#   2. Builds and loads the BPF program located in the `bpf/` directory.
#   3. Runs the program until a Ctrl+C signal is received.
#   4. Removes the BPF program and the kernel module.

make insmod || exit -1
cd bpf	# at bpf/
make run
cd ..
make rmmod

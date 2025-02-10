TARGET		:= target/debug/bpf
HEADER		:= src/bpf/my_ops.h
HEADER_SRC	:= /sys/kernel/btf/my_ops
SKELTON		:= src/bpf/example.skel.rs
BPF_SRC		:= src/bpf/example.bpf.c
BPF_SRC		+= src/bpf/my_ops_kfuncs.bpf.h
APP_SRC		:= src/main.rs

.PHONY: all
all: $(TARGET)

.PHONY: run
run: $(TARGET)
	sudo dmesg -C
	sudo $(TARGET)
	sudo dmesg

$(TARGET): $(SKELTON) $(APP_SRC)
	cargo build

$(HEADER): $(HEADER_SRC)
	sudo bpftool btf dump file /sys/kernel/btf/my_ops format c > $@

$(SKELTON): $(BPF_SRC) $(HEADER)
	bear -- cargo libbpf build
	cargo libbpf gen

.PHONY: clean
clean:
	rm -f src/bpf/example.skel.rs src/bpf/mod.rs src/bpf/my_ops.h
	rm -f compile_commands.json
	cargo clean

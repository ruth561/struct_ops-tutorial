// SPDX-License-Identifier: GPL-2.0-only
#include "my_ops.h"
#include <bpf/bpf_tracing.h>
char LICENSE[] SEC("license") = "GPL";

SEC("struct_ops/my_ops_calculate")
int BPF_PROG(my_ops_calculate, int n)
{
	return 1729;
}

SEC(".struct_ops.link")
struct my_ops my_ops_sample = {
	.calculate = (void *) my_ops_calculate,
};

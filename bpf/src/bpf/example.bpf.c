// SPDX-License-Identifier: GPL-2.0-only
#include "my_ops.h"
#include <bpf/bpf_tracing.h>
char LICENSE[] SEC("license") = "GPL";

SEC("struct_ops/my_ops_calculate")
u64 BPF_PROG(my_ops_calculate, u64 n)
{
	u64 retval;

	retval = n * n;
	bpf_printk("calculate(%d) -> %d", n, retval);
	return retval;
}

SEC(".struct_ops.link")
struct my_ops my_ops_sample = {
	.calculate = (void *) my_ops_calculate,
};

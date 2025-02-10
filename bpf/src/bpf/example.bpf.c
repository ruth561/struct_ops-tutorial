// SPDX-License-Identifier: GPL-2.0-only
#include "my_ops.h"
#include "my_ops_kfuncs.bpf.h"
#include <bpf/bpf_tracing.h>
char LICENSE[] SEC("license") = "GPL";

u64 cnt = 0;

SEC("struct_ops/my_ops_calculate")
u64 BPF_PROG(my_ops_calculate, u64 n)
{
	u64 retval;
	s32 i;
	struct bpf_graph *graph;

	retval = n * n;

	cnt += 1;
	retval += cnt;
	
	bpf_printk("calculate(%d) -> %d", n, retval);

	my_ops_log("THIS IS TEST!\n");

	graph = bpf_graph_alloc(10);

	if (!graph) {
		bpf_printk("Failed to allocate bpf_graph.");
		return -1;
	}

	bpf_printk("graph->n = %d", graph->n);
	bpf_printk("graph->m = %d", graph->m);

	bpf_graph_free(graph);

	return retval;
}

SEC(".struct_ops.link")
struct my_ops my_ops_sample = {
	.calculate = (void *) my_ops_calculate,
};

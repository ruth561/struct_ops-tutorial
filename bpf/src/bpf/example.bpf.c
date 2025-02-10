// SPDX-License-Identifier: GPL-2.0-only
#include "my_ops.h"
#include "my_ops_kfuncs.bpf.h"
#include <bpf/bpf_tracing.h>
char LICENSE[] SEC("license") = "GPL";

u64 cnt = 0;

#define assert(cond)						\
	do {							\
		if (!(cond)) {					\
			bpf_printk("%s:%d assertion failed",	\
				__FILE__, __LINE__);		\
		}						\
	} while (0)

#define assert_ret(cond)					\
	do {							\
		if (!(cond)) {					\
			bpf_printk("%s:%d assertion failed",	\
				__FILE__, __LINE__);		\
			return;					\
		}						\
	} while (0)

#define assert_ret_err(cond)					\
	do {							\
		if (!(cond)) {					\
			bpf_printk("%s:%d assertion failed",	\
				__FILE__, __LINE__);		\
			return -1;				\
		}						\
	} while (0)

SEC("struct_ops/my_ops_calculate")
u64 BPF_PROG(my_ops_calculate, u64 n)
{
	u64 retval;
	s32 ret;
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

	/*
	 * Example of a graph used in the paper:
	 *   "Intra-Task Priority Assignment in Real-Time Scheduling of DAG Tasks on Multi-Cores."
	 */
	ret = bpf_graph_add_edge(graph, 0, 1);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 0, 2);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 0, 3);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 1, 4);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 2, 4);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 3, 6);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 4, 5);
	assert(!ret);
	ret = bpf_graph_add_edge(graph, 5, 6);
	assert(!ret);

	/*
	 * @from is not a valid node.
	 */
	ret = bpf_graph_add_edge(graph, 10, 0);
	assert(ret < 0);

	/*
	 * @to is not a valid node.
	 */
	ret = bpf_graph_add_edge(graph, 0, 10);
	assert(ret < 0);

	/*
	 * @from and @to are not valid nodes.
	 */
	ret = bpf_graph_add_edge(graph, 20, 20);
	assert(ret < 0);

	/*
	 * The edge (5, 6) already exists.
	 */
	ret = bpf_graph_add_edge(graph, 5, 6);
	assert(ret == 1);
 
	bpf_printk("graph->n = %d", graph->n);
	bpf_printk("graph->m = %d", graph->m);

	bpf_graph_dump(graph);

	bpf_graph_free(graph);

	return retval;
}

SEC(".struct_ops.link")
struct my_ops my_ops_sample = {
	.calculate = (void *) my_ops_calculate,
};

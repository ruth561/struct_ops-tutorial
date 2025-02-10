#ifndef __BPF_GRAPH_H
#define __BPF_GRAPH_H

#include <linux/kernel.h>
#include <linux/bug.h>

#define FIXED_VECTOR_CAPACITY	100
#define BPF_GRAPH_MAX_NODES	FIXED_VECTOR_CAPACITY

struct fixed_vector_u32 {
	u32	len;
	u32	cap;
	u32	buf[FIXED_VECTOR_CAPACITY];
};

struct bpf_graph {
	u32			n;
	u32			m;
	struct fixed_vector_u32	edges[BPF_GRAPH_MAX_NODES];
};

#endif

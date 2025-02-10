#ifndef __MY_OPS_KFUNCS_H
#define __MY_OPS_KFUNCS_H


#include "my_ops.h"

#ifndef __ksym
#define __ksym __attribute__((section(".ksyms")))
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif

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

extern void my_ops_log(const char *) __weak __ksym;
extern struct bpf_graph *bpf_graph_alloc(u32 nr_nodes) __weak __ksym;
extern void bpf_graph_free(struct bpf_graph *graph) __weak __ksym;
extern s32 bpf_graph_add_edge(struct bpf_graph * graph, u32 from, u32 to) __weak __ksym;
extern void bpf_graph_dump(struct bpf_graph * graph) __weak __ksym;

#endif /* __MY_OPS_KFUNCS_H */

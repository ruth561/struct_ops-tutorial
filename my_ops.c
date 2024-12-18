// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bpf.h>

MODULE_AUTHOR("Takumi Jin");
MODULE_DESCRIPTION("A simple implementation of a kernel module using struct_ops");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("0.1");

// =============================================================================
// These implementations handle sysfs interactions for my_ops.
// =============================================================================

/**
 * struct my_ops - Operation table for BPF-based arithmetic operations in sysfs:my_ops
 *
 * Users can define custom arithmetic operations to be used during read access
 * to sysfs:my_ops/ctl. These operations are loaded into this table using struct_ops
 * and utilized when a read access occurs on the ctl file.
 */
struct my_ops {
	/**
	 * calculate - Performs an arithmetic operation during read access to
	 * sysfs:my_ops/ctl file.
	 * @n: An integer passed from a previous write to sysfs:my_ops/ctl
	 *
	 * When a user reads from sysfs:my_ops/ctl after writing the integer x,
	 * the result will be the value of ops.calculate(x).
	 */ 
	int (*calculate)(int n);
};

/*
 * gops - Global ops variable used by ctl_show
 *
 * By setting the loaded (and jitted) BPF program here, the BPF program can finally
 * be used. Note that the struct_ops subsystem does not automatically set the BPF
 * program in this variable; instead, it is manually replaced within the st_ops->reg
 * function. See bpf_my_ops_reg.
 *
 * TODO: use rcu for mutual exclusion
 */
static struct my_ops gops;

/*
 * The argument of my_ops's callback is an integer, so we don't have to invoke
 * btf_ctx_access or bpf_tracing_btf_ctx_access. If the access is within the range 
 * of the ctx region, it is considered a valid access.
 *
 * See bpf_verifier_ops->is_valid_access
 */
static bool my_ops_is_valid_access(int off, int size,
				   enum bpf_access_type type,
				   const struct bpf_prog *prog,
				   struct bpf_insn_access_aux *info)
{
	if (off == 0 && size == sizeof(u64))
		return true;
	return false;
}

/*
 * bpf_base_func_proto allows BPF programs to use certain basic BPF helper functions,
 * such as bpf_printk. However, it does not support all BPF helper functions.
 * If you need to use additional BPF helper functions, such as bpf_event_output_data,
 * you must implement the necesary enabling process.
 *
 * For more details, refer to the implementation of bpf_base_func_proto.
 *
 * NOTE: bpf_base_func_proto is not exported to LKMs until v6.12-rc1.
 * Therefore, it cannot be used with kernel versions earlier than v6.12.
 */
static const struct bpf_func_proto *my_ops_get_func_proto(enum bpf_func_id func_id,
							  const struct bpf_prog *prog)
{
	return bpf_base_func_proto(func_id, prog);
}

/*
 * We only implement is_valid_access, so BPF programs attached to my_ops cannot
 * invoke any BPF helper functions. If you want to enable specific BPF helpers,
 * you should implements .get_func_proto.
 */
static struct bpf_verifier_ops my_ops_bpf_verifier_ops = {
	.get_func_proto  = my_ops_get_func_proto,
	.is_valid_access = my_ops_is_valid_access,
};

/**
 * bpf_my_ops_init - Function assigned to st_ops->init
 * @btf: BTF data structure that struct my_ops is located
 *
 * If this implementation is directly included in the kernel image, then this
 * will be btf_vmlinux. Otherwise, if this implementation is built as a module,
 * it will be the BTF structure for the module.
 *
 * This function is invoked by register_bpf_struct_ops.
 */
static int bpf_my_ops_init(struct btf *btf)
{
	pr_info("st_ops->init()\n");

	return 0;
}

/**
 * bpf_my_ops_init_member - Function assigned to st_ops->init_member
 *
 * This function is invoked when members of struct my_ops are updated or linked.
 * for example, during bpf(BPF_LINK_CREATE, ...) or bpf(BPF_MAP_UPDATE_ELEM, ...).
 */
static int bpf_my_ops_init_member(const struct btf_type *t,
				  const struct btf_member *member,
				  void *kdata, const void *udata)
{
	pr_info("st_ops->init_member()\n");

	return 0;
}

/**
 * bpf_my_ops_check_member - Function assigned to st_ops->check_member
 *
 * This function checks @prog, which is about to be attached to @member.
 * We only check for sleepability here.
 */
static int bpf_my_ops_check_member(const struct btf_type *t,
				   const struct btf_member *member,
				   const struct bpf_prog *prog)
{
	pr_info("st_ops->check_member()\n");

	/*
	 * All callbacks aren't allowed sleep.
	 */
	if (prog->sleepable)
		return -EINVAL;
	return 0;
}

/**
 * bpf_my_ops_reg - Function assigned to st_ops->reg
 * @kdata: Pointer to struct my_ops
 * @link: TODO:
 *
 * @kdata points to a custom my_ops structure provided by the user. It is a table
 * of JIT-compiled BPF program. We copy the function pointers to gops here, which
 * will be used later during read access to sysfs:my_ops/ctl. This functions is
 * called in the context of BPF_MAP_UPDATE_ELEM or BPF_LINK_CREATE.
 */
static int bpf_my_ops_reg(void *kdata, struct bpf_link *link)
{
	pr_info("st_ops->reg()\n");

	gops = *(struct my_ops *) kdata;
	return 0;
}

/**
 * bpf_my_ops_unreg - Function assigned to st_ops->unreg
 *
 * TODO:
 */
static void bpf_my_ops_unreg(void *kdata, struct bpf_link *link)
{
	pr_info("st_ops->unreg()\n");
}

/*
 * This function serves as a CFI (Control Flow Integrity) stub and is not called.
 */
static int calculate_stub(int n)
{
	return n;
}

/*
 * See st_ops->cfi_stubs
 */
static struct my_ops my_ops_stubs = {
	.calculate = calculate_stub,
};

static struct bpf_struct_ops bpf_my_ops = {
	.verifier_ops	= &my_ops_bpf_verifier_ops,
	.init		= bpf_my_ops_init,
	.init_member	= bpf_my_ops_init_member,
	.check_member	= bpf_my_ops_check_member,
	.reg		= bpf_my_ops_reg,
	.unreg		= bpf_my_ops_unreg,
	.name		= "my_ops",
	.owner		= THIS_MODULE,
	.cfi_stubs	= &my_ops_stubs,
};

// =============================================================================
// The following implementations are for files in sysfs:my_ops.
// =============================================================================
static int n = 0;

static ssize_t ctl_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	int val;

	if (gops.calculate) {
		val = gops.calculate(n);
	} else {
		pr_info("ops.calculate is NULL...\n");
		val = n;
	}

	return sprintf(buf, "ctl_show: val=%d\n", val);
}

static ssize_t ctl_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;

	ret = kstrtoint(buf, 10, &n);
	if (ret < 0) {
		pr_err("failed to convert buf to int\n");
		return ret;
	}

	pr_info("ctl_store: n=%d\n", n);
	return count;
}

// sysfs:my_ops dir
static struct kobject *my_ops_kobj;
// sysfs:my_ops/ctl file
static struct kobj_attribute ctl_attr = __ATTR(ctl, 0660, ctl_show, ctl_store);

static int __init my_ops_init(void)
{
	int err;

	pr_info("my_ops_init\n");

	memset(&gops, 0, sizeof(struct my_ops));

	my_ops_kobj = kobject_create_and_add("my_ops", kernel_kobj);
	if (!my_ops_kobj) {
		pr_err("failed to create and add my_ops kobject\n");
		return -ENOMEM;
	}

	err = sysfs_create_file(my_ops_kobj, &ctl_attr.attr);
	if (err) {
		pr_err("failed to create file sysfs:my_ops/ctl\n");
		return err;
	}

	err = register_bpf_struct_ops(&bpf_my_ops, my_ops);
	if (err) {
		pr_err("failed to register struct_ops my_ops\n");
		return err;
	}

	return 0;
}

static void __exit my_ops_exit(void)
{
	pr_info("my_ops_exit\n");

	kobject_put(my_ops_kobj);
}

module_init(my_ops_init);
module_exit(my_ops_exit);

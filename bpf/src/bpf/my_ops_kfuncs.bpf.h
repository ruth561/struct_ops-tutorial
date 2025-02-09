#ifndef __MY_OPS_KFUNCS_H
#define __MY_OPS_KFUNCS_H


#ifndef __ksym
#define __ksym __attribute__((section(".ksyms")))
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif

extern void my_ops_log(const char *) __weak __ksym;

#endif /* __MY_OPS_KFUNCS_H */

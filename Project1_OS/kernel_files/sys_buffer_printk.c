#include <linux/kernel.h>
asmlinkage long sys_buffer_printk(int *p, long long *s1, long long *s2, long long *e1, long long *e2){
	long long ns = 1E9;
    printk("[Project1] %d %lld.%lld %lld.%lld\n", *p, *s1, *s2, *e1, *e2);
    return 0;
}
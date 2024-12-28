#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kprobes.h>

// This will be our kprobe struct
static struct kprobe kp = {
    .symbol_name  = "__sys_connect"
};

int kpb_pre(struct kprobe *p, struct pt_regs *regs)
{
    printk("lomond - socket connected, RDI=%lu, RSI=%lu, RDX=%lu\n", regs->di, regs->si, regs->dx);
    struct sockaddr* sa = (struct sockaddr*)regs->si;
    unsigned int fam = sa->sa_family;
    printk("sa_family is %u\n", fam);
    /*if (sa->sa_family == 2)
    {
        printk("sa_data is %s\n", sa->sa_data);
    } */
    
    return 0;
}
void kpb_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    printk("lomond - socket connect finished\n");
}
int minit(void)
{
    printk("lomond - Module inserted\n");
    kp.pre_handler = kpb_pre;
    kp.post_handler = kpb_post;
    register_kprobe(&kp); //When a kprobe is registered, Kprobes makes a copy of the probed instruction and replaces the first byte(s) of the probed instruction with a breakpoint instruction (e.g., int3 on i386 and x86_64). ~ Kernel docs
    return 0;
}
void mexit(void)
{
    unregister_kprobe(&kp);
    printk("lomond - Module removed\n");
}
module_init(minit);
module_exit(mexit);
MODULE_AUTHOR("Assaf R.");
MODULE_DESCRIPTION("Socket prob");
MODULE_LICENSE("GPL");
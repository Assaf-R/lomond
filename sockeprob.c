#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kprobes.h>

static struct kprobe kp = {
    .symbol_name  = "__sys_connect"
};

int kpb_pre(struct kprobe *p, struct pt_regs *regs)
{
    printk("socket connected, RDI=%lu, RSI=%lu, RDX=%lu\n", regs->di, regs->si, regs->dx);
    struct sockaddr* sa = (struct sockaddr*)regs->si;
    printk("sa_family is %u\n", sa->sa_family);
    /*if (sa->sa_family == 2)
    {
        printk("sa_data is %s\n", sa->sa_data);
    } */
    
    return 0;
}
void kpb_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    printk("socket connect finished\n");
}
int minit(void)
{
    printk("Module inserted\n");
    kp.pre_handler = kpb_pre;
    kp.post_handler = kpb_post;
    //kp.addr = (kprobe_opcode_t *)0xffffffffb0801f60;
    register_kprobe(&kp);
    return 0;
}
void mexit(void)
{
    unregister_kprobe(&kp);
    printk("Module removed\n");
}
module_init(minit);
module_exit(mexit);
MODULE_AUTHOR("Assaf R.");
MODULE_DESCRIPTION("Socket prob");
MODULE_LICENSE("GPL");
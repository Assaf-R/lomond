#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/kprobes.h>
#include<linux/in.h>
#include<linux/socket.h>
#include <linux/uaccess.h> // For copy_from_user
#include <linux/inet.h>    // For %pI4 format

int kpb_pre(struct kprobe *p, struct pt_regs *regs)
{
    //connect(5, {sa_family=AF_INET, sin_port=htons(1025), sin_addr=inet_addr("8.8.8.8")}, 16) = 0
    printk("lomond - socket connected, RDI=%lu, RSI=%lu, RDX=%lu\n", regs->di, regs->si, regs->dx);

    struct sockaddr sa;

    if (copy_from_user(&sa, (struct sockaddr __user *)regs->si, sizeof(struct sockaddr))) {
        printk("lomond - Error acurred\n");
        return 0;
    }

    unsigned long fam = sa.sa_family;

    if(fam == AF_INET)
    {
        struct sockaddr_in sai;
        if (copy_from_user(&sai, (struct sockaddr_in __user *)regs->si, sizeof(struct sockaddr_in))) 
        {
            printk("lomond - Error acurred\n");
            return 0;
        }
        int port = sai.sin_port;
        struct in_addr dst_addr_full = sai.sin_addr;

        printk("lomond - port %d, addr - %pI4\n", port, &sai.sin_addr.s_addr);

    }

    return 0;
}


void kpb_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    printk("lomond - socket connect finished\n");
}

static struct kprobe kp = {
    .symbol_name  = "__sys_connect",
    .pre_handler = *kpb_pre,
    .post_handler = *kpb_post,
};

int minit(void)
{
    printk("lomond - Module inserted\n");
    register_kprobe(&kp); //When a kprobe is registered, Kprobes makes a copy of the probed instruction and replaces the first byte(s) of the probed instruction with a breakpoint instruction (e.g., int3 on i386 and x86_64). ~ Kernel docs
    return 0;
}
void mexit(void)
{
    unregister_kprobe(&kp);
    printk("lomond - Module removed\n----- lomond ----\n");
}
module_init(minit);
module_exit(mexit);
MODULE_AUTHOR("Assaf R.");
MODULE_DESCRIPTION("Socket prob");
MODULE_LICENSE("GPL");
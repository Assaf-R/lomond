#include<linux/uaccess.h>
#include<linux/version.h>
#include<linux/kprobes.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/socket.h>
#include<linux/init.h>
#include<linux/inet.h>
#include<linux/in.h>

int kpb_pre(struct kprobe *p, struct pt_regs *regs)
{
    // This function will execute every time a socket connection is made, So first I'd like to filter out unix socket connection and focus on IPv4
    // sockets. By strace-ing a simple ping I'll get this -

    //connect(5, {sa_family=AF_INET, sin_port=htons(1025), sin_addr=inet_addr("8.8.8.8")}, 16) = 0
    
    // If you'd like to see for yourself uncomment the next line
    // printk("lomond - socket connected, Registers - RDI=%lu, RSI=%lu, RDX=%lu\n", regs->di, regs->si, regs->dx);

    // So I'd like to cast the data in the RSI register to a sockaddr struct
    struct sockaddr sa;

    // Why use copy_from_user? cause if I don't it crashes, (user mode data and kernel mode data don't mix)
    if (copy_from_user(&sa, (struct sockaddr __user *)regs->si, sizeof(struct sockaddr))) {
        printk("lomond - Error acurred\n");
        return 0;
    }

    // Check if the socket family is AF_INET
    if(sa.sa_family == AF_INET)
    {
        struct sockaddr_in sa_in;
        if (copy_from_user(&sa_in, (struct sockaddr_in __user *)regs->si, sizeof(struct sockaddr_in))) 
        {
            printk("lomond - Error acurred\n");
            return 0;
        }
        uint32_t *destination_addr = &sa_in.sin_addr.s_addr;
        int destination_port = sa_in.sin_port;
        printk("lomond - connection made - %pI4:%d\n", destination_addr, destination_port);

    }

    return 0;
}

// This will be my kprobe, so I connect it to the sys_connect function and set my two functions as the pre function
static struct kprobe kp = {
    .symbol_name  = "__sys_connect",
    .pre_handler = *kpb_pre
};

int minit(void)
{
    printk("lomond - Module inserted\n");
    
    //When a kprobe is registered, Kprobes makes a copy of the probed instruction and replaces the first byte(s) of the probed instruction with a breakpoint instruction (e.g., int3 on i386 and x86_64). ~ Kernel docs
    register_kprobe(&kp);
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
MODULE_DESCRIPTION("lomond connect probe");
MODULE_LICENSE("GPL");
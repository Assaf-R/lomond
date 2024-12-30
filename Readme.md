# Lomond - Socket connection probe
### Who this?
Named After the Scottish loch, Lomond is a simple example on how to use kprobes in an LKM for learning purposes and gainning a deeper understanding of the Linux kernel.

This program creates a kprobe and attaches it to the __sys_connect syscall, every time a connection is made a check will be made if it's an IPv4 socket, and if so the target address and port wil be logged in the kernel logs as such.
```
[Mon Dec 30 17:48:00 2024] Lomond - connection made - 8.8.8.8:260
```

## Makefile tricks

### To compile
```
make
```
### To load compiled LKM
```
make i
```
### To remove loaded LKM
```
make r
```
### To ping google.com once (easy networking check)
```
make t
```
### To compile, load compiled LKM, and ping google.com
```
make full
```
### To print kernel logs (dmsg) and filter with our keyword
```
make d
```
#
## credits
Written by Assaf R.
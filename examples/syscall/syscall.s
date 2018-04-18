
# as -o syscall.o syscall.s
# ld -o syscall syscall.o

	.file "syscall.s"

.text
.globl _start

_start:
	movl $72,%eax   # Our new syscall, sys_hello
	int $0x80       # call kernel

	movl $0,%ebx    # first argument: exit code, true = 0
	movl $1,%eax    # sys_exit
	int $0x80       # call kernel

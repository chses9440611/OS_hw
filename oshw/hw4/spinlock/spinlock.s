	.text
	.globl	spin_lock
	.globl	spin_unlock
spin_lock:
	movl	$1, %edx
.L2:
	movl	$0, %eax
	lock cmpxchgl	%edx, (%rdi)
	jne	.L2
	;xorl	%eax, %eax
	ret

spin_unlock:
	movl	$0, (%rdi)
	ret


.text
.global spin_lock
.global spin_unlock

spin_lock:
	pushq %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
L1:
	cmp $0, -8(%rbp)
	je L1
	movl $0, -8(%rbp)

	pop %rbp
	ret

spin_unlock:
	push %rbp
	mov %rsp, %rbp
	movl $1, -8(%rbp)
	pop %rbp
	ret

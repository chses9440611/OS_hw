
.text
.global spin_lock
.global spin_unlock

spin_lock:
	push %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
.L1:
	movq -8(%rbp), %rax
	movl (%rax), %eax
	cmpl $1, %eax
	je .L1 
	movq -8(%rbp), %rax
	movl $1, (%rax)
	popq %rbp
	ret

spin_unlock:
	push %rbp
	movq %rsp, %rbp
	movq %rdi, -8(%rbp)
	movq -8(%rbp), %rax
	movl $0, (%rax)
	pop %rbp
	ret

	.file	"batt_update.c"
	.text
	.globl	set_batt_from_ports
	.type	set_batt_from_ports, @function
set_batt_from_ports:
.LFB52:
	.cfi_startproc
	movzwl	BATT_VOLTAGE_PORT(%rip), %eax
	testw	%ax, %ax
	js	.L7
	movw	%ax, (%rdi)
	cwtl
	subl	$3000, %eax
	cmpl	$807, %eax
	jle	.L3
	movb	$100, 2(%rdi)
.L4:
	testb	$1, BATT_STATUS_PORT(%rip)
	jne	.L8
	movb	$0, 3(%rdi)
	movl	$0, %eax
	ret
.L3:
	cmpl	$-7, %eax
	jge	.L5
	movb	$0, 2(%rdi)
	jmp	.L4
.L5:
	leal	7(%rax), %edx
	testl	%eax, %eax
	cmovns	%eax, %edx
	sarl	$3, %edx
	movb	%dl, 2(%rdi)
	jmp	.L4
.L8:
	movb	$1, 3(%rdi)
	movl	$0, %eax
	ret
.L7:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE52:
	.size	set_batt_from_ports, .-set_batt_from_ports
	.globl	set_display_from_batt
	.type	set_display_from_batt, @function
set_display_from_batt:
.LFB53:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$80, %rsp
	.cfi_def_cfa_offset 96
	movq	%fs:40, %rax
	movq	%rax, 72(%rsp)
	xorl	%eax, %eax
	movl	%edi, %r8d
	sall	$8, %r8d
	sarl	$24, %r8d
	cmpb	$4, %r8b
	jle	.L10
	movl	$2, %eax
.L10:
	cmpb	$29, %r8b
	jle	.L11
	addl	$1, %eax
	addl	%eax, %eax
.L12:
	cmpb	$49, %r8b
	jle	.L13
	leal	2(%rax,%rax), %eax
.L14:
	cmpb	$69, %r8b
	jle	.L15
	leal	2(%rax,%rax), %eax
.L16:
	cmpb	$89, %r8b
	jle	.L17
	leal	2(%rax,%rax), %eax
.L18:
	movl	%edi, %r11d
	sarl	$24, %r11d
	cmpb	$1, %r11b
	je	.L31
	leal	3(,%rax,4), %ecx
.L20:
	movl	$63, 16(%rsp)
	movl	$3, 20(%rsp)
	movl	$109, 24(%rsp)
	movl	$103, 28(%rsp)
	movl	$83, 32(%rsp)
	movl	$118, 36(%rsp)
	movl	$126, 40(%rsp)
	movl	$35, 44(%rsp)
	movl	$127, 48(%rsp)
	movl	$119, 52(%rsp)
	movl	$0, 56(%rsp)
	testb	%r11b, %r11b
	jne	.L21
	movswl	%di, %edx
	imull	$-31981, %edx, %edx
	shrl	$16, %edx
	addl	%edi, %edx
	sarw	$9, %dx
	movl	%edi, %eax
	sarw	$15, %ax
	subl	%eax, %edx
	movswl	%dx, %r9d
	imulw	$1000, %dx, %dx
	movl	%edi, %ebx
	subl	%edx, %ebx
	movswl	%bx, %ebx
	movl	$1374389535, %edx
	movl	%ebx, %eax
	imull	%edx
	sarl	$5, %edx
	movl	%ebx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %r10d
	imull	$100, %edx, %eax
	subl	%eax, %ebx
	movl	%ebx, %edi
	addl	$5, %edi
	movl	$1717986919, %edx
	movl	%edi, %eax
	imull	%edx
	sarl	$2, %edx
	sarl	$31, %edi
	subl	%edi, %edx
.L21:
	imull	$103, %r8d, %eax
	sarw	$10, %ax
	movl	%r8d, %edi
	sarb	$7, %dil
	subl	%edi, %eax
	movsbl	%al, %eax
	cmpb	$1, %r11b
	je	.L32
.L22:
	cmpb	$1, %r11b
	je	.L33
.L24:
	cmpb	$1, %r11b
	je	.L34
.L23:
	movl	%r9d, 4(%rsp)
	movl	%r10d, 8(%rsp)
	movl	%edx, 12(%rsp)
	movl	$0, %eax
	jmp	.L25
.L11:
	addl	%eax, %eax
	jmp	.L12
.L13:
	addl	%eax, %eax
	jmp	.L14
.L15:
	addl	%eax, %eax
	jmp	.L16
.L17:
	addl	%eax, %eax
	jmp	.L18
.L31:
	leal	4(,%rax,4), %ecx
	jmp	.L20
.L32:
	cmpl	$10, %eax
	jne	.L22
	movl	$0, %edx
	movl	$0, %r10d
	movl	$1, %r9d
	jmp	.L23
.L33:
	testl	%eax, %eax
	jne	.L24
	movsbl	%r8b, %edx
	movl	$10, %r10d
	movl	$10, %r9d
	jmp	.L23
.L34:
	imull	$103, %r8d, %edx
	sarw	$10, %dx
	movl	%r8d, %edi
	sarb	$7, %dil
	subl	%edi, %edx
	leal	(%rdx,%rdx,4), %edx
	leal	(%rdx,%rdx), %edi
	movl	%r8d, %edx
	subl	%edi, %edx
	movsbl	%dl, %edx
	movl	%eax, %r10d
	movl	$10, %r9d
	jmp	.L23
.L26:
	sall	$7, %ecx
	movslq	%eax, %rdx
	movslq	4(%rsp,%rdx,4), %rdx
	addl	16(%rsp,%rdx,4), %ecx
	addl	$1, %eax
.L25:
	cmpl	$2, %eax
	jle	.L26
	movl	%ecx, (%rsi)
	movl	$0, %eax
	movq	72(%rsp), %rsi
	xorq	%fs:40, %rsi
	jne	.L35
	addq	$80, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L35:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE53:
	.size	set_display_from_batt, .-set_display_from_batt
	.globl	batt_update
	.type	batt_update, @function
batt_update:
.LFB54:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	leaq	4(%rsp), %rdi
	call	set_batt_from_ports
	cmpl	$1, %eax
	je	.L36
	leaq	BATT_DISPLAY_PORT(%rip), %rsi
	movl	4(%rsp), %edi
	call	set_display_from_batt
	cmpl	$1, %eax
	je	.L36
	movl	$0, %eax
.L36:
	movq	8(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L41
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L41:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE54:
	.size	batt_update, .-batt_update
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits

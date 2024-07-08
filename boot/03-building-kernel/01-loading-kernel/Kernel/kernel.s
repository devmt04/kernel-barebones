	.file	"kernel.c"
	.text
	.globl	write_char
	.type	write_char, @function
write_char:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %ecx
	movl	%esi, %eax
	movq	%rdx, -32(%rbp)
	movl	%ecx, %edx
	movb	%dl, -20(%rbp)
	movb	%al, -24(%rbp)
	movzbl	-24(%rbp), %eax
	sall	$8, %eax
	movl	%eax, %edx
	movsbw	-20(%rbp), %ax
	orl	%edx, %eax
	movw	%ax, -2(%rbp)
	movq	-32(%rbp), %rax
	movzwl	-2(%rbp), %edx
	movw	%dx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	write_char, .-write_char
	.globl	print_msg
	.type	print_msg, @function
print_msg:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movabsq	$8022916924116329800, %rax
	movq	%rax, -24(%rbp)
	movl	$6581362, -16(%rbp)
	movq	$753664, -8(%rbp)
	movw	$0, -10(%rbp)
	movw	$0, -12(%rbp)
	movswl	-10(%rbp), %eax
	cltq
	movzbl	-24(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	$753664, %edx
	movl	$15, %esi
	movl	%eax, %edi
	call	write_char
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	print_msg, .-print_msg
	.globl	clear_screen
	.type	clear_screen, @function
clear_screen:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$753664, -16(%rbp)
	movl	$4000, -20(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L5
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	clear_screen, .-clear_screen
	.globl	main
	.type	main, @function
main:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	clear_screen
	movl	$0, %eax
	call	print_msg
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits

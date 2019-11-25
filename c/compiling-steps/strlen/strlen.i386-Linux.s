	.file	"strlen.c"
	.text
.globl countLen
	.type	countLen,@function
countLen:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)
.L5:
	movl	8(%ebp), %eax
	incl	8(%ebp)
	cmpb	$0, (%eax)
	jne	.L7
	jmp	.L6
.L7:
	leal	-4(%ebp), %eax
	incl	(%eax)
	jmp	.L5
.L6:
	movl	-4(%ebp), %eax
	leave
	ret
.Lfe1:
	.size	countLen,.Lfe1-countLen
	.section	.rodata
.LC0:
	.string	"My dog has no nose"
.LC1:
	.string	"Then how does he smell?"
.LC2:
	.string	"Terrible!"
.LC3:
	.string	"%d : %2d : %s\n"
	.text
.globl main
	.type	main,@function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	movl	$.LC0, -24(%ebp)
	movl	$.LC1, -20(%ebp)
	movl	$.LC2, -16(%ebp)
	movl	$0, -12(%ebp)
	movl	$0, -28(%ebp)
.L9:
	movl	-28(%ebp), %eax
	cmpl	$0, -24(%ebp,%eax,4)
	jne	.L12
	jmp	.L10
.L12:
	subl	$12, %esp
	movl	-28(%ebp), %eax
	pushl	-24(%ebp,%eax,4)
	call	countLen
	addl	$16, %esp
	movl	%eax, -32(%ebp)
	movl	-28(%ebp), %eax
	pushl	-24(%ebp,%eax,4)
	pushl	-32(%ebp)
	pushl	-28(%ebp)
	pushl	$.LC3
	call	printf
	addl	$16, %esp
	leal	-28(%ebp), %eax
	incl	(%eax)
	jmp	.L9
.L10:
	subl	$12, %esp
	pushl	$0
	call	exit
.Lfe2:
	.size	main,.Lfe2-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.2.3 20030502 (Red Hat Linux 3.2.3-58)"

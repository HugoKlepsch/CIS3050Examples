	.text
.globl _countLen
_countLen:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -12(%ebp)
	jmp	L2
L3:
	leal	-12(%ebp), %eax
	incl	(%eax)
L2:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	setne	%al
	incl	8(%ebp)
	testb	%al, %al
	jne	L3
	movl	-12(%ebp), %eax
	leave
	ret
	.cstring
LC0:
	.ascii "My dog has no nose\0"
LC1:
	.ascii "Then how does he smell?\0"
LC2:
	.ascii "Terrible!\0"
LC3:
	.ascii "%d : %2d : %s\12\0"
	.text
.globl _main
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$52, %esp
	call	L11
"L00000000001$pb":
L11:
	popl	%ebx
	leal	LC0-"L00000000001$pb"(%ebx), %eax
	movl	%eax, -32(%ebp)
	leal	LC1-"L00000000001$pb"(%ebx), %eax
	movl	%eax, -28(%ebp)
	leal	LC2-"L00000000001$pb"(%ebx), %eax
	movl	%eax, -24(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -16(%ebp)
	jmp	L7
L8:
	movl	-16(%ebp), %eax
	movl	-32(%ebp,%eax,4), %eax
	movl	%eax, (%esp)
	call	_countLen
	movl	%eax, -12(%ebp)
	movl	-16(%ebp), %eax
	movl	-32(%ebp,%eax,4), %eax
	movl	%eax, 12(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC3-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	L_printf$stub
	leal	-16(%ebp), %eax
	incl	(%eax)
L7:
	movl	-16(%ebp), %eax
	movl	-32(%ebp,%eax,4), %eax
	testl	%eax, %eax
	jne	L8
	movl	$0, (%esp)
	call	L_exit$stub
	.section __IMPORT,__jump_table,symbol_stubs,self_modifying_code+pure_instructions,5
L_printf$stub:
	.indirect_symbol _printf
	hlt ; hlt ; hlt ; hlt ; hlt
L_exit$stub:
	.indirect_symbol _exit
	hlt ; hlt ; hlt ; hlt ; hlt
	.subsections_via_symbols

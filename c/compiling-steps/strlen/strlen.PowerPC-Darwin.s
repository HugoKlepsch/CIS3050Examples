	.section __TEXT,__text,regular,pure_instructions
	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.machine ppc
	.text
	.align 2
	.globl _countLen
_countLen:
	stmw r30,-8(r1)
	stwu r1,-64(r1)
	mr r30,r1
	stw r3,88(r30)
	li r0,0
	stw r0,24(r30)
	b L2
L3:
	lwz r2,24(r30)
	addi r0,r2,1
	stw r0,24(r30)
L2:
	lwz r2,88(r30)
	lbz r0,0(r2)
	extsb r0,r0
	rlwinm r0,r0,0,0xff
	neg r0,r0
	srwi r0,r0,31
	mr r9,r0
	lwz r2,88(r30)
	addi r0,r2,1
	stw r0,88(r30)
	cmpwi cr7,r9,0
	bne cr7,L3
	lwz r0,24(r30)
	mr r3,r0
	lwz r1,0(r1)
	lmw r30,-8(r1)
	blr
	.cstring
	.align 2
LC0:
	.ascii "My dog has no nose\0"
	.align 2
LC1:
	.ascii "Then how does he smell?\0"
	.align 2
LC2:
	.ascii "Terrible!\0"
	.const_data
	.align 2
_C.24.2378:
	.long	LC0
	.long	LC1
	.long	LC2
	.long	0
	.cstring
	.align 2
LC3:
	.ascii "%d : %2d : %s\12\0"
	.text
	.align 2
	.globl _main
_main:
	mflr r0
	stmw r30,-8(r1)
	stw r0,8(r1)
	stwu r1,-112(r1)
	mr r30,r1
	bcl 20,31,"L00000000001$pb"
"L00000000001$pb":
	mflr r31
	addis r2,r31,ha16(_C.24.2378-"L00000000001$pb")
	la r2,lo16(_C.24.2378-"L00000000001$pb")(r2)
	lwz r0,0(r2)
	lwz r9,4(r2)
	lwz r11,8(r2)
	lwz r2,12(r2)
	stw r0,64(r30)
	stw r9,68(r30)
	stw r11,72(r30)
	stw r2,76(r30)
	li r0,0
	stw r0,60(r30)
	b L7
L8:
	lwz r0,60(r30)
	slwi r2,r0,2
	addi r0,r30,56
	add r2,r2,r0
	addi r2,r2,8
	lwz r0,0(r2)
	mr r3,r0
	bl _countLen
	mr r0,r3
	stw r0,56(r30)
	lwz r0,60(r30)
	slwi r2,r0,2
	addi r0,r30,56
	add r2,r2,r0
	addi r2,r2,8
	lwz r0,0(r2)
	addis r2,r31,ha16(LC3-"L00000000001$pb")
	la r3,lo16(LC3-"L00000000001$pb")(r2)
	lwz r4,60(r30)
	lwz r5,56(r30)
	mr r6,r0
	bl L_printf$LDBLStub$stub
	lwz r2,60(r30)
	addi r0,r2,1
	stw r0,60(r30)
L7:
	lwz r0,60(r30)
	slwi r2,r0,2
	addi r0,r30,56
	add r2,r2,r0
	addi r2,r2,8
	lwz r0,0(r2)
	cmpwi cr7,r0,0
	bne cr7,L8
	li r3,0
	bl L_exit$stub
	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 5
L_printf$LDBLStub$stub:
	.indirect_symbol _printf$LDBLStub
	mflr r0
	bcl 20,31,"L00000000001$spb"
"L00000000001$spb":
	mflr r11
	addis r11,r11,ha16(L_printf$LDBLStub$lazy_ptr-"L00000000001$spb")
	mtlr r0
	lwzu r12,lo16(L_printf$LDBLStub$lazy_ptr-"L00000000001$spb")(r11)
	mtctr r12
	bctr
	.lazy_symbol_pointer
L_printf$LDBLStub$lazy_ptr:
	.indirect_symbol _printf$LDBLStub
	.long	dyld_stub_binding_helper
	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 5
L_exit$stub:
	.indirect_symbol _exit
	mflr r0
	bcl 20,31,"L00000000002$spb"
"L00000000002$spb":
	mflr r11
	addis r11,r11,ha16(L_exit$lazy_ptr-"L00000000002$spb")
	mtlr r0
	lwzu r12,lo16(L_exit$lazy_ptr-"L00000000002$spb")(r11)
	mtctr r12
	bctr
	.lazy_symbol_pointer
L_exit$lazy_ptr:
	.indirect_symbol _exit
	.long	dyld_stub_binding_helper
	.subsections_via_symbols

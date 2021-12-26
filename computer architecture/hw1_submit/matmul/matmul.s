.globl	matmul
matmul:
/* save return address (in x1) in stack*/
/* first argument (x10): the address of output buffer */
/* second argument (x11): the start address of a */
/* third argument (x12): the start address of b */
/* fourth argument (x13): the dimension */
/* your matmul code from here */
# addi sp, sp, -8
# sd x1, 0(sp)
# /* ...*/
# ld x1, 0(sp)
# addi sp, sp, 8
/* your matmul code to here */
	addi	sp,sp,-64
	sd	x1,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	x10,-40(s0)
	sd	x11,-48(s0)
	sd	x12,-56(s0)

	sw	x13,-60(s0)
	sw	x0,-20(s0) #i call as a4
	j	Exit
j_to_0:
	sw	x0,-24(s0) # make j and call as a5
	j	Loop1
Loop2:
	lw	a5,-24(s0)
	lw	a4,-20(s0)
	ld	x12,-56(s0)
	call	matmul_idx
	lw	a5,-24(s0)
	add	a5,a5,1
	sw	a5,-24(s0)
Loop1:
	lw a5,-24(s0)
	lw	a4,-20(s0)
	lw	x13,-60(s0)
	blt	a5,x13,Loop2
	
	add	a4,a4,1
	sw	a4,-20(s0)
Exit:
	lw	x19, -20(s0)
	blt	x19,x13,j_to_0
	ld	x1,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	x1

.globl	matmul_idx
matmul_idx:
/* Recommanded arguments */
/* first argument (x10): the address of output buffer */
/* second argument (x11): the start address of a */
/* third argument (x12): the start address of b */
/* fourth argument (x13): the dimension */
/* fourth argument (x14): row index of the result matrix to fill
out */
/* fourth argument (x15): column index of the result matrix to
fill out */
/* your matmul_idx (helper function) code from here */
/* your matmul_idx (helper function) code to here */
	addi	sp,sp,-80
	sd	s0,72(sp)
	addi	s0,sp,80
	sd	x10,-40(s0)	#	x10 : output address
	sd	x11,-48(s0)	#	x11 : a address
	sd	x12,-56(s0)	#	x12 : b address
	mv	x12,a3
	sw	x13,-60(s0)	#	x13 : dimension
	sw	x14,-64(s0)	#	x14 : row
	sw	x15,-68(s0)	#	x15 : column
	
	
	lw	x14,-64(s0)	#	x14 : row
	lw	a3,-60(s0)	#	x13 : dimension
	mul	a3,x14,a3	#	dimension = x * dim
	lw	x14,-68(s0)
	add	a3,x14,a3	#	dimextion = column + dimention -> int c = x*dim+y;
	sw	a3,-24(s0)
	
	lw	a4,-24(s0)
	slli	a4,a4,2
	
	ld	a6,-40(s0)
	add	a4,a6,a4
	sw	x0,0(a4)
	sw	x0,-20(s0) # k from 1 call a6
	j	Exit_2
Loop:
	lw	a5,-24(s0)
	
	slli	a5,a5,2
	add	a5,x10,a5
	lw	x13,0(a5)
	lw	x14,-64(s0)
	lw	a5,-60(s0)
	mul	a5,a4,a5
	lw	a6,-20(s0)
	add	a5,a6,a5

	slli	a5,a5,2	#	a
	add	a5,x11,a5
	lw	a4,0(a5)
	lw	a2,-60(s0)
	lw	a5,-20(s0)
	mul	a5,a2,a5
	lw	a2,-68(s0)
	add	a5,a2,a5

	slli	a5,a5,2	#	b
	ld	a2,-56(s0)
	add	a5,a2,a5
	lw	a5,0(a5)
	mul	a5,a4,a5
	sext.w	a4,a5
	lw	a5,-24(s0)

	slli	a5,a5,2	#	output
	ld	a2,-40(s0)
	add	a5,a2,a5
	add	a4,a3,a4
	sw	a4,0(a5)

	lw	a6,-20(s0)	# k =k+1
	add	a6,a6,1
	sw	a6,-20(s0)

Exit_2:
	lw	a4,-20(s0)
	lw	a5,-60(s0)

	blt	a4,a5,Loop
	ld	s0,72(sp)
	add	sp,sp,80
	jr	x1
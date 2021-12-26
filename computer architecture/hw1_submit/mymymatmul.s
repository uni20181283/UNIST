.global matmul
matmul:
	addi	sp,sp,-64
	sd	ra,56(sp)
	sd	s0,48(sp)
	# addi	s0,sp,64
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	mv	a5,a3
	sw	a5,-60(s0)
	sw	zero,-20(s0)
	j	.L5
.L8:
	sw	zero,-24(s0)
	j	.L6
.L7:
	lw	a5,-24(s0)
	lw	a4,-20(s0)
	lw	a3,-60(s0)
	ld	a2,-56(s0)
	ld	a1,-48(s0)
	ld	a0,-40(s0)
	call	matmul_idx
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sw	a5,-24(s0)
.L6:
	lw	a5,-24(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L7
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L5:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	blt	a4,a5,.L8
	ld	ra,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	x1



.global matmul_idx
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
	sd	a0,-40(s0)
	sd	a1,-48(s0)
	sd	a2,-56(s0)
	mv	a2,a3
	mv	a3,a4
	mv	a4,a5
	mv	a5,a2
	sw	a5,-60(s0)
	mv	a5,a3
	sw	a5,-64(s0)
	mv	a5,a4
	sw	a5,-68(s0)
	lw	a5,-64(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	mulw	a5,a4,a5
	lw	a4,-68(s0)
	addw	a5,a4,a5
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	sw	zero,0(a5)
	sw	zero,-20(s0)
	j	.L2
loop:
	lw	a5,-64(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	mul	a5,a4,a5
	lw	a4,-68(s0)
	add	a5,a4,a5
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a3,0(a5)
	lw	a5,-64(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	mul	a5,a4,a5
	lw	a4,-20(s0)
	add	a5,a4,a5
	slli	a5,a5,2
	ld	a4,-48(s0)
	add	a5,a4,a5
	lw	a4,0(a5)
	lw	a5,-60(s0)
	mv	a2,a5
	lw	a5,-20(s0)
	mul	a5,a2,a5
	lw	a2,-68(s0)
	add	a5,a2,a5
	slli	a5,a5,2
	ld	a2,-56(s0)
	add	a5,a2,a5
	lw	a5,0(a5)
	mul	a5,a4,a5
	sext.w	a4,a5
	lw	a5,-64(s0)
	mv	a2,a5
	lw	a5,-60(s0)
	mul	a5,a2,a5
	lw	a2,-68(s0)
	add	a5,a2,a5
	slli	a5,a5,2
	ld	a2,-40(s0)
	add	a5,a2,a5
	add	a4,a3,a4
	sw	a4,0(a5)
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-60(s0)
	blt	a4,a5,loop
	ld	s0,72(sp)
	add	sp,sp,80
	jr	x1

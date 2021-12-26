.global sort
sort:
/* first argument (x10): the address of output buffer */
/* second argument (x11): the start address of the incoming list
*/
/* third argument (x12): the length of incoming list */
/* your sort code from here */
/* your sort code to here */
	addi	sp,sp,-64
	sd	x1,56(sp)
	sd	s0,48(sp)
	addi	s0,sp,64
	sd	x10,-40(s0)	#	address output
	sd	x11,-48(s0)	#	start incoming address
	sw	x12,-52(s0)	#	length of list
	# sw	x0,-20(s0)	# make first zero
	j	Exit
Loop:
	lw	a5,-20(s0)
	ld	a4,-48(s0)
	add	a5,a4,a5

	lw	a4,0(a5)
	lw	a5,-20(s0)


	call	insert
	lw	a5,-20(s0)
	add	a5,a5,1
	sw	a5,-20(s0)
Exit:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-52(s0)
	blt	a4,a5,Loop

	ld	x1,56(sp)
	ld	s0,48(sp)
	addi	sp,sp,64
	jr	x1

.globl	insert
insert:
/* Recommanded arguments */
/* first argument (x10): the address of output buffer */
/* third argument (x11): the current length of output buffer */
/* third argument (x12): the integer to insert */
/* your insert code from here */
/* your insert code to here */
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	sd	x10,-40(s0)	#	output address
	sw	a5,-44(s0)	#	current length
	sw	a1,-48(s0)	#	insert integer
	mv	a5,a4

	lw	a5,-44(s0)
	add	a5,a5,-1
	sw	a5,-20(s0)
	j	Loop2
Loop1:
	lw	a3,-20(s0)
	add	x10,x10,a3
	add	a3,a3,1		#	j + 1
	add	a3,x10,a3	#	out[j + 1]
	
	add	a3,a3,-1	#	j = j -1
	sw	a3,-20(s0)
Loop2:
	lw	a5,-20(s0)
	lw	a4,-48(s0)

	blt	a5,x0,Exit_2
	add	a5,x10,a5	#	out[j + 1] = !!
	blt	a5,x0,Loop1
Exit_2:
	lw	a3,-20(s0)
	add	a3,a3,1
	slli	a3,a3,2
	ld	a4,-40(s0)
	add	a3,a4,a3
	sw	a4,0(a3)
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	x1
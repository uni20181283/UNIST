
.global gcd
gcd:
/* Your Code for gcd from here */
/* a @ x10, b @ x11 returns the gcd*/
/* Your Code for gcd to here */

	beq x11, x0, L1
	rem x6, x10, x11
	mv x10, x11
	mv x11, x6
	j gcd
	
L1:	jr x1



.global lcm
lcm:
/* Your Code for lcm from here */
/* a @ x10, b @ x11 returns the lcm*/
/* Your Code for lcm to here */

	mul x5, x10, x11
loop:
	beq x11, x0, L2
	rem x7, x10, x11
	mv x10, x11
	mv x11, x7
	j loop

L2:	
	div x5, x5, x10
	mv x10, x5
	jr x1

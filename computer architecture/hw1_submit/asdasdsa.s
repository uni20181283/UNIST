.global matmul
matmul:
/* save return address (in x1) in stack*/
/* first argument (x10): the address of output buffer */
/* second argument (x11): the start address of a */
/* third argument (x12): the start address of b */
/* fourth argument (x13): the dimension */
/* your matmul code from here */
    addi sp, sp, -64
    sd x1, 0(sp)
    sd x10, 8(sp)
    sd x11, 16(sp)
    sd x12, 24(sp)
    sd x13, 32(sp)
    sd x19, 36(sp)  # int i = 0
    sd x20, 40(sp)  # int j = 0
    /* ...*/
    ld x1, 0(sp)
Loop_matmul:
    add x5, x19, x11
    add x6, x20, x12
    blt
    
/* your matmul code to here */
L2: addi sp, sp, 64
    jk x1
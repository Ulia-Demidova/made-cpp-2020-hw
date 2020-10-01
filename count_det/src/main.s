.textш
.global count_det

count_det:
        push {r4-r10}
        ldr r1, [r0], #4
        ldr r2, [r0], #4
        ldr r3, [r0], #4
        ldr r4, [r0], #4
        ldr r5, [r0], #4
        ldr r6, [r0], #4
        ldr r7, [r0], #4
        ldr r8, [r0], #4
        ldr r9, [r0], #4

        mul r10, r5, r1
        mul r10, r9, r10
        mul r1, r6, r1
        mul r1, r8, r1
        mul r9, r4, r9
        mul r9, r2, r9
        mul r2, r6, r2
        mul r2, r7, r2
        mul r4, r4, r5
        mul r4, r4, r8
        mul r3, r3, r5
        mul r3, r3, r7

        sub r0, r10, r1
        sub r0, r0, r9
        add r0, r2
        add r0, r4
        sub r0, r0, r3

        pop {r4-r10}

        bx lr

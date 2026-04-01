.section ".itcm", "ax"
.arm

// r0 = context pointer
.global dspadpcm_decode
.type dspadpcm_decode, %function
dspadpcm_decode:    
    stmfd sp!, {r4, r5, r6, r7, r8, lr}

    mov lr, #1024
    ldr r5,= #0x7FFFFFFF

    ldmia r0!, {r1, r2} // context->coefTable, context->data
    strb lr, [r0, #(36 - 8)] // context->frameIdx = 0
    add r2, r2, #8
    str r2, [r0, #(4 - 8)] // context->data += 8
    ldrb r3, [r2, #-8]!
    ldr r7, [r0, #(32 - 8)] // r7 = last1_last2

    and r8, r3, #0xF0
    ldr r6, [r1, r8, lsr #2] // r6 = coef2_coef1

    and r1, r3, #0xF
    add r1, r1, #11 // r1 = scale

    mov r4, #7
sample_loop:
    ldrsb r3, [r2, #1]!
    smlatb r8, r6, r7, lr // r8  = coef2 * last2 + 1024
    smlabt r8, r6, r7, r8 // r8 += coef1 * last1
    mov r3, r3, ror #4
    add r8, r8, r3, lsl r1// r8 += nibble << scale

    mov r12, r8, lsl #5
    cmp r8, r12, asr #5
    eorne r12, r5, r8, asr #31

    smlatt r7, r6, r7, lr
    smlabt r7, r6, r12, r7
    mov r3, r3, asr #28
    add r7, r7, r3, lsl r1

    mov r8, r7, lsl #5
    cmp r7, r8, asr #5
    eorne r8, r5, r7, asr #31

    bic r8, r8, r5, lsr #15
    orr r7, r8, r12, lsr #16

    str r7, [r0], #4

    subs r4, r4, #1
    bne sample_loop

    ldmfd sp!, {r4, r5, r6, r7, r8, pc}
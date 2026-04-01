.section ".itcm", "ax"
.arm

// r0 = bitmap
// r1 = vram
.global cover_bitmapToTiledCopy
.type cover_bitmapToTiledCopy, %function
cover_bitmapToTiledCopy:
    push {r4-r9}
    mov r12, #12
1:
    sub r12, r12, #(14 << 16)
2:
    ldrd r2, [r0], #128
    ldrd r4, [r0], #128
    ldrd r6, [r0], #128
    ldrd r8, [r0], #128
    stmia r1!, {r2-r9}
    ldrd r2, [r0], #128
    ldrd r4, [r0], #128
    ldrd r6, [r0], #128
    ldrd r8, [r0]
    stmia r1!, {r2-r9}
    sub r0, r0, #(7 * 128 - 8)
    adds r12, r12, #(1 << 16)
    bmi 2b
    add r0, r0, #(128 * 8 - (14 * 8))
    subs r12, r12, #1
    bne 1b

    pop {r4-r9}
    bx lr

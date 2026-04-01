.section .itcm
.arm

.global sh_callSemihostingFunction
sh_callSemihostingFunction:
    swi 0x123456
    bx lr
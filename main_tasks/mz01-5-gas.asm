.text

.global exchange

exchange: 
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %edi
    pushl %esi
    xorl %ecx, %ecx
    leal (%eax, %edx, 4), %ebx
    subl $4, %ebx
    shrl $1, %edx
.L: 
    testl %edx, %edx
    je .EL
    movl (%eax), %edi
    movl (%ebx), %esi
    cmpl %esi, %edi
    jna .skip
    movl %edi, (%ebx)
    movl %esi, (%eax)
    incl %ecx
.skip: 
    addl $4, %eax
    subl $4, %ebx
    decl %edx
    jmp .L
.EL: 
    popl %esi
    popl %edi
    popl %ebx
    movl %ebp, %esp
    popl %ebp
    ret

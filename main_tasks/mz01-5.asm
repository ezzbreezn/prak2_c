%include "io.inc"

section .bss
    a resd 1000
    b resd 1

section .text

exchange:
    push ebp
    mov ebp, esp
    push ebx
    push edi
    push esi
    xor ecx, ecx
    lea ebx, [eax + 4 * edx]
    sub ebx, 4
    shr edx, 1
.L:
    test edx, edx
    je .EL
    mov edi, dword[eax]
    mov esi, dword[ebx]
    cmp edi, esi
    jna .skip
    mov dword[ebx], edi 
    mov dword[eax], esi
    inc ecx
.skip: 
    add eax, 4
    sub ebx, 4
    dec edx
    jmp .L
.EL:
    pop esi
    pop edi
    pop ebx
    mov esp, ebp
    pop ebp
    ret

global CMAIN
CMAIN:
    GET_DEC 4, edx
    mov dword[b], edx
    xor ecx, ecx
.INPUT:
    cmp ecx, edx
    je .EINPUT
    GET_DEC 4, [a + 4 * ecx]
    inc ecx
    jmp .INPUT
.EINPUT:    
    mov eax, a
    call exchange
    PRINT_DEC 4, ecx
    NEWLINE
    xor ecx, ecx
.OUTPUT:
    cmp ecx, dword[b]
    je .EOUTPUT
    PRINT_DEC 4, [a + 4 * ecx]
    PRINT_CHAR ' '
    inc ecx
    jmp .OUTPUT
.EOUTPUT:
    xor eax, eax
    ret
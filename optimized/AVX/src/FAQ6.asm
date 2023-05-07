section .text

global _FAQ6

_FAQ6:
        movzx   eax, BYTE [rdi]
        test    al, al
        je      .null
        xor     edx, edx
.lp:
        add     rax, rdx
        add     rdi, 1
        mov     rdx, rax
        sal     rdx, 10
        add     rax, rdx
        mov     rdx, rax
        shr     rdx, 6
        xor     rdx, rax

        movzx   eax, BYTE [rdi]
        test    al, al
        jne     .lp

        lea     rax, [rdx+rdx*8]
        mov     rdx, rax
        shr     rdx, 11
        xor     rdx, rax
        mov     rax, rdx
        sal     rax, 15
        add     rax, rdx

        ret

.null:
        xor     eax, eax
        ret
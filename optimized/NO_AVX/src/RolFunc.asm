section .text
global _AsmRolFunc

_AsmRolFunc:
            mov rax, rdi   ; first arg
            rol rax, 1

            ret 
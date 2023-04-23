section .text
global _RolFunc

_RolFunc:
            mov rax, rdi   ; first arg
            rol rax, 1

            ret 
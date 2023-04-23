section .text
global _RolHash

_RolHash:   
            xor rax, rax
            movsx rdx, BYTE [rdi]   ; first arg is string
            test dl, dl             ; if char = 0
            je done

RolFunc:    rol rax, 1
            xor rax, rdx 

            inc rdi
            movsx rdx, BYTE [rdi]
            test dl, dl
            jne RolFunc

            ret

done:
            mov eax, 0
            ret 

GLOBAL lockMutexASM
GLOBAL unlockMutexASM

final:
    mov rsp, rbp
    pop rbp
    ret

lockMutexASM:
	push rbp
	mov rbp, rsp
	xor rax, rax
	mov rax, 0
	xchg rax, [rdi]
	jmp final

unlockMutexASM:
	push rbp
	mov rbp, rsp
	xor rax, rax
	mov rax, 1
	xchg rax, [rdi]
	jmp final

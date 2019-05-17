section .bss
registers: resb 128

section .text
start:
l0:
mov dword [registers+0], 5
l3:
call l0
l5:
l7:
ret
l8:
mov dword [registers+4], 2
l11:
mov dword [registers+80], 1
l14:
mov dword [registers+120], 1
l17:
mov eax, dword [registers+0]
mov ebx, dword [registers+4]
sub eax, ebx
push eax
l20:
pop eax
cmp eax, 0
jl l34
l22:
mov eax, dword [registers+120]
mov ebx, dword [registers+4]
imul eax, ebx
push eax
l25:
pop eax
mov dword [registers+120], eax
l27:
mov eax, dword [registers+4]
mov ebx, dword [registers+80]
add eax, ebx
push eax
l30:
pop eax
mov dword [registers+4], eax
l32:
jmp l17
l34:
ret
mov eax, 5
call fattoriale
; display edx
ret

fattoriale:
mov ebx, 2
mov ecx, 1
mov edx, 1

inizio:
sub eax, ebx
cmp eax, 0
jl fine
imul edx, ebx
add ebx, ecx
jmp inizio

fine:
ret

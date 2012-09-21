extern catch_open_2_arg
extern catch_open_3_arg

global catch_open

section .data
    msg1 db 'argc = 3', 10
    msg1len dd $ - msg1

    msg2 db 'argc = 2', 10
    msg2len dd $ - msg2

section .text

catch_open:

    push ebp
    mov ebp, esp

    pushad

    mov eax, [ebp + 16]

    cmp eax, 0777
    jg .label1

    cmp eax, 0
    jl .label1

    mov eax, [ebp + 16]
    push eax
    mov eax, [ebp + 12]
    push eax
    mov eax, [ebp + 8]
    push eax
    call catch_open_3_arg

    pop eax
    pop eax
    pop eax

    jmp .label2

.label1:

    mov eax, [ebp + 12]
    push eax
    mov eax, [ebp + 8]
    push eax
    call catch_open_2_arg
    pop eax
    pop eax

.label2:
    popad
    mov esp, ebp
    pop ebp

    ret

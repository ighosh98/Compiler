extern printf
extern scanf
extern exit

SECTION .data
true_output: db 'Output: true',10,0
false_output: db 'Output: false',10,0
integer_output: db 'Output: %d',10,0
output_str: db 'Output: ',0
single_int: db '%d ',0
single_false: db 'false ',0
single_true: db 'true ',0
nextline: db 10,0

SECTION .text
bits 32
global main

main:
	sub esp,8  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	mov edx, 10
	push edx 
	mov edx, 90
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	push edx 
	mov edx, 5
	push edx 
	mov edx, 2
	pop eax
	sub eax, edx    ;perform (eax - edx) subtraction
	mov edx, eax    ;store result of subtraction in edx
	pop eax
	add edx, eax
	mov [ebp+0], edx   ;assign value to a variable
	pushad
	push dword true_output
	call printf
	pop eax
	popad
	pushad
	push dword false_output
	call printf
	pop eax
	popad
	mov edx, 123
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, 0   ; assigning false
	mov [ebp+4], edx   ;assign value to a variable
	mov edx, [ebp+4]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	cmp edx,0
	mov edx, single_false
	cmove eax, edx
	mov edx, single_true
	cmovne eax, edx
	push eax
	call printf
	pop eax
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	mov edx, 1   ; assigning true
	mov [ebp+4], edx   ;assign value to a variable
	mov edx, [ebp+4]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	cmp edx,0
	mov edx, single_false
	cmove eax, edx
	mov edx, single_true
	cmovne eax, edx
	push eax
	call printf
	pop eax
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
exit_main:  call exit


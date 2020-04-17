extern printf
extern scanf
extern exit

SECTION .data
input_str_int: db 'Input: Enter an integer value',10,0
input_str_boolean: db 'Input: Enter a boolean value',10,0
input_format_int: db '%d',0
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
	pushad
	push dword input_str_int
	call printf
	pop eax
	popad
	pushad
	mov eax, ebp
	add eax, 0
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	pushad
	push dword input_str_boolean
	call printf
	pop eax
	popad
	pushad
	mov eax, ebp
	add eax, 4
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
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


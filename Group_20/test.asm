extern malloc
extern printf
extern scanf
extern exit

SECTION .data
bound_error_str: db 'RUNTIME ERROR: Array Index Out of Bounds',10,0
input_arr_int: db 'Input: Enter %d array elemts of integer type for range %d to %d',10,0
input_arr_boolean: db 'Input: Enter %d array elemts of boolean type for range %d to %d',10,0
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
	sub esp,16  ;allocating space on the stack
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
	mov edx, 5
	mov [ebp+4], edx   ;assign value to a variable
	sub esp, 20
	mov edx, [ebp+0]
	mov  [esp+0], edx
	mov edx, [ebp+4]
	mov  [esp+4], edx
	mov edx, [ebp+8]
	mov  [esp+8], edx
	mov edx, [ebp+12]
	mov  [esp+12], edx
	push ebp
	mov ebp, esp
	add ebp, 4
	call mod1
	pop ebp
	mov edx, [esp+8]
	mov  [ebp+8], edx
	mov edx, [esp+12]
	mov  [ebp+12], edx
	add esp, 20
	mov edx, [ebp+8]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+12]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
exit_main:  call exit

mod1:
	pushad
	mov edx, 10
	mov [ebp+16], edx   ;assign value to a variable
	mov edx, [ebp+0]
	push edx 
	mov edx, [ebp+4]
	push edx 
	mov edx, 10
	pop eax
	sub eax, edx    ;perform (eax - edx) subtraction
	mov edx, eax    ;store result of subtraction in edx
	pop eax
	add edx, eax
	mov [ebp+8], edx   ;assign value to a variable
	mov edx, [ebp+4]
	push edx 
	mov edx, 5
	pop eax
	push ebx
	mov ebx,edx
	mov edx, 0
	idiv ebx	;edx:eax divided by ebx result stored in eax
	mov edx, eax    ;move the quotient into edx
	pop ebx
	push edx 
	mov edx, [ebp+16]
	pop eax
	add edx, eax
	mov [ebp+12], edx   ;assign value to a variable
	popad
ret
	BOUND_ERROR:
	pushad
	push dword bound_error_str
	call printf
	pop eax
	popad
	call exit

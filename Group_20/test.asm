extern malloc
extern printf
extern scanf
extern exit

SECTION .data
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

compute_expr:
	pushad
	pushad
	push dword input_str_int
	call printf
	pop eax
	popad
	pushad
	mov eax, ebp
	add eax, 28
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	pushad
	push dword input_str_int
	call printf
	pop eax
	popad
	pushad
	mov eax, ebp
	add eax, 24
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	mov edx, [ebp+0]
	push edx 
	mov edx, [ebp+4]
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	push edx 
	mov edx, [ebp+28]
	push edx 
	mov edx, 2
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	push edx 
	mov edx, 3
	pop eax
	sub eax, edx    ;perform (eax - edx) subtraction
	mov edx, eax    ;store result of subtraction in edx
	pop eax
	add edx, eax
	mov [ebp+20], edx   ;assign value to a variable
	mov edx, [ebp+8]
	push edx
	mov edx, [ebp+0]
	push edx 
	mov edx, [ebp+4]
	pop eax
	cmp eax,edx
	mov edx,0   ;assign false initially in comparing
	mov eax, 1   ;cmov requires register, therefore place true in eax
	cmovle  edx , eax	;place true if condition met
	pop eax
	and edx, eax
	mov [ebp+32], edx   ;assign value to a variable
	mov edx, [ebp+20]
	push edx 
	mov edx, [ebp+24]
	pop eax
	add edx, eax
	mov [ebp+12], edx   ;assign value to a variable
	mov edx, [ebp+32]
	push edx
	mov edx, [ebp+20]
	push edx 
	mov edx, [ebp+24]
	pop eax
	cmp eax,edx
	mov edx,0   ;assign false initially in comparing
	mov eax, 1   ;cmov requires register, therefore place true in eax
	cmovle  edx , eax	;place true if condition met
	pop eax
	or edx, eax
	mov [ebp+16], edx   ;assign value to a variable
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+4]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+8]
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
	mov edx, [ebp+28]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+20]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+24]
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
	mov edx, [ebp+16]
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
	popad
ret
main:
	sub esp,20  ;allocating space on the stack
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
	pushad
	push dword input_str_int
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
	mov edx, 0   ; assigning false
	mov [ebp+16], edx   ;assign value to a variable
	sub esp, 36
	mov edx, [ebp+0]
	mov  [esp+0], edx
	mov edx, [ebp+4]
	mov  [esp+4], edx
	mov edx, [ebp+16]
	mov  [esp+8], edx
	mov edx, [ebp+8]
	mov  [esp+12], edx
	mov edx, [ebp+12]
	mov  [esp+16], edx
	push ebp
	mov ebp, esp
	add ebp, 4
	call compute_expr
	pop ebp
	mov edx, [esp+12]
	mov  [ebp+8], edx
	mov edx, [esp+16]
	mov  [ebp+12], edx
	add esp, 36
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+4]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+8]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+12]
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


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

main:
	sub esp,12  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	mov edx, 0
	mov [ebp+0], edx   ;assign value to a variable
	mov edx, 1
	mov [ebp+4], edx   ;assign value to a variable
	pushad
	push dword 52
	call malloc
	mov [ebp+8],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+8]	;base pointer to the array
	mov [edi], dword 0
	mov [edi+4], dword 10
	popad
	pushad
	mov edi, [ebp+8]
	push dword [edi+4]
	push dword [edi]
	mov eax, [edi+4]
	sub eax, [edi]
	add eax, 1
	push eax
	push dword input_arr_boolean
	call printf
	add esp, 16
	popad
	pushad
	mov edi, [ebp+8]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
	add edi, 4
INPUT_LABEL_1:
	add edi, 4
	pushad
	push edi
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle INPUT_LABEL_1
	popad
	mov edx, [ebp+8]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+8]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_1:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	cmp ebx, 0
	mov eax, dword single_false
	mov edx, single_true
	cmove ebx, eax
	cmovne ebx, edx
	push ebx
	call printf
	pop ebx
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_1
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
exit_main:  call exit


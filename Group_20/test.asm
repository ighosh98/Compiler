extern malloc
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
	sub esp,16  ;allocating space on the stack
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
	push ecx    ;save ecx before loop start
	mov ecx,0
	mov [ebp+12],ecx   ;mov first index into loop var
FOR_LOOP_1:
	mov edx, [ebp+12]
	mov esi, [ebp+12]  ;place value of index var
	mov edi,[ebp+8]   ;edi has base address of array
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+4*esi+2*4],edx    ;first 2 bytes store the bounds
	push edx
	mov edx,10
	mov ecx, [ebp+12]
	add ecx,1
	mov [ebp+12],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
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


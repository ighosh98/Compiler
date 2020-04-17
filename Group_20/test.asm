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
	sub esp,20  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	mov edx, 0
	mov [ebp+0], edx   ;assign value to a variable
	mov edx, 1
	mov [ebp+4], edx   ;assign value to a variable
	pushad
	push dword 48
	call malloc
	mov [ebp+8],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+8]	;base pointer to the array
	mov [edi], dword 1
	mov [edi+4], dword 10
	popad
	pushad
	mov eax,[ebp+4]	    ;value of index 2
	sub eax,[ebp+0]	    ;subtract value of index1
	add eax, 1
	push edx
	mov edx,4
	imul edx ;eax has the result of 4*eax the multiplication
	pop edx
	add eax, 8   ;space for index values
	push eax
	call malloc
	mov [ebp+12],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+12]	;base pointer to the array
	mov eax, [ebp+0]
	mov [edi], eax
	mov eax, [ebp+4]
	mov [edi+4], eax
	popad
	push ecx    ;save ecx before loop start
	mov ecx,1
	mov [ebp+16],ecx   ;mov first index into loop var
FOR_LOOP_1:
	mov edx, [ebp+16]
	mov esi, [ebp+16]  ;place value of index var
	mov edi,[ebp+8]   ;edi has base address of array
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+4*esi+2*4],edx    ;first 2 bytes store the bounds
	push edx
	mov edx,10
	mov ecx, [ebp+16]
	add ecx,1
	mov [ebp+16],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+8]
	mov [ebp+12],edx   ;assign pointer of the array
	push ecx    ;save ecx before loop start
	mov ecx,1
	mov [ebp+16],ecx   ;mov first index into loop var
FOR_LOOP_2:
	mov edi, [ebp+12]
	mov esi, [ebp+16]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,10
	mov ecx, [ebp+16]
	add ecx,1
	mov [ebp+16],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_2
	pop ecx	;restore ecx after the loop
exit_main:  call exit


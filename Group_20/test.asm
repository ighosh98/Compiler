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
	sub esp,48  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	mov edx, 0
	mov [ebp+12], edx   ;assign value to a variable
	mov edx, 4
	mov [ebp+16], edx   ;assign value to a variable
	mov edx, 0
	mov [ebp+28], edx   ;assign value to a variable
	mov edx, 0
	mov [ebp+32], edx   ;assign value to a variable
	mov edx, 123
	mov [ebp+0], edx   ;assign value to a variable
	pushad
	push dword 28
	call malloc
	mov [ebp+44],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+44]	;base pointer to the array
	mov [edi], dword 4
	mov [edi+4], dword 8
	popad
	push ecx    ;save ecx before loop start
	mov ecx,[ebp+16]
	mov [ebp+8],ecx   ;mov first index into loop var
FOR_LOOP_5:
	mov edx, [ebp+8]
	push edx 
	mov edx, 2
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	mov esi, [ebp+8]  ;place value of index var
	mov edi,[ebp+44]   ;edi has base address of array
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+4*esi+2*4],edx    ;first 2 bytes store the bounds
	mov edi, [ebp+44]
	mov esi, [ebp+8]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push ecx    ;save ecx before loop start
	mov ecx,3
	mov [ebp+36],ecx   ;mov first index into loop var
FOR_LOOP_6:
	mov edx, 2
	push edx 
	mov edx, [ebp+36]
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	mov [ebp+40], edx   ;assign value to a variable
	mov edx, [ebp+40]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push ecx    ;save ecx before loop start
	mov ecx,0
	mov [ebp+12],ecx   ;mov first index into loop var
FOR_LOOP_7:
	mov edx, [ebp+12]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,[ebp+16]
	mov ecx, [ebp+12]
	add ecx,1
	mov [ebp+12],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_7
	pop ecx	;restore ecx after the loop
	push edx
	mov edx,5
	mov ecx, [ebp+36]
	add ecx,1
	mov [ebp+36],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_6
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+16]
	mov [ebp+20], edx   ;assign value to a variable
WHILE_LABEL_3:
	mov edx, [ebp+20]
	push edx 
	mov edx, [ebp+8]
	pop eax
	cmp eax,edx
	mov edx,0   ;assign false initially in comparing
	mov eax, 1   ;cmov requires register, therefore place true in eax
	cmovle  edx , eax	;place true if condition met
	cmp edx, 0
	je EXIT_WHILE_3
	mov edx, [ebp+20]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+16]
	mov [ebp+28], edx   ;assign value to a variable
WHILE_LABEL_4:
	mov edx, [ebp+28]
	push edx 
	mov edx, 10
	pop eax
	cmp eax,edx
	mov edx,0   ;assign false initially in comparing
	mov eax, 1   ;cmov requires register, therefore place true in eax
	cmovl  edx , eax	;place true if condition met
	cmp edx, 0
	je EXIT_WHILE_4
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, [ebp+28]
	push edx 
	mov edx, 1
	pop eax
	add edx, eax
	mov [ebp+28], edx   ;assign value to a variable
	jmp WHILE_LABEL_4
EXIT_WHILE_4:
	push ecx    ;save ecx before loop start
	mov ecx,4
	mov [ebp+32],ecx   ;mov first index into loop var
FOR_LOOP_8:
	mov edx, [ebp+32]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,6
	mov ecx, [ebp+32]
	add ecx,1
	mov [ebp+32],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_8
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+20]
	push edx 
	mov edx, 1
	pop eax
	add edx, eax
	mov [ebp+20], edx   ;assign value to a variable
	jmp WHILE_LABEL_3
EXIT_WHILE_3:
	push edx
	mov edx,8
	mov ecx, [ebp+8]
	add ecx,1
	mov [ebp+8],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_5
	pop ecx	;restore ecx after the loop
exit_main:  call exit

	BOUND_ERROR:
	pushad
	push dword bound_error_str
	call printf
	pop eax
	popad
	call exit

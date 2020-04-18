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
	add eax, 32
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	pushad
	mov eax,[ebp+32]	    ;value of index 2
	sub eax,[ebp+28]	    ;subtract value of index1
	add eax, 1
	push edx
	mov edx,4
	imul edx ;eax has the result of 4*eax the multiplication
	pop edx
	add eax, 8   ;space for index values
	push eax
	call malloc
	mov [ebp+36],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+36]	;base pointer to the array
	mov eax, [ebp+28]
	mov [edi], eax
	mov eax, [ebp+32]
	mov [edi+4], eax
	popad
	pushad
	mov eax,[ebp+32]	    ;value of index 2
	sub eax,[ebp+28]	    ;subtract value of index1
	add eax, 1
	push edx
	mov edx,4
	imul edx ;eax has the result of 4*eax the multiplication
	pop edx
	add eax, 8   ;space for index values
	push eax
	call malloc
	mov [ebp+40],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+40]	;base pointer to the array
	mov eax, [ebp+28]
	mov [edi], eax
	mov eax, [ebp+32]
	mov [edi+4], eax
	popad
	pushad
	mov eax,[ebp+32]	    ;value of index 2
	sub eax,[ebp+28]	    ;subtract value of index1
	add eax, 1
	push edx
	mov edx,4
	imul edx ;eax has the result of 4*eax the multiplication
	pop edx
	add eax, 8   ;space for index values
	push eax
	call malloc
	mov [ebp+44],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+44]	;base pointer to the array
	mov eax, [ebp+28]
	mov [edi], eax
	mov eax, [ebp+32]
	mov [edi+4], eax
	popad
	pushad
	mov edi, [ebp+36]
	push dword [edi+4]
	push dword [edi]
	mov eax, [edi+4]
	sub eax, [edi]
	add eax, 1
	push eax
	push dword input_arr_int
	call printf
	add esp, 16
	popad
	pushad
	mov edi, [ebp+36]
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
	pushad
	mov edi, [ebp+40]
	push dword [edi+4]
	push dword [edi]
	mov eax, [edi+4]
	sub eax, [edi]
	add eax, 1
	push eax
	push dword input_arr_int
	call printf
	add esp, 16
	popad
	pushad
	mov edi, [ebp+40]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
	add edi, 4
INPUT_LABEL_2:
	add edi, 4
	pushad
	push edi
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle INPUT_LABEL_2
	popad
	mov edx, 7
	mov [ebp+8], edx   ;assign value to a variable
	push ecx    ;save ecx before loop start
	mov ecx,1
	mov [ebp+12],ecx   ;mov first index into loop var
FOR_LOOP_1:
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
	mov edi, [ebp+36]
	mov esi, [ebp+0]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx 
	mov edi, [ebp+40]
	mov esi, [ebp+4]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+16], edx   ;assign value to a variable
	mov edi, [ebp+36]
	mov esi, [ebp+8]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx 
	mov edi, [ebp+40]
	mov esi, [ebp+4]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+20], edx   ;assign value to a variable
	mov edi, [ebp+36]
	mov esi, [ebp+0]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx 
	mov edi, [ebp+40]
	mov esi, [ebp+8]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+24], edx   ;assign value to a variable
	mov edx, [ebp+16]
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
	push edx
	mov edx,3
	mov ecx, [ebp+12]
	add ecx,1
	mov [ebp+12],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
exit_main:  call exit

	BOUND_ERROR:
	pushad
	push dword bound_error_str
	call printf
	pop eax
	popad
	call exit

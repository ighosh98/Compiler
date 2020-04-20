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

array_parameter_demo:
	pushad
	mov edx, [ebp+0]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+0]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_1:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
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
	mov edx, [ebp+4]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+4]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_2:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_2
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	pushad
	push dword input_str_int
	call printf
	pop eax
	popad
	pushad
	mov eax, ebp
	add eax, 36
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
	add eax, 40
	push eax
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	pushad
	push dword 28
	call malloc
	mov [ebp+44],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+44]	;base pointer to the array
	mov [edi], dword 10
	mov [edi+4], dword 14
	popad
	pushad
	push dword 28
	call malloc
	mov [ebp+48],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+48]	;base pointer to the array
	mov [edi], dword 10
	mov [edi+4], dword 14
	popad
	pushad
	mov eax,[ebp+40]	    ;value of index 2
	sub eax,[ebp+36]	    ;subtract value of index1
	add eax, 1
	push edx
	mov edx,4
	imul edx ;eax has the result of 4*eax the multiplication
	pop edx
	add eax, 8   ;space for index values
	push eax
	call malloc
	mov [ebp+52],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+52]	;base pointer to the array
	mov eax, [ebp+36]
	mov [edi], eax
	mov eax, [ebp+40]
	mov [edi+4], eax
	popad
	pushad
	mov edi, [ebp+44]
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
	mov edi, [ebp+44]
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
	mov edi, [ebp+52]
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
	mov edi, [ebp+52]
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
	mov edx, [ebp+44]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+44]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_3:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_3
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	mov edx, 0
	mov [ebp+8], edx   ;assign value to a variable
	push ecx    ;save ecx before loop start
	mov ecx,10
	mov [ebp+28],ecx   ;mov first index into loop var
FOR_LOOP_1:
	mov edi, [ebp+44]
	mov esi, [ebp+28]
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
	mov edx, [ebp+8]
	push edx 
	mov edi, [ebp+44]
	mov esi, [ebp+28]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+8], edx   ;assign value to a variable
	mov edx, [ebp+8]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,14
	mov ecx, [ebp+28]
	add ecx,1
	mov [ebp+28],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+44]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+44]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_4:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_4
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	mov edx, [ebp+8]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edi, [ebp+52]
	mov esi, 6
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx 
	mov edi, [ebp+52]
	mov esi, 7
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov edi,[ebp+52]   ;edi has base address of array
	mov esi,5
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+4*esi+2*4],edx	;first 2 bytes store the bounds
	mov edx, [ebp+52]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+52]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_5:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_5
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	mov edx, 0
	mov [ebp+12], edx   ;assign value to a variable
	mov edx, [ebp+0]
	mov [ebp+48],edx   ;assign pointer of the array
	mov edx, [ebp+48]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+48]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_6:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_6
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	push ecx    ;save ecx before loop start
	mov ecx,10
	mov [ebp+32],ecx   ;mov first index into loop var
FOR_LOOP_2:
	mov edi, [ebp+48]
	mov esi, [ebp+32]
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
	mov edx, [ebp+12]
	push edx 
	mov edi, [ebp+48]
	mov esi, [ebp+32]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+12], edx   ;assign value to a variable
	mov edx, [ebp+12]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,14
	mov ecx, [ebp+32]
	add ecx,1
	mov [ebp+32],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_2
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+48]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+48]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_7:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_7
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	mov edx, [ebp+12]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	mov edx, 0
	mov [ebp+16], edx   ;assign value to a variable
	mov edx, [ebp+4]
	pushad
	push dword output_str
	call printf
	pop eax
	popad
	pushad
	mov edi, [ebp+4]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
OUTPUT_LABEL_8:
	mov ebx, [edi+ecx*4+2*4]
	pushad
	push ebx
	push dword single_int
	call printf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle OUTPUT_LABEL_8
	popad
	pushad
	push dword nextline
	call printf
	pop eax
	popad
	push ecx    ;save ecx before loop start
	mov ecx,1
	mov [ebp+20],ecx   ;mov first index into loop var
FOR_LOOP_3:
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
	mov edx, [ebp+16]
	push edx 
	mov edi, [ebp+4]
	mov esi, [ebp+24]
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+16], edx   ;assign value to a variable
	mov edx, [ebp+16]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,4
	mov ecx, [ebp+20]
	add ecx,1
	mov [ebp+20],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_3
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+16]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	popad
ret
main:
	sub esp,28  ;allocating space on the stack
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
	pushad
	push dword 28
	call malloc
	mov [ebp+20],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+20]	;base pointer to the array
	mov [edi], dword 10
	mov [edi+4], dword 14
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
	mov [ebp+24],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+24]	;base pointer to the array
	mov eax, [ebp+0]
	mov [edi], eax
	mov eax, [ebp+4]
	mov [edi+4], eax
	popad
	pushad
	mov edi, [ebp+20]
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
	mov edi, [ebp+20]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
	add edi, 4
INPUT_LABEL_3:
	add edi, 4
	pushad
	push edi
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle INPUT_LABEL_3
	popad
	pushad
	mov edi, [ebp+24]
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
	mov edi, [ebp+24]
	mov ebx, [edi]
	mov eax, [edi+4]
	sub eax, ebx
	mov ecx, 0
	add edi, 4
INPUT_LABEL_4:
	add edi, 4
	pushad
	push edi
	push dword input_format_int
	call scanf
	add esp, 8
	popad
	add ecx,1
	cmp ecx,eax
	jle INPUT_LABEL_4
	popad
	sub esp, 56
	mov edx, [ebp+20]
	mov  [esp+0], edx
	mov edx, [ebp+24]
	mov  [esp+4], edx
	mov edx, [ebp+8]
	mov  [esp+8], edx
	mov edx, [ebp+12]
	mov  [esp+12], edx
	mov edx, [ebp+16]
	mov  [esp+16], edx
	push ebp
	mov ebp, esp
	add ebp, 4
	call array_parameter_demo
	pop ebp
	mov edx, [esp+8]
	mov  [ebp+8], edx
	mov edx, [esp+12]
	mov  [ebp+12], edx
	mov edx, [esp+16]
	mov  [ebp+16], edx
	add esp, 56
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
	mov edx, [ebp+16]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
exit_main:  call exit

BOUND_ERROR:
	pushad
	push dword bound_error_str
	call printf
	pop eax
	popad
	call exit

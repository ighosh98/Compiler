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

arraysum:
	pushad
	mov edx, 0.0
	mov [ebp+12], edx   ;assign value to a variable
	push ecx    ;save ecx before loop start
	mov ecx,4
	mov [ebp+20],ecx   ;mov first index into loop var
FOR_LOOP_1:
	mov edx, [ebp+12]
	push edx 
	pop eax
	add edx, eax
	mov [ebp+12], edx   ;assign value to a variable
	push edx
	mov edx,20
	mov ecx, [ebp+20]
	add ecx,1
	mov [ebp+20],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+12]
	mov [ebp+4], edx   ;assign value to a variable
	popad
ret
main:
	sub esp,12  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	pushad
	push dword 36
	call malloc
	mov [ebp+8],eax	;store the allocated memory pointer
	pop eax
	mov edi,[ebp+8]	;base pointer to the array
	mov [edi], dword 4
	mov [edi+4], dword 10
	popad
	push ecx    ;save ecx before loop start
	mov ecx,6
	mov [ebp+4],ecx   ;mov first index into loop var
FOR_LOOP_2:
	mov edx, [ebp+0]
	push edx 
	mov edx, [ebp+4]
	pop eax
	sub eax, edx    ;perform (eax - edx) subtraction
	mov edx, eax    ;store result of subtraction in edx
	push edx 
	mov edx, [ebp+0]
	push edx 
	mov edx, [ebp+4]
	pop eax
	sub eax, edx    ;perform (eax - edx) subtraction
	mov edx, eax    ;store result of subtraction in edx
	pop eax
	imul edx	;eax*edx stored in edx:eax 
	mov edx, eax    ;truncated result moved to edx 
	mov esi, [ebp+4]  ;place value of index var
	mov edi,[ebp+8]   ;edi has base address of array
	cmp esi,[edi]
	jl BOUND_ERROR
	cmp esi,[edi+4]
	jg BOUND_ERROR
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+4*esi+2*4],edx    ;first 2 bytes store the bounds
	mov edi, [ebp+8]
	mov esi, [ebp+4]
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
	push edx
	mov edx,10
	mov ecx, [ebp+4]
	add ecx,1
	mov [ebp+4],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_2
	pop ecx	;restore ecx after the loop
	sub esp, 24
	mov edx, [ebp+8]
	mov  [esp+0], edx
	mov edx, [ebp+0]
	mov  [esp+4], edx
	push ebp
	mov ebp, esp
	add ebp, 4
	call arraysum
	pop ebp
	mov edx, [esp+4]
	mov  [ebp+0], edx
	add esp, 24
	mov edx, [ebp+0]
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

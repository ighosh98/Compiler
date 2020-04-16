extern printf
extern scanf
extern exit

SECTION .data
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

arraySum proc
	push dword true_output
	call printf
	pop eax
	push dword false_output
	call printf
	pop eax
	mov edx, 123
	push edx
	push dword integer_output
	call printf
	add esp, 8
	mov edx, [ebp+1000]
	push edx
	push dword integer_output
	call printf
	add esp, 8
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, 5
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx
	push dword integer_output
	call printf
	add esp, 8
	mov edx, [ebp+1004]
	push dword output_str
	call printf
	pop eax
	cmp edx,0
	cmove eax, single_false
	cmovne eax, single_true
	push eax
	call printf
	pop eax
	push dword nextline
	call printf
	pop eax
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+72]
	mov esi, 2
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push dword output_str
	call printf
	pop eax
	cmp edx,0
	cmove eax, single_false
	cmovne eax, single_true
	push eax
	call printf
	pop eax
	push dword nextline
	call printf
	pop eax
	mov edx, 0.0
	mov [ebp+64], edx   ;assign value to a variable
	mov edx, 1   ; assigning true
	push edx
	mov edx, 0   ; assigning false
	push edx 
	mov edx, 1   ; assigning true
	pop eax
	or edx, eax
	pop eax
	and edx, eax
	mov [ebp+1004], edx   ;assign value to a variable
	push ecx    ;save ecx before loop start
	mov ecx,1
FOR_LOOP_1:
	mov edx, [ebp+1000]
	push edx 
	mov edx, [ebp+1008]
	pop eax
	cmp eax,edx
	mov edx,0	;assign false initially in comparing
	cmovle  edx , 1	;place true if condition met
	mov [ebp+1004], edx   ;assign value to a variable
	mov edx, [ebp+1004]
	cmp edx,1
	je switch_1_true
	mov edx, [ebp+1004]
	cmp edx,0
	je switch_1_false
switch_1_true:
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, [ebp+1000]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	push edx
	push dword integer_output
	call printf
	add esp, 8
	mov edx, [ebp+64]
	push edx 
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, [ebp+1000]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+64], edx   ;assign value to a variable
	jmp switch_exit_1
switch_1_false:
	mov edx, [ebp+64]
OUTPUT FLOAT NUMBER
	mov edx, [ebp+1000]
	push edx
	push dword integer_output
	call printf
	add esp, 8
	jmp switch_exit_1
	switch_exit_1:
	push edx
	mov edx,15
	add ecx, 1
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+64]
OUTPUT FLOAT NUMBER
ret
arraySum endp

main:
	sub esp,136  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+4]
	mov esi, 5
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	mov [ebp+128], edx   ;assign value to a variable
	Getting value of variable: k
	Calling function readArr
	    push base pointer
	    push input and output variables
	    assign base pointer as stack pointer
	    increase stack pointer size by new function size
	Calling function arraySum
	    push base pointer
	    push input and output variables
	    assign base pointer as stack pointer
	    increase stack pointer size by new function size
	mov edx, [ebp+128]
OUTPUT FLOAT NUMBER
exit_main:  call exit

readArr proc
WHILE_LABEL_1:
	mov edx, [ebp+132]
	push edx 
	mov edx, [ebp+120]
	pop eax
	cmp eax,edx
	mov edx,0	;assign false initially in comparing
	cmovle  edx , 1	;place true if condition met
	cmp edx, 0
	je EXIT_WHILE_1
	Getting value of variable: tempvar
	mov edx, [ebp+124]
	mov esi, [ebp+132]  ;place value of index var
	CHECK ARRAY BOUNDS
	mov edi,[ebp+0]   ;edi has base address of array
	sub esi, [edi]  ;subtract base index of the array
	mov [edi+2*esi+2*4],edx    ;first 2 bytes store the bounds
	mov edx, [ebp+132]
	push edx 
	mov edx, 1
	pop eax
	add edx, eax
	mov [ebp+132], edx   ;assign value to a variable
	jmp WHILE LABEL_1
EXIT_WHILE_1:
ret
readArr endp


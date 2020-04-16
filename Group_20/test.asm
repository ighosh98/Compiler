bits 32
global main
extern printf
extern scanf

arraySum proc
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, [ebp+124]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	mov [ebp+124], edx   ;assign value to a variable
	mov edx, 0.0
	mov [ebp+124], edx   ;assign value to a variable
	mov edx, 1   ; assigning true
	push edx
	mov edx, 0   ; assigning false
	push edx 
	mov edx, 1   ; assigning true
	pop eax
	or edx, eax
	pop eax
	and edx, eax
	mov [ebp+136], edx   ;assign value to a variable
	push ecx    ;save ecx before loop start
	mov ecx,1
FOR_LOOP_1:
	mov edx, [ebp+132]
	push edx 
	mov edx, [ebp+140]
	pop eax
	cmp eax,edx
	mov edx,0	;assign false initially in comparing
	cmovle  edx , 1	;place true if condition met
	mov [ebp+136], edx   ;assign value to a variable
	mov edx, [ebp+136]
	cmp edx,1
	je switch_1_true
	mov edx, [ebp+136]
	cmp edx,0
	je switch_1_false
switch_1_true:
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, [ebp+132]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	Printing variable: list
	mov edx, [ebp+124]
	push edx 
	VERIFY THAT INDEX IS WITHIN BOUNDS
	mov edi, [ebp+0]
	mov esi, [ebp+132]
	sub esi, [edi]	;subtract the base index
	mov edx, [edi+esi*4+2*4]
	pop eax
	add edx, eax
	mov [ebp+124], edx   ;assign value to a variable
	jmp switch_exit_1
switch_1_false:
	mov edx, [ebp+124]
	Printing variable: sum
	mov edx, [ebp+132]
	Printing variable: index
	jmp switch_exit_1
	switch_exit_1:
	push edx
	mov edx,15
	add ecx, 1
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
	mov edx, [ebp+124]
	Printing variable: sum
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
	Printing variable: s
exit_main:

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


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

main:
	sub esp,4  ;allocating space on the stack
	mov ebp, esp	;ebp accesses upwards, while stack grows downwards
	push ecx    ;save ecx before loop start
	mov ecx,1
	mov [ebp+0],ecx   ;mov first index into loop var
FOR_LOOP_1:
	mov edx, [ebp+0]
	push edx 
	mov edx, 1
	pop eax
	add edx, eax
	mov [ebp+0], edx   ;assign value to a variable
	mov edx, [ebp+0]
	pushad
	push edx
	push dword integer_output
	call printf
	add esp, 8
	popad
	push edx
	mov edx,9
	mov ecx, [ebp+0]
	add ecx,1
	mov [ebp+0],ecx	;add 1 to loop variable
	cmp ecx,edx
	pop edx
	jle FOR_LOOP_1
	pop ecx	;restore ecx after the loop
exit_main:  call exit


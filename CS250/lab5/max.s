.data 
prompt1: .asciz "Enter the 1st number: " 
p1Len = . - prompt1
prompt2: .asciz "Enter the 2nd number: " 
p2Len = . - prompt2
answer: .asciz "The largest number is "
aLen = . - answer
num1: 0
num2: 0

.globl main 

main: 
mov r0, $1		@print prompt1
ldr r1, =prompt1
ldr r2, =p1Len
mov r7, $4
svc $0

mov r7, $3		@ read syscall
mov r0, $1		
ldr r1, =num1
mov r2, $0x30
svc $0

mov r0, $1		@print prompt2
ldr r1, =prompt2
ldr r2, =p2Len
mov r7, $4
svc $0

mov r7, $3		@ read syscall
mov r0, $1		
ldr r1, =num2
mov r2, $0x30
svc $0

bge num1, num2, Print	@compare and put the larger in num1 
move num2, num1 

Print: 
mov r0, $1	@print num1
ldr r1, =num1
ldr r2, $0x30
mov r7, $4
svc $0

mov r7, $1	@ exit syscall
svc $0		@ wake kernel
.end


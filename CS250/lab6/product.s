.data
scan_format: .asciz "%d"
print_format: .asciz "%d\n"
str1: .asciz "***Enter the numbers here:***\n"
str2: .asciz "***The product is:***\n"
n: .word 0

.text
.global main

main:
push {lr}
mov r4, #1	@assign 1 to r4
ldr r0, =str1	@print opening
bl printf

loop:
ldr r0, =scan_format	@read a number to n 
ldr r1, =n
bl scanf

ldr r3, =n	@load n to r3
ldr r3, [r3]
cmp r3, #-1	@if number is -1, print
beq print
mul r4, r4, r3	@product
bl loop

print:
ldr r0, =str2   @print ending
bl printf

ldr r0, =print_format	@print product
mov r1, r4
bl printf

pop {pc}

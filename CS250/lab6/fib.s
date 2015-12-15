.data
scan_format: .asciz "%d"
print_format: .asciz "%d\n"
n: .word 0

.text
.global main

main:
push {lr}	
mov r4, #0	@assign 0 to r4, r4 is previous term
mov r5, #1	@assign 1 to r5, r5 is this term

ldr r0, =scan_format	@read  n 
ldr r1, =n
bl scanf

ldr r3, =n	@load n to r3
ldr r3, [r3]

loop:
cmp r3, #1	@if count down to 1, print
beq print
sub r3, r3, #1	@count down
add r5, r5, r4	@r5 move ahead
sub r4, r5, r4	@r4 is prev(r5)
bl loop

print:
ldr r0, =print_format	@print nth term
mov r1, r5 
bl printf

pop {pc}

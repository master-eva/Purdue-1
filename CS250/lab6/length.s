.data
scan_format: .asciz "%s"
print_format: .asciz "%d\n"
str: .asciz ""

.text
.global main

main:
push {lr}
ldr r0, =scan_format	@scan a string
ldr r1, =str
bl scanf
mov r3, #0	@init r3 to 0
ldr r1, =str	@load str to r1

loop:
add r3, r3, #1	@count
ldr r2, [r1], #1	@load nth char in r1 to r2
cmp r2, #0	@if r2 is '/0', meet end, quit
bne loop

print:
sub r3, r3, #1	@count become larger 1 because counted '/0'
ldr r0, =print_format	@print
mov r1, r3
bl printf

pop {pc}

.global bubble 

bubble:
push {lr}
@r0 = array pointer
@r1 = total number of array
@r4 = number of times of outter loop
@when r4 equals r1, the whole program end
@r5 = number of inner loop
@compare r5th and r5+1th item in the array
@if not good, swap

@init r4 to 0
mov r4, #0

loop1:
add r4, r4, #1	@give r4 the number of loops now
cmp r4, r1	@if r4 equals length, quit
beq exit
mov r5, #0	@reset r5
sub r3, r1, r4	@r3 = length -r4

loop2:
cmp r5, r3	@if r5 = r3, this inner loop finished
beq loop1
mov r10, #4	@r10 = 4, temp
mul r6, r5, r10	@r6 is array point of r5th item in array
add r6, r6, r0
ldr r8, [r6]	@r8 is the number in r6
add r7, r5, #1	@r7 is the next item of r6
mul r7, r7, r10
add r7, r7, r0
ldr r9, [r7]	@r9 is number in r7
cmp r8, r9	@compare r8 and r9, if r8 is great than r9, swap
bgt swap

nonswap:	@if not swap
add r5, r5, #1
b loop2

swap:		@if swap
str r9, [r6]
str r8, [r7]
add r5, r5, #1
b loop2

exit:
pop {lr}
bx lr

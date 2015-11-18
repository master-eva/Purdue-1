.global quicksort

partition:
push {r0-r6, r8-r10, lr}
sub r2, r1, #1 	@pivot = a[n-1], r2 = n-1
mov r12, #4
mul r3, r2, r12
add r3, r0, r3	@r3 = array point on n-1 
ldr r4, [r3]	@r4 = a[n-1]

mov r5, #0	@i = 0
mov r6, r0	@r6 = array pointer of a[i]
mov r7, r5      @j = i

loop:
cmp r7, r2
bgt exit
beq exit
mul r8, r7, r12
add r8, r8, r0	@r8 = array pointer of a[j]
ldr r9, [r8] 	@r9 = content of a[j]
cmp r9, r4
blt swap
add r7, r7, #1
beq loop
bgt loop
swap:
ldr r10, [r6]
str r9, [r6]
str r10, [r8]
add r5, r5, #1	@update i, also update array pointer
mul r6, r5, r12
add r6, r0, r6
add r7, r7, #1
b loop
exit:
ldr r10, [r6]
str r10, [r3]
str r4, [r6]
mov r7, r5
pop {r0-r6, r8-r10, lr}
bx lr

quicksort:
push {r0-r9, lr}
cmp r1, #2
blt exitqs
bl partition
mov r9, r1
mov r1, r7
push {r0, r1, r9}
bl quicksort
pop {r0, r1, r9}
mov r12, #4
mul r8, r1, r12
add r8, r8, #4
add r8, r0, r8
mov r0, r8
sub r9, r9, r1
sub r9, r9, #1
mov r1, r9
push {r0, r1}
bl quicksort
pop {r0, r1}
exitqs:
pop {r0-r9, lr}
bx lr

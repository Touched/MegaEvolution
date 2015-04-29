@@action hook 0x01D040 2

.thumb
.align 2

move_hook:
	ldr r2, ram_loc
	ldrb r3, [r2]
	
	cmp r3, #0x22
	bne do_once

return:
	ldrb r0, [r0]
	lsl r0, #2
	add r0, r1
	ldr r0, [r0]
	bl call_via_r0
	pop {pc}
	
call_via_r0:
	bx r0
	
call_via_r3:
	bx r3
	
do_once:
	mov r3, #0x22
	strb r3, [r2]
	
	push {r0-r3}
	
	ldr r0, =0x02023BC4
	mov r1, #0
	strb r1, [r0]
	
	ldr r0, =(0x02023BE4)
	bl can_mega_evolve
	cmp r0, #0
	beq exit
	
	@ We need a pointer to the returned value
	push {r0}
	
	@ Call battle command 0x20 - it's a NOP
	
	@ We need to write 4 bytes from that pointer (the evolution struct)
	mov r1, #4
	mov r0, #0
	mov r2, sp
	ldr r3, =(0x0800E800 + 1)
	bl call_via_r3
	
	@ Fix stack
	pop {r0}
	
	@ Mark buffer for execution
	mov r0, #0
	ldr r3, =(0x08017248 + 1)
	bl call_via_r3
	
exit:
	pop {r0-r3}
	
	b return
	
.align 2
ram_loc: .word 0x020370B8

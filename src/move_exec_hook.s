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
	bl handle_mega_evolution
	pop {r0-r3}
	
	b return
	
.align 2
ram_loc: .word 0x020370B8

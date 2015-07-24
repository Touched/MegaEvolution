	@ hook at 080483A4

	.thumb
	.align 2

	level_string_hook:
	lsl r4, #0x18
	lsr r4, #0x18
	
	@ Check if is mega
	ldr r0, battle_data
	
	@ Get owner of the health bar
	ldr r2, objects
	lsl r1, r5, #4
	add r1, r5
	lsl r1, r1, #2
	add r1, r2
	ldrh r1, [r1, #0x3A]
	
	mov r2, #0x58
	mul r1, r2
	add r0, r1 
	bl is_mega
	cmp r1, #0
	bne load_special
	
	ldr r1, level_string
	mov r0, sp
	mov r2, #0x10
	bl memcpy
	mov r0, sp
	add r0, #2
	
	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str
	
	mov r1, r0
	sub r1, #2
	b return
	
	load_special:
	adr r1, special_string
	mov r0, sp
	mov r2, #0x10
	bl memcpy
	mov r0, sp
	add r0, #0
	
	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str
	
	mov r1, r0
	sub r1, #2
	
	@
	mov r0, sp
	sub r1, r0
	mov r0, #3
	sub r0, r1
	lsl r1, r0, #2
	add r1, r1, r0
	sub r1, #2
	add r3, sp, #0x10
	mov r0, sp
	mov r2, #3
	ldr r6, =(0x080483D6 + 1)
	bx r6
	
	return:	
	ldr r6, =(0x080483C4 + 1)
	bx r6
	
	int_to_str:
	ldr r6, =(0x08008E78 + 1)
	bx r6
	
	memcpy:
	ldr r3, =(0x081E5E78 + 1)
	bx r3
	
	.align 2
	level_string: .word 0x0826051C
	battle_data: .word 0x02023BE4
	b_current_bank: .word 0x02023D6F
	objects: .word 0x0202063C
	special_string:
	.byte 0xFF
	.rept 16
	.byte 0
	.endr

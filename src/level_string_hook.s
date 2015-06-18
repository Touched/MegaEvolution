@ hook at 080483A4

.thumb
.align 2

level_string_hook:
	lsl r4, #0x18
	lsr r4, #0x18
	
	@ Check if is mega
	ldr r0, battle_data
	ldr r1, b_current_bank
	ldrb r1, [r1]
	mov r2, #0x58
	mul r1, r2
	add r0, r1
	bl is_mega
	cmp r1, #0
	bne load_special
	ldr r1, level_string
	b return
	
load_special:
	adr r1, special_string
	
return:
	mov r0, sp
	mov r2, #0x10
	ldr r3, =(0x080483AE + 1)
	bx r3
	
.align 2
level_string: .word 0x0826051C
battle_data: .word 0x02023BE4
b_current_bank: .word 0x02023D6F
special_string:
	.byte 0x5
	.byte 0x5
	.byte 0xFF
	.rept 16
	.byte 0
	.endr

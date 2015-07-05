	@ hook at 0802EC10

	.thumb
	.align 2

	move_button_hook:
	mov r7, #2 @ B Button
	mov r0, r7
	and r0, r1
	cmp r0, #0
	beq check_start
	
	@ Handle B button
	ldr r0, =(0x0802EC1A + 1)
	bx r0
	
	check_start:
	mov r0, #8 @ Start
	and r0, r1
	cmp r0, #0
	beq check_left
	
	@ Hook code here
	bl trigger_mega_evolution
	
	@ Return
	ldr r0, =(0x0802EDBA + 1)
	bx r0
	
	check_left:
	ldr r0, =(0x0802EC38 + 1)
	bx r0

@ 0802E438 in r0
battle_menu_hook:
	push {r4-r7, lr}
	mov r7, r8
	push {r7}

	bl mega_reset_player_triggers
	
	ldr r7, =(0x02022BC4)
	ldr r4, =(0x0802E440 + 1)
	bx r4
	

	@ hook at 080159DC
	.thumb
	.align 2

	exit_battle_hook:
	bl revert_megas

	ldr r1, =(0x03004F84)
	ldr r0, =(0x08015A30 + 1)
	str r0, [r1]
	ldr r1, =(0x0300537C)
	
	ldr r0, =(0x080159E4 + 1)
	bx r0

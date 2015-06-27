	.thumb

	@ at 08038668 in r0
	.align 2
	ai_move_hook:
	bl ai_trigger_mega_evolution

	ldr r0, =(0x0803870E + 1)
	bx r0

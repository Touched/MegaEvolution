	.align 2
	.thumb

	@ hook at 0803495A
	load_graphics_hook:
	push {r0}
	mov r0, r5
	bl healthbar_load_graphics
	pop {r0}

	pop {r4-r6}
	pop {r1}
	bx r1

	.align 2

	@ hook at 08047FD8
	show_graphics_hook:
	bl healthbar_display_graphics

	mov r10, r5
	pop {r4-r7}
	pop {r1}
	bx r1

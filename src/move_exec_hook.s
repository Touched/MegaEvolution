	.thumb

	@ 0801DB00 in r1
	.align 2
	attack_canceller_hook:
	bl handle_mega_evolution
	mov r8, r3
	mov r9, r4
	pop {r4-r7}
	pop {r0}
	bx r0


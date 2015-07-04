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

@ 080154A0 in r0
.align 2
bc_pre_attacks_hook:
	push {r4-r7, lr}
	ldr r0, =(0x02023DD0)
	ldr r0, [r0]
	ldr r1, =0x8000

	push {r0-r3}
	bl handle_mega_evolutions
	pop {r0-r3}
		
	ldr r2, =(0x080154AA + 1)
	bx r2

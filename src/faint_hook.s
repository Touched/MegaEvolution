	@ Hook here 080326C4
	.thumb
	.align 2

	faint_hook:
	add r0, r2
	
	push {r0-r3}
	bl revert_mega
	pop {r0-r3}

	ldr r3, =(0x08035244 + 1)
	bl call_via_r3
	mov r1, #0x40
	
	ldr r3, =(0x080326CC + 1)
	call_via_r3:
	bx r3

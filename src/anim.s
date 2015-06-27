	@ Modded function to load an animation script from a pointer rather than a table
	@ I was surpised that this didn't exist, but it appears GF has a fetish for code
	@ duplication. 

	.thumb
	.global animation_script_start

	@ void animation_script_start(u8 *script, u8 attacker, u8 defender)
	.align 2

	@ Essential
	.thumb_func
	animation_script_start:
	push {r4, lr}

	ldr r3, =0x02037F1A
	strb r1, [r3]
	ldr r3, =0x02037F1B
	strb r2, [r3]
	
	@ Rest of the function needs this (after continue)
	mov r4, r1
	
	@ Create pointer to animation script (table with single entry)
	push {r0}
	
	@ Index zero (first entry in above created table)
	mov r1, #0
	
	@ Call function
	mov r0, sp
	mov r2, #0
	ldr r3, =(0x080725D4 + 1)
	bl call_via_r3
	
	@ Fix the stack
	pop {r0}
	
	@ Continue
	ldr r3, =(0x080341F6 + 1)
	bx r3
	
	call_via_r3:
	bx r3
	

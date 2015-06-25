.thumb

@ 0801D76C in r1
.align 2
attack_canceller_hook:
	ldrb r0, [r0]
	cmp r0, #0
	beq go
	ldr r1, =0x02023BE3 
	ldr r0, =(0x0801D774 + 1)
	bx r0
	
go:
	push {r0-r3}
	bl handle_mega_evolution
	pop {r0-r3}
	
	ldr r0, =(0x0801D784 + 1)
	bx r0

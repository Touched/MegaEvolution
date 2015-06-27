	.thumb

	@ This is probably the stupidest part of the entire project, as well as the most
	@ tedious. Whenever a Pokemon gets hit with a damaging attack, the healthbox
	@ shakes a little. It does this by creating an invisible object that has a callback
	@ that alters the healthbox. These hooks save an index to those invisible objects
	@ so that we can actually move the box a bit.

	@ at 0804BE80 in r3
	.align 2
	create_shaker_hook:
	lsl r0, #0x18
	lsr r0, #0x18
	
	@ Save Index
	ldr r3, shaker_data
	strb r0, [r3, #1]
	
	@ Set boolean to true
	mov r2, #1
	strb r2, [r3]
	
	@ Return
	ldr r2, =(0x0202063C)
	lsl r1, r0, #4
	ldr r3, =(0x0804BE88 + 1)
	bx r3
	
	@ at 0804BEDC in r2
	.align 2
	objc_shaker_hook:
	mov r2, #0
	asr r0, #0x10
	cmp r0, #0x15
	bne return
	
	strh r2, [r1, #0x24]
	strh r2, [r1, #0x26]
	
	@ Set boolean to false
	ldr r1, shaker_data
	strb r2, [r1]
	
	ldr r2, =(0x0804BEE6 + 1)
	bx r2
	
	return:
	pop {r0}
	bx r0

	.align 2
	shaker_data: .word 0x02022AD0 @ 2 fre bytes

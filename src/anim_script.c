#include "types.h"
#include "animscript.h"

u8 anim_script2[] = {
	3,
	0x20 + 1,
	0x14,
	0x0F,
	0x08,
	2,
	1,
	1,
	0,
	8
};

u8 anim_script[] = {
	3,
	ANIM_LONG(0x080F1420 + 1),
	2,
	1,
	ANIM_SHORT(1),
	8
};

// From anim.s
void animation_script_start(u8 *script, u8 attacker, u8 defender);

void play_mega_evolution(u8 attacker, u8 defender) {
	animation_script_start(anim_script, attacker, defender);
}

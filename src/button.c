#include "types.h"
#include "mega.h"

void play_sound(u16 sound_id);

void trigger_mega_evolution() {
	battle_data *bdata = (battle_data *) 0x02023BE4;
	if (!can_mega_evolve(bdata)) return;

	if (megadata->trigger[0]) {
		// Turn off
		play_sound(3);
		megadata->trigger[0] = 0;
	} else {
		// Turn on
		play_sound(2);
		megadata->trigger[0] = 1;
	}
}

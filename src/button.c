#include "types.h"
#include "mega.h"

void audio_play(u16 sound_id);

void trigger_mega_evolution() {
	battle_data *bdata = (battle_data *) 0x02023BE4;
	if (!can_mega_evolve(bdata)) return;

	if (megadata->trigger[0]) {
		// Turn off
		audio_play(3);
		megadata->trigger[0] = 0;
	} else {
		// Turn on
		audio_play(2);
		megadata->trigger[0] = 1;
	}
}

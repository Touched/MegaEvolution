#include "types.h"
#include "mega.h"
#include "battle.h"

void audio_play(u16 sound_id);

void trigger_mega_evolution() {
	if (!can_mega_evolve(&bdata[*b_current_bank])) {
		return;
	}

	if (megadata->trigger[*b_current_bank]) {
		// Turn off
		audio_play(3);
		megadata->trigger[*b_current_bank] = 0;
	} else {
		// Turn on
		audio_play(2);
		megadata->trigger[*b_current_bank] = 1;
	}
}

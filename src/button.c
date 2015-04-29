#include "types.h"

void audio_play(u16 sound_id);

void trigger_mega_evolution() {
	audio_play(0xB);
}

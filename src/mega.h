#ifndef MEGA_H_
#define MEGA_H_

#include "types.h"
#include "battle.h"
#include "evo.h"

typedef struct mega_data {
  u8 trigger[4];
  u8 done[4];
  u8 running;
} mega_data;

// Some unused RAM again
mega_data *megadata = (mega_data *) 0x02022B40;

evolution *can_mega_evolve(battle_data *pokemon);
u16 is_mega(battle_data *pokemon);
u8 mega_evolution_enabled();

#endif

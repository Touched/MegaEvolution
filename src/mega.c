// Mega evolution helper functions

#include "mega.h"
#include "common.h"
#include "battle.h"
#include "pokemon.h"

// Return all the evolution data, null otherwise
evolution *can_mega_evolve(battle_data *pokemon) {
  // Cast to u32 for accurate calculation of the pointer
  evolution *evolutions = (evolution*) ((u32) evolution_table + pokemon->species * sizeof(evolution) * (*evos_per_poke + 1));
  u8 i, j;
	
  for (i = 0; i <= *evos_per_poke; ++i) {
    if (evolutions[i].type == MEGA_EVOLUTION) {
      // Ignore reverison information
      if (evolutions[i].argument == 0) {
	continue;
      }
		
      // Check for held item
      if (evolutions[i].unknown == MEGA_VARIANT_STANDARD) {
	if (evolutions[i].argument == pokemon->held_item) {
	  return &evolutions[i];
	} 
      } else if (evolutions[i].unknown == MEGA_VARIANT_WISH) {
	// Check learned moves
	for (j = 0; j < 4; ++j) {
	  if (pokemon->move_ids[j] == evolutions[i].argument) {
	    return &evolutions[i];
	  }
	}
      }
    }
  }
	
  // NULL
  return 0;
}

// TODO: Same as above but for primals

u16 is_mega(battle_data *pokemon) {
  evolution *evolutions = (evolution*) ((u32) evolution_table + pokemon->species * sizeof(evolution) * (*evos_per_poke + 1));
  u8 i;
	
  for (i = 0; i <= *evos_per_poke; ++i) {
    // Null argument means revert to that species
    // Match found
    if (evolutions[i].type == MEGA_EVOLUTION && evolutions[i].argument == 0) {
      return 1;
    }
  }
	
  return 0;
}

void build_cmdbuf_mega(u8 arg, u16 len, u8 **data);
void mark_buffer_for_execution(u8 arg);

void command();
void handle_mega_evolution() {
  u8 bank = *b_attacker;
	
  if (!megadata->trigger[bank]) return;

  if (megadata->done[bank & 1]) return;
	
  megadata->trigger[bank] = 0;
	
  battle_data *data = &bdata[bank];
  evolution *evo = can_mega_evolve(data);
	
  // Make sure we only mega evolve once. Primals are exempt
  if (evo->unknown != MEGA_VARIANT_PRIMAL) {
    megadata->done[bank & 1] = 1;
  }
	
  // Null check
  if (evo) {
    *b_active_side = bank;
    build_cmdbuf_mega(0, 4, (u8**) &evo);

    *((u32*) 0x02023BC8) |= 1 << (bank);
  }

}

void revert_mega(u8 *poke) {
  u16 species = 0;
  evolution *evo = *evolution_table;
	
  u16 current_species = pokemon_getattr(poke, 0xB, 0);
	
  evolution *evolutions = (evolution*) ((u32) evolution_table + current_species 
					* sizeof(evolution) * (*evos_per_poke + 1));
  u8 i;
	
  for (i = 0; i <= *evos_per_poke; ++i) {
    // Null argument means revert to that species
    // Match found
    if (evolutions[i].type == MEGA_EVOLUTION && evolutions[i].argument == 0) {
      species = evolutions[i].species;
    }
  }
	
  // If a match was found
  if (species) {
    pokemon_setattr(poke, 0xB, (u8*) &species);
    recalc_stats(poke);
  }
}

void reset_mega();

void revert_megas() {
  u8 *player_party = (u8*) 0x02024284;
  u8 *poke_quantity = (u8*) 0x02024029;
  u8 i;
	
  for (i = 0; i < *poke_quantity; ++i) {
    revert_mega(player_party + i * 100);
  }

  reset_mega();
}

void reset_mega() {
  u8 i;
  for(i = 0; i < 4; ++i) megadata->done[i] = 0;
  for(i = 0; i < 4; ++i) megadata->trigger[i] = 0;
}

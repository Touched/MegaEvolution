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
	
	// Can't mega evolve, we've already done it
	if (megadata->done) {
		return 0;
	}
	
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
	// Unset the trigger
	
	megadata->trigger[bank] = 0;
	
	battle_data *data = &bdata[bank];
	evolution *evo = can_mega_evolve(data);
	
	// Make sure we only mega evolve once. Primals are exempt
	if (evo->unknown != MEGA_VARIANT_PRIMAL) {
		megadata->done = 1;
	}
	
	// Null check
	if (evo) {
	  *b_active_side = bank;
	  build_cmdbuf_mega(0, 4, (u8**) &evo);
	  /* u8 *dest = (u8*) (0x02022BC4 + 0x200 * bank); */
	  /* u32 data = (u32) evo; */

	  /* *dest++ = 0x20;	/\* command index *\/ */
	  /* *dest++ = 4;		/\* byte count *\/ */
	  /* *dest++ = 0; */
	  /* *dest++ = data & 0xFF; */
	  /* *dest++ = (data >> 8) & 0xFF; */
	  /* *dest++ = (data >> 16) & 0xFF; */
	  /* *dest = (data >> 24) & 0xFF; */
	  
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
	megadata->done = 0;
}

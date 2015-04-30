// Mega evolution helper functions

#include "mega.h"
#include "common.h"
#include "pokemon.h"

// Return all the evolution data, null otherwise
evolution *can_mega_evolve(battle_data *pokemon) {
	// Cast to u32 for accurate calculation of the pointer
	evolution *evolutions = (evolution*) ((u32) evolution_table + pokemon->species * sizeof(evolution) * (*evos_per_poke + 1));
	u8 i, j;
	
	for (i = 0; i <= *evos_per_poke; ++i) {
		if (evolutions[i].type == MEGA_EVOLUTION) {
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

void build_cmdbuf_mega(u8 arg, u16 len, u8 **data);
void mark_buffer_for_execution(u8 arg);

void handle_mega_evolution() {
	if (!megadata->trigger[0]) return;
	megadata->trigger[0] = 0;

	*((u8*) 0x02023BC4) = 0;
	
	battle_data *bdata = (battle_data *) 0x02023BE4;
	evolution *evo = can_mega_evolve(bdata);
	
	// Null check
	if (evo) {
		build_cmdbuf_mega(0, 4, (u8**) &evo);
		mark_buffer_for_execution(0);
	}
}

void revert_mega(u8 *poke) {
	u16 species = 0;
	evolution *evo = *evolution_table;
	
	u16 current_species = pokemon_getattr(poke, 0xB, 0);
	
	evolution *evolutions = (evolution*) ((u32) evolution_table + current_species * sizeof(evolution) * (*evos_per_poke + 1));
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

void revert_megas() {
	u8 *player_party = (u8*) 0x02024284;
	u8 *poke_quantity = (u8*) 0x02024029;
	u8 i;
	
	for (i = 0; i < *poke_quantity; ++i) {
		revert_mega(player_party + i * 100);
	}
}

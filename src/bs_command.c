/*
 * Custom battle script command; it handles basically the entire mega evolution
 * transformation.
 */
 
#include "types.h"
#include "common.h"
#include "battle.h"
#include "pokemon.h"

void set_species(u16 index);
void show_message(char *buf);
void prepare_message(char *buf);
u8 *get_pokemon_data();
void buffer_str();

typedef void (*bxcb)(void);
void set_b_x_callback(bxcb callback);

//void move_anim_start(u8,u8,u8,u8);
void animation_script_start(u8 *script, u8 attacker, u8 defender);

extern void play_mega_evolution(u8 attacker, u8 defender);

void command() {
	char *buffer = (char*) 0x0202298C;

	set_species(150);
	prepare_message(buffer);
	show_message(buffer);
	
	//animation_script_start((u8*) 0x081D6594, 0, 1);
	
	play_mega_evolution(0, 1);
	
	//move_anim_start(0, 0, 1, 5);
	//exec_completed();
}

void ability_fix_cb() {
	bxcb *b_c = (bxcb*) 0x03004F84;
	bxcb *bc_backup = (bxcb*) 0x02023D78;
	
	// Call the function that does all the work
	((bxcb) (0x801385C + 1))();
	
	// If this condition is true, the above callback finished and overwrote b_c
	// Restore the back up in this case
	if (*b_c == 0x08014040 + 1) {
		*b_c = *bc_backup;
	}
}

void ability_fix() {
	// Sets b_c to a callback that calls ability_something
	// Fixes abilities that run on enter (drought, etc.)
	// Thanks daniilS!
	u8 **dp08_ptr = (u8**) 0x02023FE8;
	u8 *dp08 = *dp08_ptr;
	bxcb *b_c = (bxcb*) 0x03004F84;
	
	// Some unused word in the memory - pick any
	bxcb *bc_backup = (bxcb*) 0x02023D78;
	
	*(dp08 + 0x4C) = 0;
	*(dp08 + 0xD9) = 0;
	*(dp08 + 0xB6) = 0;
	
	// Fix bug introduced by this - the callback we need resets b_c
	// Remember the old b_c
	*bc_backup = *b_c;
	
	// Use wrapper function
	*b_c = ability_fix_cb;
}

void set_species(u16 index) {
	u16 species = 3;
	u8 *data = get_pokemon_data();
	u8 i;
	
	pokemon_setattr(data, 0xB, (u8*) &species);
	recalc_stats(data);
	
	// TODO: Update healthbar? Mega's shouldn't change health so theres no need
	
	// Update battle data
	// TODO: Calculate correctnessssssss
	battle_data *bdata = (battle_data *) 0x02023BE4 + sizeof(battle_data) * 0; 
	
	// Update stats
	for (i = 0; i < 5; ++i) {
		bdata->stats[i] = pokemon_getattr(data, 0x3B + i, 0);
		//*((u16*)(battle_data + 2 + i * 2)) = pokemon_getattr(data, 0x3B + i, 0);
	}
	
	// We need the base stats to set the rest
	pokemon_base *base_stats = (pokemon_base*) 0x08254784;
	
	// Set ability
	// Megas only have one ability; don't bother with the second one
	bdata->ability_id = base_stats[index].ability1;
	
	// Fix abilities that activate when switching (intimidate, weather abilities, etc.) 
	// do not work
	ability_fix();
	
	// Type changes
	bdata->type1 = base_stats[index].type1;
	bdata->type2 = base_stats[index].type2;
}

char *item_name(u16 index) {
	return (char *) (0x083DB028 + index * 0x2C);
}

char str_apostrophe_s[] = {0xB4, 0xE7, 0x00, 0xFF};
char str_middle[] = {0x00, 0xDD, 0xE7, 0x00, 0xE6, 0xD9, 0xD5, 0xD7, 0xE8, 0xDD, 0xE2, 0xDB, 0xFE, 0xE8, 0xE3, 0x00, 0xFF};
char str_end[] = {0xAB, 0xFB, 0xFF};

void prepare_message(char *buf) {
	// Empty the buffer
	*buf = 0xFF;
	
	char buffer[10];
	u8 *data = get_pokemon_data();
	
	// Add the nickname to the string
	pokemon_getattr(data, 2, (u8*) buffer);
	gf_strcat(buf, buffer);
	gf_strcat(buf, str_apostrophe_s);
	
	// Add item name
	// TODO: Read from held item
	gf_strcat(buf, item_name(0x115));
	
	// Add "is reacting to"
	gf_strcat(buf, str_middle);
	
	// Player's name (first entry in the save block)
	u8 **saveblock2 = (u8**) 0x0300500C;
	gf_strcat(buf, *saveblock2);
	gf_strcat(buf, str_apostrophe_s);
	
	// Add item name - player's accessory; probably a key item
	// TODO: Read from table or variable I'd guess
	gf_strcat(buf, item_name(0x161));
	
	// Exclamation mark and red arrow
	gf_strcat(buf, str_end);
}

void show_message(char *buf) {
	buffer_str();

	// Set vblank
	*((u16*) 0x02022974) = 0;
	*((u16*) 0x02022976) = 0;
	
	// Display the message
	// TODO: Find out what second arg does
	battle_show_message(buf, 0x40);
	
	set_b_x_callback((bxcb) (0x08030610 + 1));
}

void set_b_x_callback(bxcb callback) {
	bxcb *bx = ((bxcb*) 0x03004FE0);
	u8 *b_active_side = (u8*) 0x02023BC4;
	bx += *b_active_side << 2;
	*bx = callback;
}

u8 *get_pokemon_data() {
	u8 *team_index_by_side = (u8*) 0x02023BCE;
	u8 *active_side = (u8*) 0x02023BC4;
	u8 *team = (u8*) 0x02024284;
	
	team_index_by_side += *active_side << 1;
	team += *team_index_by_side * 100;
	return team;
}

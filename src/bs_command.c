/*
 * Custom battle script command; it handles basically the entire mega evolution
 * transformation.
 */

#include "mega.h" 
#include "types.h"
#include "common.h"
#include "battle.h"
#include "pokemon.h"
#include "strings.h"
#include "text.h"
#include "evo.h"
#include "config.h"
#include "tables.h"

#define CURRENT_BANK (*b_active_side)

void set_species(u16 index);
void show_message(char *buf);
u8 *get_pokemon_data();
void buffer_str();
void special_strcpy(u8 *dest, u8 *src);

void set_b_x_callback(bxcb callback);

//void move_anim_start(u8,u8,u8,u8);
void animation_script_start(u8 *script, u8 attacker, u8 defender);

extern void play_mega_evolution(u8 attacker, u8 defender);

evolution *get_evolution_data() {
  u8 *buffer_A = (u8*) (buffer_As + CURRENT_BANK * 0x200);
  return (evolution*) (buffer_A[3] | (buffer_A[4] << 8) | (buffer_A[5] << 16) | (buffer_A[6] << 24));
}

battle_data *get_battle_data() {
  return &b_data[CURRENT_BANK];
}

void healthbar_update(u8 bank);

void wait_for_message();
void AGBPrint(const char *);

extern u8 *string_buffer_maybe;
void command() {
  // Wait for other Mega Evolutions to finish
  if (megadata->running) return;

  // Stop other Mega Evolutions, it's our turn
  megadata->running = 1;

  // Read species from the buffer
  evolution *evo = get_evolution_data();
	
  set_species(evo->species);
	
  // Update health box (to hide level text)
  healthbar_update(CURRENT_BANK);
	
  // TODO: Support no message (for primals)
  special_strcpy((u8*) string_buffer_maybe, (u8*) str_before[evo->unknown]);
  show_message((char*) string_buffer_maybe);
		
  set_b_x_callback((bxcb) wait_for_message);
}

void set_species(u16 index) {
  u16 species = index;
  u8 *data = get_pokemon_data();
  u8 i;
	
  pokemon_setattr(data, 0xB, (u8*) &species);
  recalc_stats(data);
	
  // TODO: Update healthbar? Mega's shouldn't change health so theres no need
	
  // Update battle data
  battle_data *bdata = get_battle_data();
	
  bdata->species = species;
	
  // Update stats
  for (i = 0; i < 5; ++i) {
    bdata->stats[i] = pokemon_getattr(data, 0x3B + i, 0);
    //*((u16*)(battle_data + 2 + i * 2)) = pokemon_getattr(data, 0x3B + i, 0);
  }
	
  // Set ability
  // Megas only have one ability; don't bother with the second one
  bdata->ability_id = base_stats[index].ability1;
	
  // Type changes
  bdata->type1 = base_stats[index].type1;
  bdata->type2 = base_stats[index].type2;
}

char *item_name(u16 index) {
  return (char *) (item_names + index * 0x2C);
}

char *get_trainer_name() {
  if (CURRENT_BANK & 1) {
    u16 *trainer_flag = (u16*) 0x020386AE;
    u8 *trainer = trainers + *trainer_flag * 0x28;

#ifdef BPRE
#ifndef NO_RIVAL_NAME_SWAP // Optional
    // FireRed Rival Name for Champion and Rival classes
    u8 tclass = *(trainer + 1);
    if (tclass == 0x51 || tclass == 0x5A) {
      return (char*) (*saveblock1) + 0x3A4C;
    }
#endif
#endif

    return (char*) trainer + 4;
  } else {
    return (char*) *saveblock2;
  }
}

u16 get_keystone_index() {
  u16 *var;
  if (CURRENT_BANK & 1) {
    var = var_access(KEYSTONE_OPPONENT_VAR);
  } else {
    var = var_access(KEYSTONE_PLAYER_VAR);
  }

  u16 item = *var;
  return item ? item : KEYSTONE_DEFAULT;
}

char *get_species_name(u8 *pokemon_data) {
  u16 species = pokemon_getattr(pokemon_data, 0xB, 0);
  
  char *name = (char*) (pokemon_names + 0xB * species);
  return name;
}

void special_strcpy(u8 *dest, u8 *src) {
  u8 ch;
  u8 *data = get_pokemon_data();
  char buffer[10];
  char* buf = 0;
	
  battle_data *bdata = get_battle_data();
	
  while ((ch = *src++) != 0xFF) {
    // Do something different for variables
    if (ch == 0xFD) {
      switch (*src++) {
	// Pokemon's nickname
      case 0:
	buf = (char*) buffer;
	pokemon_getattr(data, 2, buffer);
	break;
	// Held item
      case 1:
	buf = item_name(bdata->held_item);
	break;
	// Trainer's name
      case 2:
	// TODO: Load trainer name if enemy is mega evolving
	//buf = *((u8**) 0x0300500C);
	buf = get_trainer_name();
	break;
	// Trainer's accessory
      case 3:
	// TODO: Support loading accessories from a table or something
	buf = item_name(get_keystone_index());
	break;
      case 4:
	buf = get_species_name(data);
	break;
      }
			
      // Copy smaller buffer into string
      if(buf) {
	while ((*dest++ = (u8) *buf++) != 0xFF);
      }
      dest--;
    } else {
      *dest++ = ch;
    }
  }
	
  *dest = 0xFF;
}

void exec_completed() {
  megadata->running = 0;

  if (CURRENT_BANK & 1) {
    exec_completed_tbl2();
  } else {
    exec_completed_tbl1();
  }

  // If mutliple Mega Evolutions happen in a row, this doesn't get set. So force set it
  // TODO: Neaten up
  *((u32*) 0x03004F84) = (0x080155c9);
};

void delay_before_end() {
  u16 *timer = (u16*) 0x02023D7E;
	
  if (*timer > 0) {
    (*timer)--;
  } else {
    exec_completed();
  }
}

void wait_transformation_message() {
  u16 *timer = (u16*) 0x02023D7E;

  if (!a_pressed_maybe(0)) {
    *timer = 0x3F;
    set_b_x_callback((bxcb) delay_before_end);
  }
}

void transformation_message() {
  char *buffer = (char*) 0x0202298C;
	
  evolution *evo = get_evolution_data();

  // TODO: Post transformation message
  special_strcpy((u8*) buffer, (u8*) str_after[evo->unknown]);
  show_message(buffer);
	
  set_b_x_callback((bxcb) wait_transformation_message);
}

void wait_for_animation() {
  u16 *timer = (u16*) 0x02023D7E;
  // move_anim_active_task_count
  if (*timer > 0) {
    (*timer)--;
  } else {
    if (*((u8*) 0x02037EE2) == 0) {
      set_b_x_callback((bxcb) transformation_message);
    }
  }
}

void delay_for_animation() {
  // Unused halfword - just pick any piece of padding
  u16 *timer = (u16*) 0x02023D7E;
	
  if (*timer > 0) {
    (*timer)--;
  } else {
    *timer = 0xFF;
    play_mega_evolution(CURRENT_BANK, 1);
    set_b_x_callback((bxcb) wait_for_animation);
  }
}

// Wait for message to finish printing before displaying the animation
void wait_for_message() {
  // a_pressed_maybe is called immediately after the message is finished
  // rendering
  u16 *timer = (u16*) 0x02023D7E;
	
  if (!a_pressed_maybe(0)) {
    *timer = 0x30;
    set_b_x_callback((bxcb) delay_for_animation);
  }
}

void show_message(char *buf) {
  buffer_str();

  // Set vblank
  *((u16*) 0x02022974) = 0;
  *((u16*) 0x02022976) = 0;
	
  // Display the message
  // TODO: Find out what second arg does
  battle_show_message(buf, 0x40);
}

void set_b_x_callback(bxcb callback) {
  bx[CURRENT_BANK] = callback;
}

u8 *get_pokemon_data() {
  u8 *current_team;

  current_team = team;
  if (CURRENT_BANK) current_team = enemy_team;
  
  current_team += (team_index_by_side[*b_active_side]) * 100;
  return current_team;
}

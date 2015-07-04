#include "types.h"
#include "animscript.h"
#include "common.h"
#include "battle.h"

#define MEGA_ANIM_PARTICLE_RAINBOW 1
#define MEGA_ANIM_PARTICLE_STONE 2
#define MEGA_ANIM_PARTICLE_SYMBOL 3

#define RAINBOW_X_LINE(a,b,c) 0x2, ANIM_LONG(anim_rainbow_y),	\
    0x2, 0x3, a, b, c, 0xFF, 0xD, 0x0, 0x4, 0x3
	
extern u8 *mega_animation_script;
	
// 08071E60 - could be playing the cry
u8 anim_script[] = {
  // Load Mega Stone
  
  3,
  ANIM_LONG(0x080F1420 + 1),
  2,
  1,
  ANIM_SHORT(1),
	
  //4, 10,
	
  // Play cry
  // 0x3, ANIM_LONG(0x80DD148 + 1), 1, 2, ANIM_SHORT(0), ANIM_SHORT(0xFF),
  8
};

// From anim.s
void animation_script_start(u8 *script, u8 attacker, u8 defender);

void play_mega_evolution(u8 attacker, u8 defender) {
  animation_script_start(mega_animation_script, attacker, attacker);
}

// Declare the functions all here because lazy
void move_anim_task_del(u8 index);
void refresh_graphics_maybe(u8, u8, u8);
u8 *battle_side_objid_P_and_priv5_for_dp11b3 = 0x02023D44;
u8 *b_anim_attacker = 0x02037F1A;

typedef struct task {
  u32 function;
  u8 in_use;
  u8 prev;
  u8 next;
  u8 priority;
  u16 args[16];
} task;

task *tasks = (task*) 0x03005090;
u8 task_add(void (*funcptr)(u8), u8 priority);
u8 task_find_id_by_funcptr(void (*funcptr)(u8));
u8 task_is_running(u8 index);

// Actual function
void task_ma_swap_sprites(u8 index) {
  u8 s = battle_side_objid_P_and_priv5_for_dp11b3[*b_anim_attacker];
	
  task *me = &tasks[index];
  task *t;
  u8 find_index;
  u8 next = 0;
	
  switch (me->args[10]) {
  case 0:
    // To fix an annoying graphical glitch where the old sprite would flash
    // momentarily, we hide the sprite offscreen while we refresh it.
    // Remember the old position so we can go back to it later.
    me->args[11] = *(s16*)(0x0202063C + s * 0x44 + 0x20);
    *(s16*)(0x0202063C + s * 0x44 + 0x20) = -64;
		
    // Load the palette and graphics. Note this doesn't cause the sprite to
    // refresh
    refresh_graphics_maybe(*b_anim_attacker, 1, s);
    next = 1;
    break;
  case 1:
    // Actually update the sprite now
    *(u8*)(0x0202063C + s * 0x44 + 0x3E) &= 0xFB;
    t = &tasks[task_add(0x0807331C + 1, 5)];
    t->args[0] = 0;
    t->args[2] = *b_anim_attacker;
    next = 1;
    break;
  case 2:
    // Make sure the task is done. I'm not sure if this is necessary
    find_index = task_find_id_by_funcptr(0x0807331C + 1);
    if (!task_is_running(find_index)) {
      next = 1;
    }
    break;
  case 3:
    // Restore the old X position and end the task
    *(s16*)(0x0202063C + s * 0x44 + 0x20) = me->args[11];
    move_anim_task_del(index);
    break;
  }
	
  // Update the state
  if (next) {
    me->args[10]++;
  }
}

void play_cry_maybe(u16 species);
u8 battle_side_get_owner(u8 index);
u8 *b_pokemon_team_id_by_side = (u8*) 0x02023BCE;
u8 *party_player = (u8*) 0x02024284;
u8 *party_opponent = (u8*) 0x0202402C;

void task_ma_cry(u8 index) {
  u16 species = bdata[*b_anim_attacker].species;
	
  // Play the cry and end the task immediately
  play_cry_maybe(species);
  move_anim_task_del(index);
}

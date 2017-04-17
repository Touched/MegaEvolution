#ifndef BATTLE_H_
#define BATTLE_H_

#include "types.h"

#pragma pack(push, 1)
typedef struct battle_data
{
  u16 species;
  u16 stats[5];
  u16 move_ids[4];
  u32 ivs;
  u8 stat_buffs[8];
  u8 ability_id;
  u8 type1;
  u8 type2;
  u8 padding;
  u8 current_pp[4];
  u16 current_hp;
  u8 level;
  u8 happiness;
  u16 max_hp;
  u16 held_item;
  char name[11];
  u8 field_3B;
  u8 trainer_name[8];
  u32 padding3;
  u32  pid;
  u32 status1;
  u32 status2;
  u32  otid;
} battle_data;
#pragma pack(pop)

typedef void (*bxcb)(void);

u8 *b_active_side = (u8*) 0x02023BC4;
u8 *b_attacker = (u8*) 0x02023D6B;
u8 *b_defender = (u8*) 0x02023D6C;
u8 *b_current_bank = (u8*) 0x02023D6F;
u8 *team = (u8*) 0x02024284;
u8 *enemy_team = (u8*) 0x0202402C;
//battle_data *bdata = (battle_data *) (0x02023BE4);
u32 *battle_type_flags = (u32*) (0x02022B4C);
u8 *b_num_active_sides = (u8*) 0x02023BCC;
u32 *b_x = (u32 *) 0x03004FE0;
bxcb *b_c = (bxcb*) 0x03004F84;
u8 **dp08_ptr = (u8**) 0x02023FE8;
bxcb *bx = ((bxcb*) 0x03004FE0);
u16 *team_index_by_side = (u16*) 0x02023BCE;
u8 *battle_side_objid_P_and_priv5_for_dp11b3 = (u8*) 0x02023D44;
u8 *b_anim_attacker = (u8*) 0x02037F1A;
u8 *buffer_As = (u8*) 0x02022BC4;
u8 *b_attackers_in_order = (u8 *) 0x02023BDE;
battle_data *b_data = (battle_data *) 0x02023BE4;

// u8 *b_attackers_in_order = (u8*) (0x02023BDE);

#define dp08 (*dp08_ptr)

// Some unused word in the memory - pick any
// TODO: Clean up (malloc)
bxcb *bc_backup = (bxcb*) 0x02023D78;

#pragma pack(push, 1)
typedef struct dp11b
{
  u8 field0;
  u8 field1;
  u8 objid_2;
  u8 objid_3;
  u8 field4;
  u8 field5;
  u16 gap_6;
  u8 field8;
  u16 field9;
  u8 fieldB;
} dp11b;
#pragma pack(pop)

typedef struct dp11
{
  u32 *a;
  dp11b *b;
  u32 c;
  u32 d;
} dp11;

dp11 *dp11_ptr;// = (dp11*) 0x02024018;

#endif 

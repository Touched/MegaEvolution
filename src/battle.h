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

u8 *b_active_side = (u8*) 0x02023BC4;
u8 *b_attacker = (u8*) 0x02023D6B;
u8 *b_defender = (u8*) 0x02023D6C;
u8 *b_current_bank = (u8*) 0x02023D6F;
u16 *team_index_by_side = (u16*) 0x02023BCE;
u8 *team = (u8*) 0x02024284;
battle_data *bdata = (battle_data *) (0x02023BE4);
u32 *battle_type_flags = (u32*) (0x02022B4C);
u8 *b_num_active_sides = (u8*) 0x02023BCC;
u32 *b_x = (u32 *) 0x03004FE0;

#endif 

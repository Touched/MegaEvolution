#ifndef POKEMON_H_
#define POKEMON_H_

#include "types.h"

typedef struct pokemon_base
{
  u8 hp;
  u8 atk;
  u8 def;
  u8 spd;
  u8 spatk;
  u8 spdef;
  u8 type1;
  u8 type2;
  u8 catch_rate;
  u8 xp_yield;
  u16 effort_yield;
  u16 item1;
  u16 item2;
  u8 gender;
  u8 steps_to_hatch;
  u8 base_friendship;
  u8 exp_point_table_nr;
  u8 eggroup1;
  u8 eggroup2;
  u8 ability1;
  u8 ability2;
  u8 safari_zone_flee_rate;
  u8 color;
  u16 padding;
}  pokemon_base;

#endif

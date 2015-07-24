#ifndef EVO_H_
#define EVO_H_

#include "types.h"

typedef struct evolution {
  u16 type;
  u16 argument;
  u16 species;
  u16 unknown;
} evolution;

//evolution **evolution_table = (evolution**) 0x08259754;
evolution ***evolution_table_ptr = ((evolution***) 0x08042F6C);
#define evolution_table (*evolution_table_ptr)

// an offsetstochangetonewminus1 offset in PokeRoms.ini from G3HS
u8 *evos_per_poke = (u8*) 0x08043116; 

// The evolutionary type for Megas
#define MEGA_EVOLUTION 0xFE

// Mega evolution variants
#define MEGA_VARIANT_STANDARD 0
#define MEGA_VARIANT_PRIMAL 1
#define MEGA_VARIANT_WISH 2 // Rayquaza

#endif

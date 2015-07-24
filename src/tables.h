#ifndef TABLES_H_
#define TABLES_H_

#include "evo.h"

char **pokemon_names_ptr = (char **) 0x08011450;
char **item_names_ptr = (char **) 0x0809A944;
u8 **trainers_ptr = (u8 **) 0x0800FC00;
pokemon_base **base_stats_ptr= (pokemon_base**) 0x08136794;

#define pokemon_names (*pokemon_names_ptr)
#define item_names (*item_names_ptr)
#define trainers (*trainers_ptr)
#define base_stats (*base_stats_ptr)

#endif

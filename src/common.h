#ifndef COMMON_H_
#define COMMON_H_

void exec_completed_tbl1();
void exec_completed_tbl2();
u8 *pokemon_getattr(u8 *data, u8 attr, u8 *ret);
void pokemon_setattr(u8 *data, u8 attr, u8 *value);
void gf_strcat(char *dest, char *src);
void battle_show_message(char *msg, u8 arg);
void ability_something_2(); 
void ability_something(u8,u8,u8); 
void recalc_stats(u8 *data);
char *int_to_str(char *buffer, u8 lvl, u8 a, u8 b);
u16 *var_access(u16 index);
u8 checkitem(u16 index, u16 quanity);

#endif

#ifndef COMMON_H_
#define COMMON_H_

void exec_completed();
u8 *pokemon_getattr(u8 *data, u8 attr, u8 *ret);
void pokemon_setattr(u8 *data, u8 attr, u8 *value);
void gf_strcat(char *dest, char *src);
void battle_show_message(char *msg, u8 arg);
void ability_something_2(); 
void ability_something(u8,u8,u8); 

#endif

#ifndef TRAINERS_H_
#define TRAINERS_H_

typedef struct trainer {
  u8 a;
  u8 trainer_class;
  u8 music;
  u8 sprite;
  char name[10];
  u16 a;
  
} trainer;
// 28 bytes

u16 *trainer_flag = (u16*) 0x020386AE;
trainer *trainers = (trainer*) 0x0823EAC8;

#endif 

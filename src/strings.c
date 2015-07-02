#include "text.h"

char str_mega_evo[] = {
  PC_BUFFER(0), PC_APOSTROPHE, PC_s, PC_SPACE, PC_BUFFER(1),
  PC_SPACE, 
  PC_i, PC_s,
  PC_SPACE, 
  PC_r, PC_e, PC_a, PC_c, PC_t, PC_i, PC_n, PC_g, 0xFE, 
  PC_t, PC_o,
  PC_SPACE,
  PC_BUFFER(2), PC_APOSTROPHE, PC_s, PC_SPACE, PC_BUFFER(3),
  PC_EXCLAMATION, PC_END // !
};

char str_fervent_wish[] = {
  PC_BUFFER(2), PC_APOSTROPHE, PC_s, PC_SPACE, PC_f, PC_e, PC_r, PC_v, PC_e, PC_n, PC_t,
  PC_SPACE, PC_w, PC_i, PC_s, PC_h, PC_NEWLINE, PC_h, PC_a, PC_s, PC_SPACE,
  PC_r, PC_e, PC_a, PC_c, PC_h, PC_e, PC_d, PC_SPACE, PC_BUFFER(0),
  PC_EXCLAMATION, PC_END
};

char str_post_mega[] = {
  PC_BUFFER(0), PC_SPACE, PC_M, PC_e, PC_g, PC_a, PC_SPACE, PC_E, PC_v, PC_o,
  PC_l, PC_v, PC_e, PC_d, PC_SPACE, PC_i, PC_n, PC_t, PC_o, PC_SPACE, 0xFE, 
  PC_M, PC_e, PC_g, PC_a, PC_SPACE, PC_BUFFER(4), PC_EXCLAMATION, PC_END
};

char *str_before[] = {
  str_mega_evo,
  0,
  str_fervent_wish
};

char *str_after[] = {
  str_post_mega,
  0,
  str_post_mega
};

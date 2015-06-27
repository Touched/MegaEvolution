#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct colour {
  u16 r : 5;
  u16 g : 5;
  u16 b : 5;
} color;

typedef struct palette {
  u16 c[16];
} palette;

palette *palette_bg_unfaded = (palette*) 0x020371F8;
palette *palette_obj_unfaded = (palette*) 0x020373F8;
palette *palette_bg_faded = (palette*) 0x020375F8;
palette *palette_obj_faded = (palette*) 0x020377F8;

u8 gpu_pal_tags_index_of(u16 tag);

#endif

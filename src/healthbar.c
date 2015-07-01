#include "objects.h"
#include "battle.h"
#include "common.h"
#include "graphics.h"
#include "mega.h"
#include "config.h"
#include "images/indicators.h"
#include "images/mega_trigger.h"

//resource gfx_healthbar = {0x083EF524, 0x80, 0x1234};
resource gfx_indicator = {indicatorsTiles, 0x80, 0x1234};
resource pal_indicator = {indicatorsPal, 0x1234};
resource gfx_trigger = {mega_triggerTiles, 0x1C00, 0x2345};
resource pal_trigger = {mega_triggerPal, 0x2345};
// 083F6CBO - 32x32?

sprite mega_indicator = {0, 0x0, 0x00, 0x0};
//sprite mega_icon = {0x0, 0x4000, 0x000, 0x0};
sprite mega_trigger = {0, 0x8000, 0x800, 0};

void healthbar_trigger_callback(object *self);
void healthbar_indicator_callback(object *self);

template template_indicator = {0x1234, 0x1234, &mega_indicator, 0x08231CF0, 0, 0x08231CFC, healthbar_indicator_callback};
template template_trigger = {0x2345, 0x2345, &mega_trigger, 0x08231CF0, 0, 0x08231CFC, healthbar_trigger_callback};

/* Declare the colors the trigger button ignores */
u16 ignored_cols[TRIGGER_NUM_IGNORED_COLORS] = {
  TRIGGER_IGNORED_COLORS
};

/* Easy match function */
u8 ignored_trigger_color(u16 color) {
  u8 i;
  for (i = 0; i < TRIGGER_NUM_IGNORED_COLORS; ++i) {
    if (ignored_cols[i] == color) return 1;
  }
  return 0;
}

// charset: 0 - en, 1 -jp
int font_get_width_of_string(u8 charset, char *string, u16 xcursor);
void gpu_pal_obj_alloc_tag_and_apply(resource *pal);

object *get_healthbox_objid(u8 bank) {
  u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;
  u8 id = healthbox_objid_by_side[bank];
  return &objects[id];
}

s16 get_pingpong(s16 phase, s16 scale);

u16 calcGrayscale(u16 color) {
  u32 r = color & 31,
    g = (color >> 5) & 31,
    b = (color >> 10) & 31;
		
  r *= 0x4C;
  g *= 0x96;
  b *= 0x1E;
	
  u16 gray = (r + g + b + 0x80) >> 8;
	
  return gray | (gray << 5) | (gray << 10);
}

u16 calcEnabled(u16 clra) {
  u16 clrb = 0x7FFF;
	
  u32 currentAlpha  = 20;


  const u32 rbmask= ((0x1F)|(0x1F << 10)), gmask= 0x1F << 5;
  const u32 rbhalf= 0x4010, ghalf= 0x0200;

  // Red and blue
  u32 parta = clra & rbmask;
  u32 partb = clrb & rbmask;
  u32 part = (partb-parta) * (32 - ((currentAlpha < 0x100) ? currentAlpha : currentAlpha >> 12)) + parta*32 + rbhalf;
  u16 clr = (part >> 5) & rbmask;

  // Green
  parta = clra & gmask;
  partb = clrb & gmask;
  part = (partb-parta) * (32 - ((currentAlpha < 0x100) ? currentAlpha : currentAlpha >> 12)) + parta*32 + ghalf;
  clr |= (part >> 5) & gmask;

  return clr;
}

void healthbar_trigger_callback(object *self) {
  if (!can_mega_evolve(&bdata[*b_current_bank])) {
    self->x = -32;
    return;
  }
	
  s16 xshift, yshift;
  if (*battle_type_flags & 1) {
    xshift = -6;
    yshift = 0;
  } else {
    xshift = -5;
    yshift = 2;
  }

  // Find the health box object that this trigger is supposed to be attached to
  u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;
  u8 id = healthbox_objid_by_side[*b_current_bank];
  object *healthbox = &objects[id];
	
  u8 y = (u8) healthbox->final_oam.attr0;

  u8 pingid = dp11_ptr->b[*b_current_bank].objid_2;
  object *ping = &objects[pingid]; 
  if (y) {
    // Copy the healthbox's position (it has various animations)
    //self->y = healthbox->y + 20;
    self->x = (healthbox->final_oam.attr1 & 0x1FF) + xshift + self->private[3];
    self->y2 = get_pingpong(ping->private[0], ping->private[2]);
    self->y = healthbox->y + yshift;
  } else {
    // The box is offscreen, so hide this one as well
    self->x = -32;
  }
	
  if (b_x[*b_current_bank] == 0x0802EA11) {
    if (self->private[3] > 0) {
      self->private[3] -= 2;
    } else {
      self->private[3] = 0;
    }
  } else {
    if (self->private[3] < 16) {
      self->private[3] += 2;
    } else {
      // Hide offscreen once invisible
      self->x = -32;
      return;
    }
  }
	
  if (megadata->done[*b_current_bank]) {
    self->private[1] = 3;
  } else {
    if (megadata->trigger[*b_current_bank]) {
      self->private[1] = 1;
    } else {
      self->private[1] = 2;
    }
  }
	
  // Only change the palette if the state has changed
  if (self->private[1] != self->private[2]) {
    palette *pal = &palette_obj_faded[gpu_pal_tags_index_of(0x2345)];
    u8 i;
		
    for(i = 1; i < 16; i++) {
      if (ignored_trigger_color(mega_triggerPal[i])) continue;
			
      if (self->private[1] == 1) {
	pal->c[i] = calcEnabled(mega_triggerPal[i]);
      } else if (self->private[1] == 2) {
	pal->c[i] = mega_triggerPal[i];
      } else if (self->private[1] == 3) {
	pal->c[i] = calcGrayscale(mega_triggerPal[i]);
      }
    }
		
    self->private[2] = self->private[1];
  }
}

void healthbar_indicator_callback(object *self) {
  // Visibility
  if (!is_mega(&bdata[self->private[0]])) {
    self->x = -8;
    return;
  }
  object *healthbox = get_healthbox_objid(self->private[0]);
	
  if (healthbox->bitfield2 & 4) {
    self->x = -8;
    return;
  }
	
  u8 y = (u8) healthbox->final_oam.attr0,
    x =  (healthbox->final_oam.attr1 & 0x1FF);
		
  // Mirror healthbox priority
  u8 priority = ((healthbox->final_oam.attr2 >> 10) & 3);
  self->final_oam.attr2 = (self->final_oam.attr2 & ~0xC00) | (priority << 10);
		
  if (y) {
    // Figure out the X position for the indicator - it differs depending on
    // the battle type and the side the healthbox represents.
    s16 shift = 64; // Halfway point for OAM
	
    if (self->private[0] & 1) {
      // enemy
      shift += 18;
    } else {
      // player
      shift += 26;
    }
		
    // Convert the level to a string to get how long it is
    char buf[10];
    battle_data *data = &bdata[self->private[0]];
    u8 stringlen = int_to_str(buf, data->level, 0, 3) - buf;
		
    // The x position depends on the X origin of the healthbox as well as
    // the string length
    self->x = x + shift - 5 * stringlen;
		
    u8 pingid = dp11_ptr->b[self->private[0]].objid_2;
    object *ping = &objects[pingid]; 
		
    u8 pingpong_active = (dp11_ptr->b[self->private[0]].field0 & 6);
    u8 *shaker_data = (u8*) 0x02022AD0;
		
    object *shaker = &objects[shaker_data[1]];
    u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;
    u8 hbid = healthbox_objid_by_side[self->private[0]];
		
    if (pingpong_active) {
      // objc_dp11b_pingpong
      self->y = healthbox->y - 4;
      self->y2 = get_pingpong(ping->private[0], ping->private[2]);
      return;
    } else if (shaker->private[1] == hbid && *shaker_data) {
      self->y2 = (shaker->private[2] & 1) ? -1 : 1;
      return;
    }
		
    // Fix indicator position
    self->y = healthbox->y - 5;
    self->y2 = 0;
    self->x2 = 0;
  } else {
    self->x = -8;
  }
}

void healthbar_load_graphics(u8 state) {
  u8 objid;

  if (state == 2) {
    gpu_pal_obj_alloc_tag_and_apply(&pal_indicator);
    gpu_pal_obj_alloc_tag_and_apply(&pal_trigger);
	
    gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_indicator);
    gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_trigger);
		
		
    // Create a Mega Indicator for every bank
    u8 bank;
    for (bank = 0; bank < *b_num_active_sides; ++bank) {
      objid = template_instanciate_forward_search(&template_indicator, 0, 0, 1);
      objects[objid].private[0] = bank;
    }
    
    if (mega_evolution_enabled()) {
      objid = template_instanciate_forward_search(&template_trigger, 130, 90, 1);
    }
  }
}

void healthbar_update(u8 bank) {
  u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;

  u8 *current_team = (bank & 1) ? enemy_team : team;
  u8 id = team_index_by_side[bank];

  healthbar_draw_field(healthbox_objid_by_side[bank], current_team + id * 100, 0);
}

void healthbar_display_graphics() {
  //gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_healthbar);
  //template_instanciate_forward_search(&template_healthbar, 10, 10, 1);
}

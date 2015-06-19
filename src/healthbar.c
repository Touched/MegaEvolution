#include "objects.h"
#include "battle.h"

resource gfx_healthbar = {0x083EF524, 0x80, 0x1234};
resource gfx_trigger = {0x083EF524, 0x1C00, 0x2345};
// 083F6CBO - 32x32?

sprite mega_icon = {0x8000, 0x0, 0x000, 0x0};
sprite mega_trigger = {0, 0x8000, 0x800, 0};

void healthbar_trigger_callback(object *self);
void healthbar_indicator_callback(object *self);

template template_healthbar = {0x1234, 0xD6FF, &mega_icon, 0x08231CF0, 0, 0x08231CFC, healthbar_indicator_callback};
template template_trigger = {0x2345, 0xD6FF, &mega_trigger, 0x08231CF0, 0, 0x08231CFC, healthbar_trigger_callback};

/*
I don't know how much space I can use for OAMs in battle, so it should be kept
to a bare minimum. If done properly, we can be sure to only use 14 tiles and 3
palettes.

2 tiles per level icon replacer - 1 for each primal orb and 1 for the keystone
4 tiles for per big trigger icon. We can have a max of 2 of these.

1 palette for the level icon replacers
1 pallete for each big trigger icon so we can change the palette to convey state
*/

object *get_healthbox_objid(u8 bank) {
	u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;
	u8 id = healthbox_objid_by_side[bank];
	return &objects[id];
}

void healthbar_trigger_callback(object *self) {
	// Find the health box object that this trigger is supposed to be attached to
	u8 *healthbox_objid_by_side = (u8*) 0x03004FF0;
	u8 id = healthbox_objid_by_side[0];
	object *healthbox = &objects[id];
	
	u8 y = (u8) healthbox->final_oam.attr0;

	if (y) {
		// Copy the healthbox's position (it has various animations)
		self->y = y + 16;
		self->x = (healthbox->final_oam.attr1 & 0x1FF) - 7;
	} else {
		// The box is offscreen, so hide this one as well
		self->x = -32;
	}
	
	// TODO: Switch palettte according to button state
}

void healthbar_indicator_callback(object *self) {
	object *healthbox = get_healthbox_objid(0);
	
	u8 y = (u8) healthbox->final_oam.attr0,
		x =  (healthbox->final_oam.attr1 & 0x1FF);
		
	// TODO: Determine font width of level and adjust x pos accordingly
	if (y) {
		self->y = y + 8;
		self->x = x + 64 + 24;
	} else {
		self->x = -8;
	}
	
	// TODO: Visibility
}

void healthbar_load_graphics(u8 state) {
	if (state == 2) {
		gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_healthbar);
		template_instanciate_forward_search(&template_healthbar, 90, 25, 1);
		gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_trigger);
		template_instanciate_forward_search(&template_trigger, 130, 90, 1);
	}
	// state 
	// 3: player 2
	// 4: enemy 1
	// 5: enemy 2
}


void healthbar_display_graphics() {
	//gpu_tile_obj_decompress_alloc_tag_and_upload(&gfx_healthbar);
	//template_instanciate_forward_search(&template_healthbar, 10, 10, 1);
}

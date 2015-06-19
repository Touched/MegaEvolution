#include "objects.h"

resource gfx_healthbar = {0x083EF524, 0x80, 0x1234};
resource gfx_trigger = {0x083EF524, 0x1C00, 0x2345};
// 083F6CBO - 32x32?

sprite mega_icon = {0x8000, 0x0, 0x000, 0x0};
sprite mega_trigger = {0, 0x8000, 0x800, 0};

template template_healthbar = {0x1234, 0xD6FF, &mega_icon, 0x08231CF0, 0, 0x08231CFC, 0x0800760C + 1};
template template_trigger = {0x2345, 0xD6FF, &mega_trigger, 0x08231CF0, 0, 0x08231CFC, 0x0800760C + 1};

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

#include "../include/ei_draw.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color){
	int* ir;
	int* ig;
	int* ib;
	int* ia;
	hw_surface_get_channel_indices(surface, ir, ig, ib, ia);
	uint32_t ret = 0;
	if (*ia == -1)
	{
			ret = (color->red << (3-*ir)*8) + (color->green << (3-*ig)*8) + (color->blue << (3-*ib)*8) + 255;
	}
	else
	{
			ret = (color->red << (3-*ir)*8) + (color->green << (3-*ig)*8) + (color->blue << (3-*ib)*8) + (color->alpha << *ia*8);
	}
	return ret;
}


void ei_draw_polyline (ei_surface_t surface, const ei_linked_point_t*	first_point, const ei_color_t color, const ei_rect_t* clipper){
	hw_surface_lock(surface);
	// while(first_point != NULL){
	// 	printf("BOB");
	// 	first_point = first_point->next;
	// }

	uint32_t* pixel;
	pixel = (uint32_t*)hw_surface_get_buffer(surface);

	pixel += 800 * 100 + 50;
	*pixel = ei_map_rgba(surface, &color);

	//for (int i = 0; i < (800 * 600); i++)


	hw_surface_unlock(surface);
	hw_surface_update_rects(surface, NULL);
}


void ei_draw_polygon (ei_surface_t surface, const ei_linked_point_t* first_point, const ei_color_t color, const ei_rect_t* clipper){

}

void ei_draw_text (ei_surface_t surface, const ei_point_t* where, const char* text, const ei_font_t font, const ei_color_t*	color, const ei_rect_t*	clipper){

}


void ei_fill (ei_surface_t surface, const ei_color_t*	color, const ei_rect_t*	clipper){

}

int	ei_copy_surface (ei_surface_t destination, const ei_rect_t*	dst_rect, const ei_surface_t source, const ei_rect_t*	src_rect, const ei_bool_t	alpha){
	return 0;
}

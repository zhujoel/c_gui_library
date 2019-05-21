#include "../include/ei_draw.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color){
	return 0xba3030ff;
}


void ei_draw_polyline (ei_surface_t surface, const ei_linked_point_t*	first_point, const ei_color_t color, const ei_rect_t* clipper){
	hw_surface_lock(surface);

	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);

	// point de départ
	ei_point_t current = first_point->point;
	// point d'arrivée
	ei_point_t arrivee = first_point->next->point;
	float erreur = 0;
	int deltax = abs(arrivee.x - current.x);
	int deltay = abs(arrivee.y - current.y);
	if ( deltax > deltay){
		// on incrémente x de 1

		while((current.x != arrivee.x) && (current.y != arrivee.y)){
			current.x++;
			erreur += (float)deltay/deltax;

			if (erreur > 0.5){
				current.y++;
				erreur--;
			}
			pixel_ptr += current.x + current.y*800;
			*pixel_ptr = 0xba3030ff;
			pixel_ptr -= current.x + current.y*800;
		}
	}
	else{
		// on incrémente y de 1
		while((current.x != arrivee.x) && (current.y != arrivee.y)){
			current.y++;
			erreur += (float)deltay/deltax;

			if (erreur > 0.5){
				current.x++;
				erreur--;
			}
			pixel_ptr += current.x + current.y*800;
			*pixel_ptr = 0xba3030ff;
			pixel_ptr -= current.x + current.y*800;
		}
	}


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

#include "../include/ei_draw.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


uint32_t ei_map_rgba (ei_surface_t surface, const ei_color_t* color){
	int* ir = malloc(sizeof(int));
	int* ig = malloc(sizeof(int));
	int* ib = malloc(sizeof(int));
	int* ia = malloc(sizeof(int));
	hw_surface_get_channel_indices(surface, ir, ig, ib, ia);
	uint32_t ret = 0;
	if (*ia == -1)
	{
		ret = (color->red << (*ir)*8) + (color->green << (*ig)*8) + (color->blue << (*ib)*8) + 0xff000000;
	}
	else
	{
		ret = (color->red << (*ir)*8) + (color->green << (*ig)*8) + (color->blue << (*ib)*8) + (color->alpha << (*ia)*8);
	}
	return ret;
}

void ei_draw_polyline (ei_surface_t surface, const ei_linked_point_t*	first_point, const ei_color_t color, const ei_rect_t* clipper){
	hw_surface_lock(surface);

	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);

	// si il n'y a qu'un seul point
	if(first_point->next == NULL){
		pixel_ptr +=  first_point->point.x +  first_point->point.y*800;
		*pixel_ptr = 0xba3030ff;
	}
	// si on a plusieurs points
	else{
		// point de départ
		ei_point_t current = first_point->point;
		// suiveur d'arrivée
		ei_linked_point_t *suiveur_arrivee = first_point->next;
		// on dessine les segments pour tous les points
		while(suiveur_arrivee != NULL){
			int8_t incrementx = 1;
			int8_t incrementy = 1;
			// point d'arrivée
			ei_point_t arrivee = suiveur_arrivee->point;
			// si le point 1 est "avant" le point 2 en x
			if(current.x > arrivee.x){
				incrementx = -1;
			}
			if(current.y > arrivee.y){
				incrementy = -1;
			}
			int deltax = abs(arrivee.x - current.x);
			int deltay = abs(arrivee.y - current.y);
			float erreur = 0;

			if(deltax == 0){
				// on incrémente y de 1
				while(current.y != arrivee.y){
					current.y += incrementy;
					pixel_ptr += current.x + current.y*800;
					*pixel_ptr = 0xba3030ff;
					pixel_ptr -= current.x + current.y*800;
				}
			}
			else if(deltay == 0){
				// on incrémente x de 1
				while(current.x != arrivee.x){
					current.x += incrementx;
					pixel_ptr += current.x + current.y*800;
					*pixel_ptr = 0xba3030ff;
					pixel_ptr -= current.x + current.y*800;
				}
			}
			else if (deltax > deltay){
				// on incrémente x de 1
				while((current.x != arrivee.x) && (current.y != arrivee.y)){
					current.x += incrementx;

					// calcul de l'erreur
					erreur += (float)deltay/deltax;
					if (erreur > 0.5){
						current.y += incrementy;
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
					current.y += incrementy;

					// calcul de l'erreur
					erreur += (float)deltax/deltay;
					if (erreur > 0.5){
						current.x += incrementx;
						erreur--;
					}
					pixel_ptr += current.x + current.y*800;
					*pixel_ptr = 0xba3030ff;
					pixel_ptr -= current.x + current.y*800;
				}
			}
			current = arrivee;
			suiveur_arrivee = suiveur_arrivee->next;
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

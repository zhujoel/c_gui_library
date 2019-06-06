#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"


/**
 * test ei_draw_widget_with_relief_and_corner_radius_that_is_optional
 *
 * vérifie que la fonction dessine bien un bouton avec le relief et tout
 */
void test_draw_button(ei_surface_t surface, ei_rect_t* clipper){
		ei_color_t		color		= { 0, 255, 255, 50 };
		ei_point_t top_left = {100, 100};
		ei_size_t size = {200, 400};
		ei_rect_t	rect = {top_left, size};
		float rayon = 20;

		ei_draw_widget_with_relief_and_corner_radius_that_is_optional(surface, rect, color, NULL, rayon, 1, 50);
}


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t		clipper		= ei_rect(ei_point(0, 0), ei_size(800, 600));
	ei_rect_t* clipper_ptr		= &clipper;

	hw_init();

	main_window = hw_create_window(&win_size, EI_FALSE);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, NULL);

	/* Draw polylines. */
	test_draw_button(main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

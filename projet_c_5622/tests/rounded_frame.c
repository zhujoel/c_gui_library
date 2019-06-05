#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"


/* test rounded_frame
 *
 * vérifie que rounded_frame génère bien les points qui permettent de rounder les frames
 * sur un rectangle
 *
 */
void test_rounded_frame(ei_surface_t surface, ei_rect_t* clipper){
	ei_point_t topleft = {100, 100};
	ei_size_t size = {300, 300};
	ei_rect_t rectangle = {topleft, size};
	float rayon = 30;
	ei_bool_t* bords = malloc(sizeof(ei_bool_t)*4);
	bords[0] = 1;
	bords[1] = 1;
	bords[2] = 1;
	bords[3] = 1;

	ei_linked_point_t* rectangle_arrondis = rounded_frame(rectangle, rayon, bords);
	ei_color_t color = {0xff, 0x65, 0x44, 0xff};
	ei_draw_polygon(surface, rectangle_arrondis, color, clipper);
	free(rectangle_arrondis[0].next);
	free(rectangle_arrondis[1].next);
	free(rectangle_arrondis[2].next);
	free(rectangle_arrondis[3].next);
	free(rectangle_arrondis);
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

	test_rounded_frame(main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

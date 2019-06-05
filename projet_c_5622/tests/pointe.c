#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"


void test_pointe(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[23];

	pts[0].point.x = 100; pts[0].point.y = 100; pts[0].next = &pts[1];
	pts[1].point.x = 300; pts[1].point.y = 100; pts[1].next = &pts[2];
	pts[2].point.x = 200; pts[2].point.y = 500; pts[2].next = &pts[3];
	//pts[1].point.x = 200; pts[1].point.y = 500; pts[1].next = &pts[2];
	//pts[2].point.x = 300; pts[2].point.y = 100; pts[2].next = &pts[3];
	pts[3].point.x = 100; pts[3].point.y = 100; pts[3].next = NULL;


	ei_draw_polyline(surface, pts, color, clipper);

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

	test_pointe(main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

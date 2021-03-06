#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"

/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[23];

	pts[0].point.x = 100; pts[0].point.y = 500; pts[0].next = &pts[1];
	pts[1].point.x = 100; pts[1].point.y = 100; pts[1].next = &pts[2];
	pts[2].point.x = 200; pts[2].point.y = 100; pts[2].next = &pts[3];
	pts[3].point.x = 200; pts[3].point.y = 250; pts[3].next = &pts[4];
	pts[4].point.x = 100; pts[4].point.y = 250; pts[4].next = &pts[5];
	pts[5].point.x = 100; pts[5].point.y = 500; pts[5].next = &pts[6];
	pts[6].point.x = 300; pts[6].point.y = 500; pts[6].next = &pts[7];
	pts[7].point.x = 300; pts[7].point.y = 100; pts[7].next = &pts[8];
	pts[8].point.x = 400; pts[8].point.y = 100; pts[8].next = &pts[9];
	pts[9].point.x = 400; pts[9].point.y = 250; pts[9].next = &pts[10];
	pts[10].point.x = 300; pts[10].point.y = 250; pts[10].next = &pts[11];
	pts[11].point.x = 400; pts[11].point.y = 250; pts[11].next = &pts[12];
	pts[12].point.x = 400; pts[12].point.y = 500; pts[12].next = &pts[13];
	pts[13].point.x = 500; pts[13].point.y = 500; pts[13].next = &pts[14];
	pts[14].point.x = 500; pts[14].point.y = 100; pts[14].next = &pts[15];
	pts[15].point.x = 500; pts[15].point.y = 500; pts[15].next = &pts[16];
	pts[16].point.x = 600; pts[16].point.y = 500; pts[16].next = &pts[17];
	pts[17].point.x = 600; pts[17].point.y = 100; pts[17].next = &pts[18];
	pts[18].point.x = 600; pts[18].point.y = 500; pts[18].next = &pts[19];
	pts[19].point.x = 700; pts[19].point.y = 500; pts[19].next = &pts[20];
	pts[20].point.x = 700; pts[20].point.y = 100; pts[20].next = &pts[21];
	pts[21].point.x = 700; pts[21].point.y = 500; pts[21].next = &pts[22];
	pts[22].point.x = 799; pts[22].point.y = 500; pts[22].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);

}

/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 255, 0, 255 };
	ei_linked_point_t	pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 400;
	pts[0].point.y = 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
		ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	//ei_draw_polyline(surface, pts, color, clipper);
	ei_draw_polygon(surface, pts, color, clipper);
}

/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 0, 255 };
	ei_linked_point_t	pts[5];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 300;
	pts[0].point.y = 400;

	/* Draw the square */
	for(i = 1; i <= 4; i++) {
		/*	Add or remove 200 pixels or 0 for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 0 or 1, according to which coordinate grows
			The third term is simply the side of the square */
		xdiff = pow(-1, i / 2) * (i % 2) * 200;
		ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
	//ei_draw_polygon(surface, pts, color, clipper);
}

/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 0, 255 };
	ei_linked_point_t	pts[3];

	pts[0].point.x = 400; pts[0].point.y = 300; pts[0].next = &(pts[1]);
	pts[1].point.x = 400; pts[1].point.y = 300; pts[1].next = NULL;

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

	/* Draw polylines. */
	test_line	(main_window, clipper_ptr);
	//test_octogone	(main_window, clipper_ptr);
	//test_square	(main_window, clipper_ptr);
	//test_dot	(main_window, clipper_ptr);
	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

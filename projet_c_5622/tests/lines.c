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

	//ei_draw_polygon(surface, pts, color, clipper);

	ei_draw_polyline(surface, pts, color, clipper);

}

/* test ei_draw_text --
 * écrit un texte dans la fenetre
 */

void test_text(ei_surface_t surface, ei_rect_t* clipper){
	// écrit qlq chose à une position de l'écran
	char* text = "BIGUE TCHIZE";
	ei_fontstyle_t fontstyle = ei_style_normal;
	ei_font_t font = hw_text_font_create("fonts/BigCheese.ttf", fontstyle, 50);
	ei_point_t point = {200, 200};
	ei_color_t color = {0xff, 0xfa, 0x85, 0xff};
	ei_draw_text(surface, &point, text, font, &color, clipper);
	hw_text_font_free(font);
}

/* test ei_copy_surface --
 *
 * copie tous les pixels délimités par un rect d'une surface
 * source à une surface destination.
 * Test de la méthode avec rectangles source et destionation nulls
 */
void test_copy_null_null(ei_surface_t destination){

	/**
	 * CREATION DE LA SURFACE SOURCE
	 */
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		source	= NULL;
	ei_color_t		black		= { 0xff, 0xff, 0xff, 0xff };

	source = hw_surface_create(destination, &win_size, 0);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(source);
	ei_fill		(source, &black, NULL);

	test_line(source, NULL);

	/* Unlock and update the surface. */
	hw_surface_unlock(source);
	hw_surface_update_rects(source, NULL);

	/**
	 * DÉBUT DU TEST DE COPY
	 */
	ei_copy_surface(destination, NULL, source, NULL, 0);
}

/* test ei_copy_surface --
 *
 * copie tous les pixels délimités par un rect d'une surface
 * source à une surface destination.
 * Test de la méthode avec rectangle destination non null et rectangle source null
 */
void test_copy_rect_null(ei_surface_t destination){

	/**
	 * CREATION DE LA SURFACE SOURCE
	 */
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		source	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_point_t rect_top_left = {200, 200};
	ei_size_t rect_size = {400, 400};
	ei_rect_t rect_dest = {rect_top_left, rect_size};

	source = hw_surface_create(destination, &win_size, 0);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(source);
	ei_fill		(source, &white, NULL);

	test_line(source, NULL);

	/* Unlock and update the surface. */
	hw_surface_unlock(source);
	hw_surface_update_rects(source, NULL);

	/**
	 * DÉBUT DU TEST DE COPY
	 */
	ei_copy_surface(destination, &rect_dest, source, NULL, 0);
}

/* test ei_copy_surface --
 *
 * copie tous les pixels délimités par un rect d'une surface
 * source à une surface destination.
 * Test de la méthode avec rectangle destination null et rectangle source non null
 */
void test_copy_null_rect(ei_surface_t destination){

	/**
	 * CREATION DE LA SURFACE SOURCE
	 */
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		source	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_point_t rect_top_left = {300, 300};
	ei_size_t rect_size = {500, 300};
	ei_rect_t rect_src = {rect_top_left, rect_size};

	source = hw_surface_create(destination, &win_size, 0);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(source);
	ei_fill		(source, &white, NULL);

	test_line(source, NULL);

	/* Unlock and update the surface. */
	hw_surface_unlock(source);
	hw_surface_update_rects(source, NULL);

	/**
	 * DÉBUT DU TEST DE COPY
	 */
	ei_copy_surface(destination, NULL, source, &rect_src, 0);
}


/* test ei_copy_surface --
 *
 * copie tous les pixels délimités par un rect d'une surface
 * source à une surface destination.
 * Test de la méthode avec rectangle destination non null et rectangle source non null
 */
void test_copy_rect_rect(ei_surface_t destination){

	/**
	 * CREATION DE LA SURFACE SOURCE
	 */
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		source	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_point_t rect_dest_top_left = {100, 100};
	ei_size_t rect_dest_size = {300, 400};
	ei_rect_t rect_dest = {rect_dest_top_left, rect_dest_size};
	ei_point_t rect_src_top_left = {300, 200};
	ei_size_t rect_src_size = {300, 400};
	ei_rect_t rect_src = {rect_src_top_left, rect_src_size};

	source = hw_surface_create(destination, &win_size, 0);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(source);
	ei_fill		(source, &white, NULL);

	test_line(source, NULL);

	/* Unlock and update the surface. */
	hw_surface_unlock(source);
	hw_surface_update_rects(source, NULL);

	/**
	 * DÉBUT DU TEST DE COPY
	 */
	ei_copy_surface(destination, &rect_dest, source, &rect_src, 0);
}

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


	ei_draw_polygon(surface, pts, color, clipper);

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
	ei_draw_polyline(surface, pts, color, clipper);
	//ei_draw_polygon(surface, pts, color, clipper);
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

/* test ei_fill
 *
 * remplie une surface d'une couleur
 *
 */
void test_fill(ei_surface_t surface, ei_rect_t* clipper){
	ei_color_t		color		= { 0xde, 0xad, 0xbe, 0xef};
	ei_fill(surface, &color, clipper);
}

/* test arc
 *
 * vérifie les points générés par arc
 *
 */
void test_arc(ei_surface_t surface, ei_rect_t* clipper){

	ei_point_t centre = {50, 50};
	float rayon = 50;
	int angle_debut = 90;
	int angle_fin = 180;
	ei_linked_point_t* points = arc(centre, rayon, angle_debut, angle_fin);
	ei_color_t color = {0xff, 0x65, 0x44, 0xff};
	ei_draw_polyline(surface, points, color, clipper);
}
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
	bords[1] = 0;
	bords[2] = 0;
	bords[3] = 1;

	ei_linked_point_t* rectangle_arrondis = rounded_frame(rectangle, rayon, bords);
	ei_color_t color = {0xff, 0x65, 0x44, 0xff};
	ei_draw_polygon(surface, rectangle_arrondis, color, clipper);
}

/**
 * test ei_draw_button enfonced et releved
 *
 * vérifie que la fonction dessine bien un bouton avec le relief et tout
 */
void test_draw_button(ei_surface_t surface, ei_rect_t* clipper){
		ei_color_t		color		= { 0, 0, 255, 255 };
		ei_point_t top_left = {100, 100};
		ei_size_t size = {100, 500};
		ei_rect_t	rect = {top_left, size};
		float rayon = 20;

		ei_draw_button_enfonced(surface, rect, color, clipper, rayon);
		getchar();
		ei_draw_button_releved(surface, rect, color, clipper, rayon);
}

void test_benis(ei_surface_t surface){

		ei_color_t		color		= { 0, 0, 255, 255 };
		ei_point_t top_left = {200, 100};
		ei_size_t size = {100, 300};
		ei_rect_t	rect = {top_left, size};
		float rayon = 20;

		ei_point_t rond1_point = {125, 50};
		ei_linked_point_t* rond1 = arc(rond1_point, 30, 0, 360);
		ei_point_t rond2_point = {325, 50};
		ei_linked_point_t* rond2 = arc(rond2_point, 30, 0, 360);


		ei_color_t		color2		= { 255, 127, 127, 255 };
		ei_point_t top_left2 = {50, 300};
		ei_size_t size2 = {50, 200};
		ei_rect_t	rect2 = {top_left2, size2};

		ei_point_t top_left3 = {50, 450};
		ei_size_t size3 = {500, 100};
		ei_rect_t	rect3 = {top_left3, size3};

		ei_point_t top_left4 = {500, 300};
		ei_size_t size4 = {50, 200};
		ei_rect_t	rect4 = {top_left4, size4};

		ei_draw_button_enfonced(surface, rect, color, NULL, rayon);
		ei_draw_polygon(surface, rond1, color, NULL);
		ei_draw_polygon(surface, rond2, color, NULL);
		ei_draw_button_enfonced(surface, rect2, color2, NULL, rayon);
		ei_draw_button_enfonced(surface, rect3, color2, NULL, rayon);
		ei_draw_button_enfonced(surface, rect4, color2, NULL, rayon);
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
<<<<<<< HEAD
	ei_rect_t		clipper		= ei_rect(ei_point(0, 0), ei_size(800, 600));

	ei_rect_t* clipper_ptr		= &clipper;
=======

	ei_rect_t*		clipper_ptr	= NULL;
	ei_rect_t		clipper		= ei_rect(ei_point(0, 0), ei_size(800, 600));

	clipper_ptr		= &clipper;
>>>>>>> 0985c599bfa5c98a5f799d5f10d02a5f4d4b1ce2

	hw_init();

	main_window = hw_create_window(&win_size, EI_FALSE);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, NULL);

	/* Draw polylines. */
	//test_line	(main_window, clipper_ptr);
	//test_octogone	(main_window, clipper_ptr);
	//test_square	(main_window, clipper_ptr);
	//test_dot	(main_window, clipper_ptr);
	//test_text (main_window, clipper_ptr);
	//test_copy_null_null(main_window);
	//test_copy_null_rect(main_window);
	//test_copy_rect_null(main_window);
	//test_copy_rect_rect(main_window);
	//test_pointe(main_window, clipper_ptr);
	//test_fill(main_window, NULL);
	//test_arc (main_window, clipper_ptr);
	//test_rounded_frame(main_window, clipper_ptr);
	//test_draw_button(main_window, clipper_ptr);
	test_benis(main_window);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_quit();
	return (EXIT_SUCCESS);
}

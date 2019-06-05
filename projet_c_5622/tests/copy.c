#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"

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


	hw_init();

	main_window = hw_create_window(&win_size, EI_FALSE);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, NULL);

	//test_copy_null_null(main_window);
	//test_copy_null_rect(main_window);
	//test_copy_rect_null(main_window);
	test_copy_rect_rect(main_window);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

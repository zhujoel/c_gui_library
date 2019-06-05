#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"

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

  // Dessine un texte
	test_text (main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	getchar();

	hw_surface_free(main_window);
	hw_quit();
	return (EXIT_SUCCESS);
}

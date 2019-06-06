#include <stdio.h>
#include <stdlib.h>

#include "../include/ei_application.h"
#include "../include/ei_event.h"
#include "../include/hw_interface.h"
#include "../include/ei_widget.h"


ei_bool_t process_key(ei_event_t* event)
{
	if (event->type == ei_ev_keydown)
		if (event->param.key.key_sym == SDLK_ESCAPE) {
			ei_app_quit_request();
			return EI_TRUE;
		}

	return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {1920, 900};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {300,200};
	int		frame_x			= 0;
	int		frame_y			= 0;
	float		frame_rx	= 0.5;
	float		frame_ry  = 0.5;
	int		frame_w			= 0;
	int		frame_h			= 0;
	float		frame_wr		= 0.9;
	float		frame_hr		= 0.5;
	ei_anchor_t frame_anchor = ei_anc_center;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	char* text = "Fromage de chèvre";
	ei_fontstyle_t fontstyle = ei_style_normal;
	ei_color_t color = {0xff, 0xfa, 0x85, 0xff};
	ei_anchor_t text_anchor = ei_anc_northwest;
	//hw_text_font_free(font);

	static char* image_filename	= "misc/klimt.jpg";
	ei_surface_t image;
	ei_size_t image_size;
	ei_size_t image_rect_size;
	ei_anchor_t image_anchor = ei_anc_none;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	ei_font_t font = hw_text_font_create("fonts/BigCheese.ttf", fontstyle, 50);
	//image = hw_image_load(image_filename, ei_app_root_surface());
	//image_size	= hw_surface_get_size(image);

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure	(frame, &frame_size, &frame_color,
				 &frame_border_width, &frame_relief, &text, &font, &color, &text_anchor,
				 NULL, NULL, NULL);
	ei_place(frame, &frame_anchor, &frame_x, &frame_y, &frame_w, &frame_h, &frame_rx, &frame_ry, &frame_wr, &frame_hr );
	ei_event_set_default_handle_func(process_key);
	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}

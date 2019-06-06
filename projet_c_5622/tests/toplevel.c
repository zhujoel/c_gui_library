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
	ei_toplevel_register_class();
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	toplevel;
	ei_size_t	toplevel_size		= {300,200};
	int		toplevel_x			= 50;
	int		toplevel_y			= 50;
	float		toplevel_rx	= 0;
	float		toplevel_ry  = 0;
	int		toplevel_w			= 200;
	int		toplevel_h			= 300;
	float		toplevel_wr		= 0;
	float		toplevel_hr		= 0;
	ei_anchor_t toplevel_anchor = ei_anc_center;
	ei_color_t	toplevel_color		= {0x88, 0x88, 0x88, 0xff};
	int		toplevel_border_width	= 4;

	char* title = "Fromage de chèvre";
	//hw_text_font_free(font);

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	//image = hw_image_load(image_filename, ei_app_root_surface());
	//image_size	= hw_surface_get_size(image);

	/* Create, configure and place the toplevel on screen. */
	toplevel = ei_widget_create("toplevel", ei_app_root_widget());
	ei_toplevel_configure	(toplevel, &toplevel_size, &toplevel_color,
				 &toplevel_border_width, &title,
				 NULL, NULL, NULL);
	ei_place(toplevel, &toplevel_anchor, &toplevel_x, &toplevel_y, &toplevel_w, &toplevel_h, &toplevel_rx, &toplevel_ry, &toplevel_wr, &toplevel_hr );

	/* Run the application's main loop. */
	ei_event_set_default_handle_func(process_key);
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}

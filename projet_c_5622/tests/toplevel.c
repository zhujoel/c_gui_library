#include <stdio.h>
#include <stdlib.h>

#include "../include/ei_application.h"
#include "../include/ei_event.h"
#include "../include/hw_interface.h"
#include "../include/ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {1920, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	toplevel;
	ei_size_t	toplevel_size		= {300,200};
	int		toplevel_x			= 0;
	int		toplevel_y			= 0;
	float		toplevel_rx	= 0.5;
	float		toplevel_ry  = 0.5;
	int		toplevel_w			= 0;
	int		toplevel_h			= 0;
	float		toplevel_wr		= 0.9;
	float		toplevel_hr		= 0.5;
	ei_anchor_t toplevel_anchor = ei_anc_center;
	ei_color_t	toplevel_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	toplevel_relief		= ei_relief_raised;
	int		toplevel_border_width	= 6;

	char* title = "Fromage de chèvre";
	ei_color_t color = {0xff, 0xfa, 0x85, 0xff};
	//hw_text_font_free(font);

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	printf("config\n");
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	printf("config2\n");
	//image = hw_image_load(image_filename, ei_app_root_surface());
	//image_size	= hw_surface_get_size(image);

	/* Create, configure and place the toplevel on screen. */
	toplevel = ei_widget_create("toplevel", ei_app_root_widget());
	printf("config\n");
	ei_toplevel_configure	(toplevel, &toplevel_size, &toplevel_color,
				 &toplevel_border_width, &title,
				 NULL, NULL, NULL);
	ei_place(toplevel, &toplevel_anchor, &toplevel_x, &toplevel_y, &toplevel_w, &toplevel_h, &toplevel_rx, &toplevel_ry, &toplevel_wr, &toplevel_hr );

	/* Run the application's main loop. */
	printf("RUN\n");
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}

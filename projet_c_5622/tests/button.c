#include <stdio.h>
#include <stdlib.h>

#include "../include/ei_application.h"
#include "../include/ei_event.h"
#include "../include/hw_interface.h"
#include "../include/ei_widget.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if(user_param != NULL){
		int* param = (int*)user_param;
		printf("Click ! %i \n", *param);
	}
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
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
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	button;
	ei_size_t	button_size		= {300,300};
	int		button_corner_radius	= 40;
	int		button_x		= 120;
	int		button_y		= 100;
	int button_w = 190;
	int button_h = 200;

	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "Mon premier Bouton !";
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_relief_t	button_relief		= ei_relief_raised;
	ei_anchor_t button_text_anchor = ei_anc_center;
	int		button_border_width	= 6;
	ei_callback_t	button_callback 	= button_press;
	int button_user_param = 1;
	void* user_param = &button_user_param;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	ei_event_set_default_handle_func(process_key);

	// TODO: (joel) crée l'image pour le bouton
	char* image_filename	= "misc/klimt.jpg";
	ei_surface_t image = hw_image_load(image_filename, ei_app_root_surface());
	ei_anchor_t button_image_anchor = ei_anc_center;
	ei_rect_t image_rect = hw_surface_get_rect(image);
	ei_rect_t *image_rect2 = &image_rect;

	/* Create, configure and place the button on screen. */
	button = ei_widget_create("button", ei_app_root_widget());

	// TODO: (joel) bouton avec une image
	// ei_button_configure	(button, &button_size, &button_color,
	// 			 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
	// 			 &image, &image_rect2, &button_image_anchor, &button_callback, NULL);

	// TODO: (joel) bouton avec un texte
	ei_button_configure	(button, &button_size, &button_color,
	 			 &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, &button_text_anchor,
	 			 NULL, NULL, NULL, &button_callback, &user_param);
	ei_place(button, NULL, &button_x, &button_y, &button_w, &button_h, NULL, NULL, NULL, NULL);


	char*		button_title2		= "FROMAGE DE CHEVRE INCROYABLE Woooo";
	ei_button_configure	(button, NULL, NULL, NULL, NULL, NULL, &button_title2, NULL, NULL, NULL,
	 			 NULL, NULL, NULL, NULL, NULL);

	/* Hook the keypress callback to the event. */
	ei_frame_configure(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}

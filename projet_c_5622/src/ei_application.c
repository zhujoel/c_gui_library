#include "../include/ei_application.h"
#include "../include/ei_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_widget_t* root;
static ei_surface_t main_window	= NULL;
static ei_surface_t picking_surface = NULL;
static ei_bool_t continuer = EI_TRUE;

extern ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent);

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
  //Initialize the hardware
  hw_init();

  //Register widgets
  ei_frame_register_class();
  ei_button_register_class();
  ei_toplevel_register_class();
  //...

  //Creates the root window
  main_window =	hw_create_window(main_window_size, fullscreen);
  picking_surface = hw_surface_create(main_window, main_window_size, EI_TRUE);

  //Creates the root widget
  root = ei_widget_create_root("frame", NULL);

  ei_frame_configure(ei_app_root_widget(), main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  ei_place(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_free(){
  hw_quit();
}

void parcours_profondeur(ei_widget_t* widget){
  printf("PARCOURS ------\n");
  // printf("%i\n", widget->screen_location.top_left.x);
  // printf("%i\n", widget->screen_location.top_left.y);
  // printf("%i\n", widget->screen_location.size.width);
  // printf("%i\n", widget->screen_location.size.height);
  printf("W DATA %i\n", widget->placer_params->w_data);
  widget->wclass->drawfunc(widget, ei_app_root_surface(), picking_surface, &widget->screen_location);//widget->content_rect);

  ei_widget_t** courant = malloc(sizeof(ei_widget_t));
  *courant = widget->children_head;

  while (*courant != NULL)
  {
    parcours_profondeur(*courant);
    *courant = (*courant)->next_sibling;
  }
  printf("FIN W DATA %i\n", widget->placer_params->w_data);
}

void ei_app_run(){
    //TODO : Parcours de la hiérarchie de widget
    printf("PARCROUS PROFONDEUR ------\n");
    parcours_profondeur(ei_app_root_widget());
    hw_surface_update_rects(ei_app_root_surface(), NULL);

    //parcours_profondeur(ei_app_root_widget());
    /*struct ei_event_t* event = malloc(sizeof(struct ei_event_t*));
    parcours_profondeur(ei_app_root_widget(), clipper_ptr);
    while (continuer)
    {
      parcours_profondeur(ei_app_root_widget(), clipper_ptr);
      hw_event_wait_next(event);
      if ()
      {

      }
    }*/
    //parcours_profondeur(ei_app_root_widget(), clipper_ptr);
  /* Wait for a character on command line. */

}

void ei_app_invalidate_rect(ei_rect_t* rect){

}

void ei_app_quit_request(){
  continuer = EI_FALSE;
}

ei_widget_t* ei_app_root_widget(){
  return root;
}

ei_surface_t ei_app_root_surface(){
  return main_window;
}

#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_widget_t* root;
static ei_surface_t main_window	= NULL;

extern ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent);

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
  //Initialize the hardware
  hw_init();

  //Register widgets
  ei_frame_register_class();
  //ei_button_register_class();
  //ei_toplevel_register_class();
  //...

  //Creates the root window
  main_window =	hw_create_window(main_window_size, fullscreen);

  //Creates the root widget
  root = ei_widget_create_root("frame", NULL);

  ei_frame_configure(ei_app_root_widget(), main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  ei_place(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_free(){
  hw_quit();
}

void parcours_profondeur(ei_widget_t* widget, ei_rect_t* clipper){
  widget->wclass->drawfunc(widget, ei_app_root_surface(), ei_app_root_surface(), clipper);
  ei_widget_t* courant = widget->children_head;
  while (courant != NULL)
  {
    parcours_profondeur(courant, clipper);
    courant = courant->next_sibling;
  }
}

void ei_app_run(){
    //TODO : Parcours de la hiérarchie de widget
    ei_rect_t* clipper_ptr	= NULL;

    parcours_profondeur(ei_app_root_widget(), clipper_ptr);
  /* Wait for a character on command line. */
	getchar();
}

void ei_app_invalidate_rect(ei_rect_t* rect){

}

void ei_app_quit_request(){
  printf("Je ferme");
}

ei_widget_t* ei_app_root_widget(){
  return root;
}

ei_surface_t ei_app_root_surface(){
  return main_window;
}

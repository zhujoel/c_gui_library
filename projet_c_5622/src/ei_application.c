#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
  //Initialize the hardware
  hw_init();

  //Register widgets
  ei_frame_register_class();
  //ei_button_register_class();
  //ei_toplevel_register_class();
  //...

  //Creates the root window
	ei_surface_t main_window = NULL;
	main_window = hw_create_window(main_window_size, fullscreen);

  //Creates the root widget
  ei_widget_t widget_root;
  ei_widget_create("frame", &widget_root);

}

void ei_app_free(){
  hw_quit();
}

void ei_app_run(){
  //TODO : Parcours de la hiérarchie de widget
  while()
  ei_widget_t* current = ei_app_root_widget()->children_head;

  //call drawfunc

  /* Wait for a character on command line. */
	getchar();
}

void ei_app_invalidate_rect(ei_rect_t* rect){

}

void ei_app_quit_request(){

}

ei_widget_t* ei_app_root_widget(){
  return widget_root;
}

ei_surface_t ei_app_root_surface(){

}

#include "../include/ei_widget.h"
#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//Other widgets


//struct ei_widget_frame_t frame_widget;
//


//create a widget regardless of the parent, can be NULL (to create the root widget)
ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent){
    ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
    if(widgetclass == NULL){
      //The class_name dosn't exist
      fprintf(stderr, "THIS CLASS NAME DOES NOT EXIST DUDE");
      return NULL;
    }

    struct ei_widget_t* new_widget = widgetclass->allocfunc();

    new_widget->wclass = widgetclass;
    new_widget->pick_id = 0;
    //Hierarchy
    new_widget->parent = parent;

    new_widget->wclass->setdefaultsfunc(new_widget);

    return new_widget;
}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name, ei_widget_t* parent){
  if(parent != NULL){
    ei_widget_create_root(class_name, parent);
  }
  //Parent is null
  fprintf(stderr, "A widget parent cannot be NULL, DUMBASS");
  return NULL;
}

void ei_widget_destroy (ei_widget_t* widget){

}

ei_widget_t* ei_widget_pick (ei_point_t* where){

}

void ei_frame_configure (ei_widget_t* widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, ei_relief_t* relief, char** text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t* img_anchor){
  ei_widget_frame_t* widgetframe;
  widgetframe->
  (struct ei_widget_frame_t*)widget->bob = 2;
}

void ei_button_configure (ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 int*			corner_radius,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor,
							 ei_callback_t*		callback,
							 void**			user_param){

}

void ei_toplevel_configure (ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size){

}

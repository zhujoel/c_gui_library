#include "../include/ei_widget.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ei_widget_frame_t {
  ei_widget_t widget;

  //Specific attributes
  const ei_color_t color;
  //......

} ei_maclasse_t;

//Other widgets


ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name, ei_widget_t* parent){
  if(strcmp(ei_widgetclass_stringname(class_name), "frame") == 0){
    ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
    widgetclass->allocfunc();

    struct ei_widget_frame_t frame_widget;
    (ei_widget_t*)&frame_widget

  }else{
    //The class_name dosn't exist
    return NULL
  }
}

void ei_widget_destroy (ei_widget_t* widget){

}

ei_widget_t* ei_widget_pick (ei_point_t* where){

}

void ei_frame_configure (ei_widget_t* widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, ei_relief_t* relief, char** text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t* img_anchor){
  ddsfgdf
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

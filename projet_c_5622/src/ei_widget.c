#include "../include/ei_widget.h"
#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//Other widgets


//struct ei_widget_frame_t frame_widget;
typedef struct ei_widget_frame_t {
  ei_widget_t widget;

  //Specific attributes
  ei_relief_t* relief;
  char**  text;
  ei_font_t* text_font;
  ei_color_t* text_color;
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_t** img_rect;
  ei_anchor_t* img_anchor;

} ei_frame_t;

typedef struct ei_widget_button_t {
  ei_widget_t widget;

  //Specific attributes
  ei_relief_t* relief;
  char** text;
  ei_font_t* text_font;
  ei_color_t* text_color;
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_t** img_rect;
  ei_anchor_t* img_anchor;
  ei_callback_t* callback;
  void** user_param;

} ei_widget_button_t;

typedef struct ei_widget_toplevel_t {
  ei_widget_t widget;

  //Specific attributes
  char** title;
  ei_bool_t* closable;
  ei_axis_set_t* resizable;
  ei_size_t** min_size;

} ei_widget_toplevel_t;


//create a widget regardless of the parent, can be NULL (to create the root widget)
ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent){
    ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
    if(widgetclass == NULL){
      //The class_name dosn't exist
      fprintf(stderr, "THIS CLASS NAME DOES NOT EXIST DUDE");
      return NULL;
    }
    printf("Widget : %s\n", class_name);

    struct ei_widget_t* new_widget = widgetclass->allocfunc();

    new_widget->wclass = widgetclass;
    new_widget->pick_id = 0;
    //Hierarchy
    new_widget->parent = parent;

    new_widget->wclass->setdefaultsfunc(new_widget);

    return new_widget;
}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name, ei_widget_t* parent){
  printf("BOB : %s\n", parent->wclass->name);
  if(parent != NULL){
    ei_widget_create_root(class_name, parent);
  }
  //Parent is null
  fprintf(stderr, "A widget parent cannot be NULL !");
  return NULL;
}

void ei_widget_destroy (ei_widget_t* widget){

}

ei_widget_t* ei_widget_pick (ei_point_t* where){
  return NULL;
}

void ei_frame_configure (ei_widget_t* widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, ei_relief_t* relief, char** text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t* img_anchor){
  struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
  if(requested_size != NULL){
    widgetframe->widget.requested_size = *requested_size;
  }
  if(color != NULL){

  }
  if(border_width != NULL){

  }
  if(relief != NULL){
    widgetframe->relief = relief;
  }
  if(text != NULL){
    widgetframe->text = text;
  }
  if(text_font != NULL){
    widgetframe->text_font = text_font;
  }
  if(text_color != NULL){
    widgetframe->text_color = text_color;
  }
  if(text_anchor != NULL){
    widgetframe->text_anchor = text_anchor;
  }
  if(img != NULL){
    widgetframe->img = img;
  }
  if(img_rect != NULL){
    widgetframe->img_rect = img_rect;
  }
  if(img_anchor != NULL){
    widgetframe->img_anchor = img_anchor;
  }
}

void ei_button_configure (ei_widget_t* widget, ei_size_t* requested_size, const ei_color_t*	color, int* border_width, int* corner_radius, ei_relief_t* relief, char** text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_t** img_rect, ei_anchor_t* img_anchor, ei_callback_t* callback, void** user_param){
  struct ei_widget_button_t* widgetbutton;
  widgetbutton = (struct ei_widget_button_t*)widget;
  widgetbutton->widget.requested_size = *requested_size;
  widgetbutton->relief = relief;
  widgetbutton->text = text;
  widgetbutton->text_font = text_font;
  widgetbutton->text_color = text_color;
  widgetbutton->text_anchor = text_anchor;
  widgetbutton->img = img;
  widgetbutton->img_rect = img_rect;
  widgetbutton->img_anchor = img_anchor;
  widgetbutton->callback = callback;
  widgetbutton->user_param = user_param;
}

void ei_toplevel_configure (ei_widget_t* widget, ei_size_t* requested_size, ei_color_t* color, int* border_width, char** title, ei_bool_t* closable, ei_axis_set_t* resizable, ei_size_t** min_size){
  struct ei_widget_toplevel_t* widgettoplevel;
  widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  widgettoplevel->widget.requested_size = *requested_size;
  widgettoplevel->title = title;
  widgettoplevel->closable = closable;
  widgettoplevel->resizable = resizable;
  widgettoplevel->min_size = min_size;
}

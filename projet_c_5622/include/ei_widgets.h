#ifndef EI_WIDGETS_H
#define EI_WIDGETS_H

#include "../include/ei_widget.h"

//Widget FRAME
typedef struct ei_widget_frame_t {
  ei_widget_t widget;

  //Specific attributes
  ei_color_t	color;
  int border_width;
  ei_relief_t relief;
  char*  text;
  ei_font_t text_font;
  ei_color_t text_color;
  ei_anchor_t text_anchor;
  ei_surface_t img;
  ei_rect_t* img_rect;
  ei_anchor_t img_anchor;

} ei_widget_frame_t;

//Widget BUTTON
typedef struct ei_widget_button_t {
  ei_widget_t widget;

  //Specific attributes
  ei_color_t	color;
  int border_width;
  int corner_radius;
  ei_relief_t relief;
  char* text;
  ei_font_t text_font;
  ei_color_t text_color;
  ei_anchor_t text_anchor;
  ei_surface_t img;
  ei_rect_t* img_rect;
  ei_anchor_t img_anchor;
  ei_callback_t callback;
  void* user_param;

} ei_widget_button_t;

//Widget TOPLEVEL
typedef struct ei_widget_toplevel_t {
  ei_widget_t widget;

  //Specific attributes
  ei_color_t color;
  int border_width;
  char* title;
  ei_bool_t closable;
  ei_axis_set_t resizable;
  ei_size_t* min_size;

} ei_widget_toplevel_t;

#endif

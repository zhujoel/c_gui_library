#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ei_widget_frame_t {
  ei_widget_t widget;

  //Specific attributes
  const ei_color_t*	color;
  int* border_width;
  ei_relief_t relief;
  char**  text;
  ei_font_t* text_font;
  ei_color_t* text_color;
  ei_anchor_t* text_anchor;
  ei_surface_t* img;
  ei_rect_t** img_rect;
  ei_anchor_t* img_anchor;

} ei_widget_frame_t;

typedef struct ei_widget_button_t {
  ei_widget_t widget;

  //Specific attributes
  const ei_color_t*	color;
  int* border_width;
  int* corner_radius;
  ei_relief_t relief;
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
  ei_color_t* color;
  int* border_width;
  char** title;
  ei_bool_t* closable;
  ei_axis_set_t* resizable;
  ei_size_t** min_size;

} ei_widget_toplevel_t;


static ei_widgetclass_t* widgetsclass = NULL;

void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
  if(widgetsclass == NULL){
    widgetsclass = widgetclass;
  }else{
    //Get the last widggetclass
    ei_widgetclass_t* current = widgetsclass;
    while(current->next != NULL){
      current = current->next;
    }
    //Add the new widgetclass
    current->next = widgetclass;
  }
}

ei_widgetclass_t*	ei_widgetclass_from_name (ei_widgetclass_name_t name){
  ei_widgetclass_t* current = widgetsclass;
  if(current != NULL){
    while(current->next != NULL){
      if(strcmp(current->name, name)==0){
        return current;
      }
      current = current->next;
    }
    if(strcmp(current->name, name)==0){
      return current;
    }
  }
  return NULL;
}

///////////////////////
//Widget frame
//////////////////////
void* frame_allocfunc(){
  return malloc(sizeof(struct ei_widget_frame_t));
}

void frame_releasefunc(struct ei_widget_t* widget){

}

void frame_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "frame" */
  struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;

  ei_linked_point_t	pts[5];
	pts[0].point.x = 100; pts[0].point.y = 500; pts[0].next = &pts[1];
	pts[1].point.x = 100; pts[1].point.y = 100; pts[1].next = &pts[2];
	pts[2].point.x = 200; pts[2].point.y = 100; pts[2].next = &pts[3];
	pts[3].point.x = 200; pts[3].point.y = 250; pts[3].next = &pts[4];
	pts[4].point.x = 100; pts[4].point.y = 250; pts[4].next = NULL;
  ei_draw_polygon (surface, pts, *widgetframe->color, clipper);
}

void frame_setdefaultsfunc(struct ei_widget_t* widget){
  struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgetframe->widget.requested_size = screen_size;
  widgetframe->color = &default_color;
  widgetframe->border_width = 0;
  widgetframe->relief = ei_relief_none;
  widgetframe->text = NULL;
  widgetframe->text_font = ei_default_font;
  widgetframe->text_color = &default_color;
  ei_anchor_t anchor_center = ei_anc_center;
  widgetframe->text_anchor = &anchor_center;
  widgetframe->img = NULL;
  widgetframe->img_rect = NULL;
  widgetframe->img_anchor = &anchor_center;
}

void frame_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t frame_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}

///////////////////////
//Widget button
///////////////////////
void* button_allocfunc(){
  return malloc(sizeof(struct ei_widget_button_t));
}

void button_releasefunc(struct ei_widget_t* widget){
}

void button_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "button" */

}

void button_setdefaultsfunc(struct ei_widget_t*	widget){
  struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgetbutton->widget.requested_size = screen_size;
  widgetbutton->color = &default_color;
  widgetbutton->relief = ei_relief_none;
  widgetbutton->text = NULL;
  widgetbutton->text_font = ei_default_font;
  widgetbutton->text_color = &default_color;
  ei_anchor_t anchor_center = ei_anc_center;
  widgetbutton->text_anchor = &anchor_center;
  widgetbutton->img = NULL;
  widgetbutton->img_rect = NULL;
  widgetbutton->img_anchor = &anchor_center;

  int corner_radius = k_default_button_corner_radius;
  int border_width = k_default_button_border_width;
  widgetbutton->corner_radius = &corner_radius;
  widgetbutton->border_width = &border_width;
}

void button_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t button_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}

///////////////////////
//Widget toplevel
//////////////////////
void* toplevel_allocfunc(){
  return malloc(sizeof(struct ei_widget_toplevel_t));
}

void toplevel_releasefunc(struct ei_widget_t* widget){

}

void toplevel_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "toplevel" */

}

void toplevel_setdefaultsfunc(struct ei_widget_t*	widget){
  struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgettoplevel->widget.requested_size = screen_size;
  widgettoplevel->color = &default_color;
  widgettoplevel->border_width = 4;
  widgettoplevel->title = "TopLevel";
  widgettoplevel->closable
  widgettoplevel->resizable
  widgettoplevel->min_size
}

void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}


//registers
void ei_frame_register_class (){
  ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
  //ei_widgetclass_name_t* name = malloc(5 * sizeof(char));
  strcpy(frame->name, "frame");
  frame->allocfunc = &frame_allocfunc;
  frame->releasefunc = &frame_releasefunc;
  frame->drawfunc = &frame_drawfunc;
  frame->setdefaultsfunc = &frame_setdefaultsfunc;
  frame->geomnotifyfunc = &frame_geomnotifyfunc;
  frame->handlefunc = &frame_handlefunc;
  frame->next = NULL;
  ei_widgetclass_register(frame);
}

void ei_button_register_class (){
  ei_widgetclass_t button;
  button.allocfunc = &button_allocfunc;
  button.releasefunc = &button_releasefunc;
  button.drawfunc = &button_drawfunc;
  button.setdefaultsfunc = &button_setdefaultsfunc;
  button.geomnotifyfunc = &button_geomnotifyfunc;
  button.handlefunc = &button_handlefunc;
  button.next = NULL;
  ei_widgetclass_register(&button);
}

void ei_toplevel_register_class (){
  ei_widgetclass_t toplevel;
  toplevel.allocfunc = &toplevel_allocfunc;
  toplevel.releasefunc = &toplevel_releasefunc;
  toplevel.drawfunc = &toplevel_drawfunc;
  toplevel.setdefaultsfunc = &toplevel_setdefaultsfunc;
  toplevel.geomnotifyfunc = &toplevel_geomnotifyfunc;
  toplevel.handlefunc = &toplevel_handlefunc;
  toplevel.next = NULL;
  ei_widgetclass_register(&toplevel);
}

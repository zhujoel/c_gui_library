#include "../include/ei_widgetclass.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
  //JE COMPRENDS PAS
  widgetclass
}

ei_widgetclass_t*	ei_widgetclass_from_name (ei_widgetclass_name_t name){

}

///////////////////////
//Widget frame
//////////////////////
void frame_allocfunc(){
  //malloc()
}

void frame_releasefunc(struct ei_widget_t* widget){

}

void frame_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "frame" */

}

void frame_setdefaultsfunc(struct ei_widget_t*	widget){

}

void frame_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t frame_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}

///////////////////////
//Widget button
///////////////////////
void button_allocfunc(){
  //malloc()
}

void button_releasefunc(struct ei_widget_t* widget){

}

void button_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "button" */

}

void button_setdefaultsfunc(struct ei_widget_t*	widget){

}

void button_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t button_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}

///////////////////////
//Widget toplevel
//////////////////////
void toplevel_allocfunc(){
  //malloc()
}

void toplevel_releasefunc(struct ei_widget_t* widget){

}

void toplevel_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "toplevel" */

}

void toplevel_setdefaultsfunc(struct ei_widget_t*	widget){

}

void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}


//registers
void ei_frame_register_class (){
  ei_widgetclass_t frame;
  //ei_widgetclass_name_t name = "";
  //frame.name = name;
  frame.allocfunc = &frame_allocfunc;
  frame.releasefunc = &frame_releasefunc;
  frame.drawfunc = &frame_drawfunc;
  frame.setdefaultsfunc = &frame_setdefaultsfunc;
  frame.geomnotifyfunc = &frame_geomnotifyfunc;
  frame.handlefunc = &frame_handlefunc;
  ei_widgetclass_register(&frame);
}

void ei_button_register_class (){
  ei_widgetclass_t button;
  button.allocfunc = &button_allocfunc;
  button.releasefunc = &button_releasefunc;
  button.drawfunc = &button_drawfunc;
  button.setdefaultsfunc = &button_setdefaultsfunc;
  button.geomnotifyfunc = &button_geomnotifyfunc;
  button.handlefunc = &button_handlefunc;
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
  ei_widgetclass_register(&toplevel);
}

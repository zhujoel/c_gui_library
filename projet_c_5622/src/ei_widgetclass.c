#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_widgets.h"
#include "../include/ei_utils.h"
//#include "../include/ei_placer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void apply_anchor(ei_anchor_t anchor, int* x, int* y, int* w, int* h){
  switch (anchor) {
    case 0: //No anchor
      break;
    case 1: //Anchor center
      *x -= *w / 2;
      *y -= *h / 2;
      break;
    case 2: //Anchor north
      *x -= *w / 2;
      break;
    case 3: //Anchor north east
      *x -= *w;
      break;
    case 4: //Anchor east
      *x -= *w;
      *y -= *h / 2;
      break;
    case 5: //Anchor southeast
      *x -= *w;
      *y -= *h;
      break;
    case 6: //Anchor south
      *x -= *w / 2;
      *y -= *h;
      break;
    case 7: //Anchor southwest
      *y -= *h;
      break;
    case 8: //Anchor west
      *y -= *h / 2;
      break;
    case 9: //Anchor northwest
      break;
  }
}

void apply_anchor_text(ei_anchor_t anchor, int* x, int* y, int* w, int* h, int* w_parent, int* h_parent){
  switch (anchor) {
    case 0: //No anchor
      break;
    case 1: //Anchor center
      *x += (*w_parent / 2) - (*w / 2);
      *y += (*h_parent / 2) - (*h / 2);
      break;
    case 2: //Anchor north
      *x += *w_parent / 2 - *w / 2;
      break;
    case 3: //Anchor north east
      *x += *w_parent - *w;
      break;
    case 4: //Anchor east
      *x += *w_parent - *w;
      *y += *h_parent / 2 - *h / 2;
      break;
    case 5: //Anchor southeast
      *x += *w_parent - *w;
      *y += *h_parent - *h;
      break;
    case 6: //Anchor south
      *x += *w_parent / 2 - *w / 2;
      *y += *h_parent - *h;
      break;
    case 7: //Anchor southwest
      *y += *h_parent - *h;
      break;
    case 8: //Anchor west
      *y += *h / 2 - *h / 2;
      break;
    case 9: //Anchor northwest
      break;
  }
}

///////////////////////
//Widget frame
//////////////////////
void* frame_allocfunc(){
  return malloc(sizeof(struct ei_widget_frame_t));
}

void frame_releasefunc(struct ei_widget_t* widget){
  free(widget);
}

void frame_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "frame" */
  if(widget->placer_params != NULL){
    printf("Widget FRAME PLACE !\n");
    struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;

    ei_placer_params_t* placer_params = widget->placer_params;

    int x = placer_params->x_data;
    int y = placer_params->y_data;
    int w = placer_params->w_data;
    int h = placer_params->h_data;


    if(widget->parent != NULL){
      x = x + (placer_params->rx_data * widget->parent->placer_params->w_data);
      y = y + (placer_params->ry_data * widget->parent->placer_params->h_data);
      w = w + (placer_params->rw_data * widget->parent->placer_params->w_data);
      h = h + (placer_params->rh_data * widget->parent->placer_params->h_data);
    }

    apply_anchor(placer_params->anchor_data, &x, &y, &w, &h);

    ei_linked_point_t	pts[5];
    pts[0].point.x = x; pts[0].point.y = y; pts[0].next = &pts[1];
    pts[1].point.x = x + w; pts[1].point.y = y; pts[1].next = &pts[2];
    pts[2].point.x = x + w; pts[2].point.y = y + h; pts[2].next = &pts[3];
    pts[3].point.x = x; pts[3].point.y = y + h; pts[3].next = &pts[4];
    pts[4].point.x = x; pts[4].point.y = y; pts[4].next = NULL;

    printf("TEST\n");
    ei_draw_polygon (surface, pts, *widgetframe->color, clipper);

    printf("TEST\n");
    if (widgetframe->img != NULL){
      printf("Draw frame IMAGE\n");
      //Draw the image frame
      //ei_copy_surface(surface, NULL, widgetframe->img, NULL, EI_TRUE);
    }else if (widgetframe->text != NULL){
      // Draw the frame text
      printf("Draw frame Text : %s\n", *widgetframe->text);

      //Frame Clipper
      ei_point_t point = ei_point(x, y);
      ei_size_t size = ei_size(w, h);
      ei_rect_t clipper = ei_rect(point, size);

      //Point
      int x_text = x;
      int y_text = y;
      int w_text;
      int h_text;
      hw_text_compute_size (*widgetframe->text, widgetframe->text_font, &w_text, &h_text);

      apply_anchor_text(*widgetframe->text_anchor, &x_text, &y_text, &w_text, &h_text, &w, &h);

      ei_point_t text_point = ei_point(x_text, y_text);

    	ei_draw_text(surface, &text_point, *widgetframe->text, widgetframe->text_font, widgetframe->text_color, &clipper);
    }


    printf("TEST\n");
  }else{
    printf("Avoid printing frame widget\n");
  }
}

void frame_setdefaultsfunc(struct ei_widget_t* widget){
  struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgetframe->widget.requested_size = screen_size;
  widgetframe->color = &default_color;
  widgetframe->border_width = 0;
  ei_relief_t default_relief = ei_relief_none;
  widgetframe->relief = &default_relief;
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
  free(widget);
}

void button_drawfunc (struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  /* implémentation du dessin d’un widget de la classe "button" */
  if(widget->placer_params != NULL){
    printf("Widget BUTTON PLACE !\n");
    struct ei_widget_frame_t* widgetbutton = (struct ei_widget_frame_t*)widget;

    ei_placer_params_t* placer_params = widget->placer_params;

    int x = placer_params->x_data;
    int y = placer_params->y_data;
    int w = placer_params->w_data;
    int h = placer_params->h_data;

    if(widget->parent != NULL){
      x = x + (placer_params->rx_data * widget->parent->placer_params->w_data);
      y = y + (placer_params->ry_data * widget->parent->placer_params->h_data);
      w = w + (placer_params->rw_data * widget->parent->placer_params->w_data);
      h = h + (placer_params->rh_data * widget->parent->placer_params->h_data);
    }

    apply_anchor(placer_params->anchor_data, &x, &y, &w, &h);

    ei_linked_point_t	pts[5];
    pts[0].point.x = x; pts[0].point.y = y; pts[0].next = &pts[1];
    pts[1].point.x = x + w; pts[1].point.y = y; pts[1].next = &pts[2];
    pts[2].point.x = x + w; pts[2].point.y = y + h; pts[2].next = &pts[3];
    pts[3].point.x = x; pts[3].point.y = y + h; pts[3].next = &pts[4];
    pts[4].point.x = x; pts[4].point.y = y; pts[4].next = NULL;

    //ei_draw_button(surface, pts, *widgetbutton->color, clipper);

    if (widgetbutton->img != NULL){
      printf("Draw frame IMAGE\n");
      //Draw the image frame
      //ei_copy_surface(surface, NULL, widgetframe->img, NULL, EI_TRUE);
    }else if (widgetbutton->text != NULL){
      // Draw the frame text
      printf("Draw frame Text : %s\n", *widgetbutton->text);

      //Frame Clipper
      ei_point_t point = ei_point(x, y);
      ei_size_t size = ei_size(w, h);
      ei_rect_t clipper = ei_rect(point, size);

      //Point
      int x_text = x;
      int y_text = y;
      int w_text;
      int h_text;
      hw_text_compute_size (*widgetbutton->text, widgetbutton->text_font, &w_text, &h_text);

      apply_anchor_text(*widgetbutton->text_anchor, &x_text, &y_text, &w_text, &h_text, &w, &h);

      ei_point_t text_point = ei_point(x_text, y_text);

    	ei_draw_text(surface, &text_point, *widgetbutton->text, widgetbutton->text_font, widgetbutton->text_color, &clipper);
    }
  }else{
    printf("Avoid printing Button widget\n");
  }
}

void button_setdefaultsfunc(struct ei_widget_t*	widget){
  struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgetbutton->widget.requested_size = screen_size;
  widgetbutton->color = &default_color;
  ei_relief_t default_relief = ei_relief_none;
  widgetbutton->relief = &default_relief;
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
  free(widget);
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
  int default_width = 4;
  widgettoplevel->border_width = &default_width;
  strcpy(*widgettoplevel->title, "TopLevel");
  ei_bool_t closable = EI_TRUE;
  widgettoplevel->closable = &closable;
  ei_axis_set_t axis = ei_axis_both;
  widgettoplevel->resizable = &axis;
  ei_size_t	min_size = {160, 120};
  *widgettoplevel->min_size = &min_size;
}

void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  return EI_FALSE;
}


//registers
void ei_frame_register_class (){
  ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));

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
  ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));

  strcpy(button->name, "button");
  button->allocfunc = &button_allocfunc;
  button->releasefunc = &button_releasefunc;
  button->drawfunc = &button_drawfunc;
  button->setdefaultsfunc = &button_setdefaultsfunc;
  button->geomnotifyfunc = &button_geomnotifyfunc;
  button->handlefunc = &button_handlefunc;
  button->next = NULL;
  ei_widgetclass_register(button);
}

void ei_toplevel_register_class (){
  ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));

  strcpy(toplevel->name, "toplevel");
  toplevel->allocfunc = &toplevel_allocfunc;
  toplevel->releasefunc = &toplevel_releasefunc;
  toplevel->drawfunc = &toplevel_drawfunc;
  toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
  toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
  toplevel->handlefunc = &toplevel_handlefunc;
  toplevel->next = NULL;
  ei_widgetclass_register(toplevel);
}

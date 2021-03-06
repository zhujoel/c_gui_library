#include "../include/ei_application.h"
#include "../include/ei_utils.h"
#include "../include/ei_event.h"
#include "../include/ei_widget.h"
#include "../include/ei_widgetclass.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_widget_t* root;
static ei_surface_t main_window	= NULL;
static ei_surface_t picking_surface = NULL;
static ei_bool_t continuer = EI_TRUE;

static ei_font_t toplevel_font;

extern ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent);

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
  //Initialize the hardware
  hw_init();

  //Register widgets
  ei_frame_register_class();
  ei_button_register_class();
  ei_toplevel_register_class();

  //Load font
  toplevel_font = hw_text_font_create("misc/font.ttf", ei_style_normal, 20);

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


// Test si le point est dans le widget

ei_bool_t dans_w(ei_point_t point, ei_widget_t* widget)
{
  if(widget->placer_params != NULL){
    int x = widget->placer_params->x_data;
    int y = widget->placer_params->y_data;
    int w = widget->placer_params->w_data;
    int h = widget->placer_params->h_data;
    if (widget->parent != NULL)
    {
      x = x + (widget->placer_params->rx_data * widget->parent->placer_params->w_data) + widget->parent->placer_params->x_data;
      y = y + (widget->placer_params->ry_data * widget->parent->placer_params->h_data) + widget->parent->placer_params->y_data;
      w = w + (widget->placer_params->rw_data * widget->parent->placer_params->w_data);
      h = h + (widget->placer_params->rh_data * widget->parent->placer_params->h_data);
    }
    apply_anchor(widget->placer_params->anchor_data, &x, &y, &w, &h);
    return (x < point.x) && (x + w > point.x) && (y < point.y) && (y + h > point.y);
  }
  else{return EI_FALSE;}
}

// Parcourt les widgets pour trouver celui ?? l'emplacement du point

ei_widget_t* parcours_profondeur_pick(ei_widget_t* widget, ei_point_t point){
  if(widget->placer_params != NULL){
    if (dans_w(point, widget))
    {
      if (widget->children_head == NULL){return widget;}
      else
      {
        ei_widget_t* courant = widget->children_head;
        ei_widget_t* dernierwidget = NULL;
        while (courant != NULL)
        {
          if (courant->placer_params != NULL && dans_w(point, courant))
          {
            dernierwidget = courant;
          }
          courant = courant->next_sibling;
        }
        if (dernierwidget == NULL){return widget;}
        else {return parcours_profondeur_pick(dernierwidget, point);}
      }
    }
    else{return widget;}
  }else{
    return widget;
  }
}


void ei_app_run(){
  ei_widget_t* actif;
  ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), ei_app_root_surface(), picking_surface, &ei_app_root_widget()->screen_location); //widget->content_rect);
  hw_surface_update_rects(ei_app_root_surface(), NULL);
  struct ei_event_t* event = malloc(sizeof(struct ei_event_t*));

  // Boucle principale
  while (continuer)
  {
    hw_event_wait_next(event);
    actif = ei_event_get_active_widget();
    if (actif != NULL)
    {
      actif->wclass->handlefunc(actif, event);
      hw_surface_lock(ei_app_root_surface());
      ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), ei_app_root_surface(), picking_surface, &ei_app_root_widget()->screen_location);
      hw_surface_unlock(ei_app_root_surface());
    }
    else
    {
      if ((event->type == ei_ev_mouse_buttondown)
          || (event->type == ei_ev_mouse_buttonup))
      {
        actif = parcours_profondeur_pick(ei_app_root_widget(), event->param.mouse.where);
        actif->wclass->handlefunc(actif, event);

        hw_surface_lock(ei_app_root_surface());
        ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), ei_app_root_surface(), picking_surface, &ei_app_root_widget()->screen_location);
        hw_surface_unlock(ei_app_root_surface());
      }
      else
      {
        ei_default_handle_func_t f = ei_event_get_default_handle_func();
        if (f != NULL)
        {
          f(event);
        }
      }
    }
    hw_surface_update_rects(ei_app_root_surface(), NULL);

  }
  free(event);
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

ei_font_t ei_get_toplevel_font(){
  return toplevel_font;
}

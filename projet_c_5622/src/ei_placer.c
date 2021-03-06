#include "../include/ei_placer.h"
#include "../include/ei_widget.h"
#include "../include/ei_widgets.h"
#include "../include/ei_utils.h"
#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern ei_font_t ei_get_toplevel_font();

void ei_place (struct ei_widget_t* widget, ei_anchor_t* anchor, int* x, int* y, int* width, int* height, float* rel_x, float* rel_y, float* rel_width, float* rel_height){
  ei_placer_params_t* placer_params = malloc(sizeof(ei_placer_params_t));

  placer_params->anchor_data = anchor ? *anchor : ei_anc_northwest;
  placer_params->x_data = x ? *x : 0;
  placer_params->y_data = y ? *y : 0;
  placer_params->rx_data = rel_x ? *rel_x : 0.0;
  placer_params->ry_data = rel_y ? *rel_y : 0.0;

  placer_params->w_data = width ? *width : (rel_width ? 0 : widget->requested_size.width);
  placer_params->h_data  = height ? *height : (rel_height ? 0 : widget->requested_size.height);
  placer_params->rw_data = rel_width ? *rel_width : 0.0;
  placer_params->rh_data = rel_height ? *rel_height : 0.0;

  placer_params->x = &placer_params->x_data;
  placer_params->y = &placer_params->y_data;
  placer_params->w = &placer_params->w_data;
  placer_params->h = &placer_params->h_data;
  placer_params->rx = &placer_params->rx_data;
  placer_params->ry = &placer_params->ry_data;
  placer_params->rw = &placer_params->rw_data;
  placer_params->rh = &placer_params->rh_data;

  widget->placer_params = placer_params;
  ei_placer_run(widget);
}

extern void apply_anchor(ei_anchor_t anchor, int* x, int* y, int* w, int* h);

void ei_placer_run(struct ei_widget_t* widget){

  if(widget->pick_id != 0){
    int w = 0;
    int h = 0;
    if(strcmp(widget->wclass->name, "button")==0){
      struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
      if(widgetbutton->text != NULL){
        hw_text_compute_size (widgetbutton->text, widgetbutton->text_font, &w, &h);
      }
    }
    else if(strcmp(widget->wclass->name, "frame")==0){
      struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
      if(widgetframe->text != NULL){
        hw_text_compute_size (widgetframe->text, widgetframe->text_font, &w, &h);
      }
    }else if(strcmp(widget->wclass->name, "toplevel")==0){
      struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
      //The same font used in the toplevel drawfunc
      if(widgettoplevel->title != NULL){
        hw_text_compute_size (widgettoplevel->title, ei_get_toplevel_font(), &w, &h);
        //Add the red button size
        w += h + 10;
        h += 15;
      }
    }
    ei_widget_t* root = ei_app_root_widget();
    int diff_x = w - (widget->placer_params->w_data + (widget->placer_params->rw_data * root->screen_location.size.width));
    int diff_y = h - (widget->placer_params->h_data + (widget->placer_params->rh_data * root->screen_location.size.height));

    if(diff_x > 0){
      widget->placer_params->w_data += diff_x;
      widget->screen_location.size.width += diff_x;
    }
    if(diff_y > 0){
      widget->placer_params->h_data += diff_y;
      widget->screen_location.size.height += diff_y;
    }
  }

  //Recalc the screen location of the widget
  int screen_x = 0;
  int screen_y = 0;
  if (widget != ei_app_root_widget()){
    screen_x = widget->parent->screen_location.top_left.x;
    screen_y = widget->parent->screen_location.top_left.y;

    screen_x += widget->placer_params->x_data + (widget->placer_params->rx_data * widget->parent->screen_location.size.width);
    screen_y += widget->placer_params->y_data + (widget->placer_params->ry_data * widget->parent->screen_location.size.height);
    int screen_w = widget->placer_params->w_data + (widget->placer_params->rw_data * widget->parent->screen_location.size.width);
    int screen_h = widget->placer_params->h_data + (widget->placer_params->rh_data * widget->parent->screen_location.size.height);

    apply_anchor(widget->placer_params->anchor_data, &screen_x, &screen_y, &screen_w, &screen_h);

    if(screen_x + screen_w > ei_app_root_widget()->screen_location.size.width){
      screen_w = ei_app_root_widget()->screen_location.size.width - screen_x;
    }
    if(screen_y + screen_h > ei_app_root_widget()->screen_location.size.height){
      screen_h = ei_app_root_widget()->screen_location.size.height - screen_y;
    }

    ei_rect_t screen_loc = ei_rect(ei_point(screen_x, screen_y), ei_size(screen_w, screen_h));
    widget->screen_location = screen_loc;
    widget->content_rect = &screen_loc;
  }else{
    ei_rect_t screen_loc = ei_rect(ei_point_zero(), ei_size(widget->placer_params->w_data, widget->placer_params->h_data));
    widget->screen_location = screen_loc;
    widget->content_rect = &screen_loc;
  }
  //On recalcule pour les enfants
  ei_widget_t* courant = widget->children_head;
  while (courant!=NULL)
  {
    ei_placer_run(courant);
    courant = courant->next_sibling;
  }

}

void ei_placer_forget(struct ei_widget_t* widget){
  widget->placer_params = NULL;
}

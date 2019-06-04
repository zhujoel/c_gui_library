#include "../include/ei_placer.h"
#include "../include/ei_widget.h"
#include "../include/ei_widgets.h"
#include "../include/ei_utils.h"
#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void ei_place (struct ei_widget_t* widget, ei_anchor_t* anchor, int* x, int* y, int* width, int* height, float* rel_x, float* rel_y, float* rel_width, float* rel_height){
  ei_placer_params_t* placer_params = malloc(sizeof(ei_placer_params_t));

  placer_params->anchor_data = anchor ? *anchor : ei_anc_northwest;
  placer_params->x_data = x ? *x : 0;
  placer_params->y_data = y ? *y : 0;
  placer_params->rx_data = rel_x ? *rel_x : 0.0;
  placer_params->ry_data = rel_y ? *rel_y : 0.0;

  placer_params->w_data = width ? *width : (placer_params->rw ? widget->requested_size.width : 0);
  placer_params->h_data  = height ? *height : (placer_params->rh ? widget->requested_size.height : 0);
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

  int screen_x = 0;
  int screen_y = 0;
  if (widget != ei_app_root_widget()){
    screen_x = widget->parent->screen_location.top_left.x;
    screen_y = widget->parent->screen_location.top_left.y;

    // printf("screen_y : %i\n", ei_app_root_widget()->screen_location.size.width);
    // printf("screen_y : %i\n", ei_app_root_widget()->screen_location.size.height);

    screen_x += placer_params->x_data + (placer_params->rx_data * widget->parent->screen_location.size.width);
    screen_y += placer_params->y_data + (placer_params->ry_data * widget->parent->screen_location.size.height);
    int screen_w = placer_params->w_data + (placer_params->rw_data * widget->parent->screen_location.size.width);
    int screen_h = placer_params->h_data + (placer_params->rh_data * widget->parent->screen_location.size.height);

    ei_rect_t screen_loc = ei_rect(ei_point(screen_x, screen_y), ei_size(screen_w, screen_h));
    widget->screen_location = screen_loc;
    widget->content_rect = &screen_loc;
  }else{
    ei_rect_t screen_loc = ei_rect(ei_point_zero(), ei_size(placer_params->w_data, placer_params->h_data));
    widget->screen_location = screen_loc;
    widget->content_rect = &screen_loc;
  }

  // printf("%i\n", widget->screen_location.top_left.x);
  // printf("%i\n", widget->screen_location.top_left.y);
  // printf("%i\n", widget->screen_location.size.width);
  // printf("%i\n", widget->screen_location.size.height);

  // printf("Content rect ---------\n");

  // printf("%i\n", widget->content_rect->top_left.x);
  // printf("%i\n", widget->content_rect->top_left.y);
  // printf("%i\n", widget->content_rect->size.width);
  // printf("%i\n", widget->content_rect->size.height);

  ei_placer_run(widget);
}

void ei_placer_run(struct ei_widget_t* widget){
  int w;
  int h;
  // printf("dezf\n");
  if(strcmp(widget->wclass->name, "button")==0){
    struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
    if(widgetbutton->text != NULL){
      hw_text_compute_size (*widgetbutton->text, widgetbutton->text_font, &w, &h);
    }
  }else if(strcmp(widget->wclass->name, "frame")==0){
    struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
    if(widgetframe->text != NULL){
      // printf("Text found  in placer run\n");
      hw_text_compute_size (*widgetframe->text, widgetframe->text_font, &w, &h);
    }
  }

  ei_widget_t* root = ei_app_root_widget();

  int diff = w - (widget->placer_params->w_data + (widget->placer_params->w_data * root->screen_location.size.width));

  if(diff > 0){
    // printf("wow\n");
    widget->placer_params->w_data += diff;
  }

}

void ei_placer_forget(struct ei_widget_t* widget){
  widget->placer_params = NULL;
}

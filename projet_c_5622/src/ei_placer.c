#include "../include/ei_placer.h"
#include "../include/ei_widget.h"

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
}

void ei_placer_run(struct ei_widget_t* widget){

}

void ei_placer_forget(struct ei_widget_t* widget){
  widget->placer_params = NULL;
}

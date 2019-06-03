#include "../include/ei_widget.h"
#include "../include/ei_application.h"
#include "../include/ei_widgets.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint32_t picks_id = 0;

ei_color_t convertIdToColor(uint32_t id){
  ei_color_t pick_color = {id & 0x000000FF, (id & 0x0000FF00) >> 8, (id & 0x00FF0000) >> 16, (id & 0xFF000000) >> 24};
  return pick_color;
}

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
    new_widget->pick_id = picks_id++;
    ei_color_t pick_color = convertIdToColor(new_widget->pick_id);
    new_widget->pick_color = &pick_color;
    //Hierarchy
    new_widget->parent = parent;
    new_widget->children_head = NULL;
    new_widget->children_tail = NULL;
    new_widget->next_sibling = NULL;

    if(parent != NULL){
      if(parent->children_head == NULL){
        parent->children_head = new_widget;
      }else{
        parent->children_tail->next_sibling = new_widget;
      }
      parent->children_tail = new_widget;
    }

    //Geomeetry
    new_widget->placer_params = NULL;
    new_widget->wclass->setdefaultsfunc(new_widget);



    return new_widget;
}

ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name, ei_widget_t* parent){
  if(parent != NULL){
    return ei_widget_create_root(class_name, parent);
  }
  //Parent is null
  fprintf(stderr, "A widget parent cannot be NULL !");
  return NULL;
}

void ei_widget_destroy (ei_widget_t* widget){
  free(widget);
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
    widgetframe->color = color;
  }
  if(border_width != NULL){
    widgetframe->border_width = border_width;
  }
  if(relief != NULL){
    widgetframe->relief = relief;
  }
  if(text_font != NULL){
    widgetframe->text_font = text_font;
  }
  if(text != NULL){
    widgetframe->text = text;
    if(widget->placer_params != NULL){
      printf("placer run\n");
      ei_placer_run(widget);
    }
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
  struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
  if(requested_size != NULL){
    widgetbutton->widget.requested_size = *requested_size;
  }
  if(color != NULL){
    widgetbutton->color = color;
  }
  if(relief != NULL){
    widgetbutton->relief = relief;
  }
  if(text_font != NULL){
    widgetbutton->text_font = text_font;
  }
  if(text != NULL){
    widgetbutton->text = text;
    printf("%s\n", *text);
    if(widget->placer_params != NULL){
      ei_placer_run(widget);
    }
  }
  if(text_color != NULL){
    widgetbutton->text_color = text_color;
  }
  if(text_anchor != NULL){
    widgetbutton->text_anchor = text_anchor;
  }
  if(img != NULL){
    widgetbutton->img = img;
  }
  if(img_rect != NULL){
    widgetbutton->img_rect = img_rect;
  }
  if(img_anchor != NULL){
    widgetbutton->img_anchor = img_anchor;
  }
  if(callback != NULL){
    widgetbutton->callback = callback;
  }
  if(user_param != NULL){
    widgetbutton->user_param = user_param;
  }
}

void ei_toplevel_configure (ei_widget_t* widget, ei_size_t* requested_size, ei_color_t* color, int* border_width, char** title, ei_bool_t* closable, ei_axis_set_t* resizable, ei_size_t** min_size){
  struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  if(requested_size != NULL){
    widgettoplevel->widget.requested_size = *requested_size;
  }
  if(requested_size != NULL){
    widgettoplevel->title = title;
  }
  if(requested_size != NULL){
    widgettoplevel->closable = closable;
  }
  if(requested_size != NULL){
    widgettoplevel->resizable = resizable;
  }
  if(requested_size != NULL){
    widgettoplevel->min_size = min_size;
  }
}

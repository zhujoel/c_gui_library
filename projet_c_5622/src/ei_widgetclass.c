#include "../include/ei_application.h"
#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_widgets.h"
#include "../include/ei_event.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/GROSSEBIBLIOTHEQUE.h"
//#include "../include/ei_placer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

typedef enum {
  ei_none = 0,
	ei_deplace,
	ei_redim,
} ei_interaction_t;

static ei_widgetclass_t* widgetsclass = NULL;
static ei_point_t* position_precedente;
static ei_interaction_t action = ei_none;

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
    // printf("Widget FRAME PLACE !\n");
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

    ei_rect_t rect = ei_rect(ei_point(x, y), ei_size(w, h));
    // printf("%i\n",widgetframe->border_width);
    // printf("Michel !\n");
    ei_draw_widget_with_relief_and_corner_radius_that_is_optional(surface, rect, widgetframe->color, NULL, 0, widgetframe->relief, widgetframe->border_width);
    // printf("BOB !\n");


    if (widgetframe->img != NULL){
      // printf("Draw frame IMAGE\n");
      //Draw the image frame//ei_draw_polygon (surface, pts, *widgetframe->color, NULL); //clipper);
      //ei_copy_surface(surface, NULL, widgetframe->img, NULL, EI_TRUE);
    }else if (widgetframe->text != NULL){
      // Draw the frame text
      // printf("Draw frame Text : %s\n", *widgetframe->text);

      //Frame Clipper
      ei_point_t point = ei_point(x, y);
      ei_size_t size = ei_size(w, h);
      ei_rect_t clipper = ei_rect(point, size);

      //Point
      int x_text = x;
      int y_text = y;
      int w_text;
      int h_text;
      hw_text_compute_size (widgetframe->text, widgetframe->text_font, &w_text, &h_text);

      apply_anchor_text(widgetframe->text_anchor, &x_text, &y_text, &w_text, &h_text, &w, &h);

      ei_point_t text_point = ei_point(x_text, y_text);

    	ei_draw_text(surface, &text_point, widgetframe->text, widgetframe->text_font, &widgetframe->text_color, &clipper);
    }



    // printf("OUI\n");
    ei_widget_t* child = widget->children_head;
    if (child != NULL)
    {
      // printf("child != null\n");
      if(widget->children_tail == child){
          // printf("TAIL = HEAD\n");
      }

      while(child != widget->children_tail)
      {
        // printf("Widget while\n");
        //child->wclass->drawfunc(widget, ei_app_root_surface(), pick_surface, NULL);//&widget->screen_location); //widget->content_rect);
        if(child->next_sibling != NULL){
          //child = child->next_sibling;
        }
      }
      // printf("FIN WHILE\n");
      child->wclass->drawfunc(child, ei_app_root_surface(), pick_surface, NULL); //&widget->screen_location); //widget->content_rect);
    //   printf("OUI\n");
    }

  }else{
    // printf("Avoid printing frame widget\n");
  }
}

void frame_setdefaultsfunc(struct ei_widget_t* widget){
  struct ei_widget_frame_t* widgetframe = (struct ei_widget_frame_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgetframe->widget.requested_size = screen_size;
  widgetframe->color = default_color;
  int border_width = 0;
  widgetframe->border_width = border_width;
  ei_relief_t default_relief = ei_relief_none;
  widgetframe->relief = default_relief;
  widgetframe->text = NULL;
  widgetframe->text_font = ei_default_font;
  widgetframe->text_color = default_color;
  ei_anchor_t anchor_center = ei_anc_center;
  widgetframe->text_anchor = anchor_center;
  widgetframe->img = NULL;
  widgetframe->img_rect = NULL;
  widgetframe->img_anchor = anchor_center;
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

//TODO : Regarder Content_rect

  if(widget->placer_params != NULL){
    // printf("Widget BUTTON PLACE !\n");
    struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;

    ei_placer_params_t* placer_params = widget->placer_params;

    int x = placer_params->x_data;
    int y = placer_params->y_data;
    int w = placer_params->w_data;
    int h = placer_params->h_data;

    printf("hoho %i %i %i %i \n", x, y, w, h);

    if(widget->parent != NULL){
      x = x + (placer_params->rx_data * widget->parent->placer_params->w_data);
      y = y + (placer_params->ry_data * widget->parent->placer_params->h_data);
      w = w + (placer_params->rw_data * widget->parent->placer_params->w_data);
      h = h + (placer_params->rh_data * widget->parent->placer_params->h_data);
    }

    apply_anchor(placer_params->anchor_data, &x, &y, &w, &h);

    // printf("PRINT LE BOUTON\n");
    // printf("%i\n", x);
    // printf("%i\n", y);
    // printf("%i\n", w);
    // printf("%i\n", h);
    // printf("Border : %i\n", widgetbutton->border_width);
    // printf("Corner : %i\n", widgetbutton->corner_radius);

    printf("hehe %i %i %i %i \n", x, y, w, h);

    ei_point_t rect_point = {x, y};
    ei_size_t rect_size = {w, h};
    ei_rect_t rect = {rect_point, rect_size};

    printf("haha %i %i %i %i \n", rect.top_left.x, rect.top_left.y, rect.size.width, rect.size.height);

    ei_draw_widget_with_relief_and_corner_radius_that_is_optional(surface, rect, widgetbutton->color, NULL, widgetbutton->corner_radius, widgetbutton->relief, widgetbutton->border_width);
    //ei_draw_button(surface, pts, *widgetbutton->color, clipper):
    // ei_draw_polygon (surface, pts, *widgetbutton->color, NULL); //TODO Clipper du parent
    // ei_draw_polygon (pick_surface, pts, *widget->pick_color, NULL); //TODO Clipper du parent
    // printf("1\n");
    if (widgetbutton->img != NULL){
      printf("Draw Button IMAGE\n");
      //Draw the image frame
      // TODO: (joel) c'est +/- une copie/colle de la fonction draw_image parce que
      // draw image prend le filename en parametre alors que là on a déjà la surface avec l'image dessiné,
      // du coup il faut copier
      ei_surface_t image = widgetbutton->img;
  	  ei_rect_t image_rect = hw_surface_get_rect(image);
     	ei_point_t dst_point = {x, y};
     	ei_size_t dst_size = {image_rect.size.width, image_rect.size.height};
     	ei_rect_t dst_rect = {dst_point, dst_size};

      // rectangle source où copier l'image
    	if (clipper != NULL){
    		// si il y a un clipper, on va copier qu'une partie de l'image :
    		// l'intersection entre le rectangle de destination et le clipper
    		int inter_topleftx = max(dst_rect.top_left.x, clipper->top_left.x);
    		int inter_toplefty = max(dst_rect.top_left.y, clipper->top_left.y);
    		int inter_bottomrightx = min(dst_rect.top_left.x + dst_rect.size.width, clipper->top_left.x + clipper->size.width );
    		int inter_bottomrighty = min(dst_rect.top_left.y + dst_rect.size.height, clipper->top_left.y + clipper->size.height );
    		int inter_width = inter_bottomrightx - inter_topleftx;
    		int inter_height = inter_bottomrighty - inter_toplefty;
    		ei_size_t intersection_size = {inter_width, inter_height};
    		ei_point_t intersection_point = {inter_topleftx, inter_toplefty};
    		ei_rect_t intersection = {intersection_point, intersection_size};

    		ei_point_t src_point = {intersection.top_left.x - dst_rect.top_left.x, intersection.top_left.y - dst_rect.top_left.y};
    		ei_size_t src_size = intersection_size;
    		ei_rect_t src_rect = {src_point, src_size};

    		dst_rect.top_left.x = inter_topleftx;
    		dst_rect.top_left.y = inter_toplefty;
    		ei_copy_surface(surface, &dst_rect, image, &src_rect, 1);
    	}
    	else{
    		ei_copy_surface(surface, &dst_rect, image, &rect, 1);
    	}

    }

    else if (widgetbutton->text != NULL){
      // Draw the frame text
      // printf("Draw Button Text : %p\n", widgetbutton->text);
      //Frame Clipper
      ei_point_t point = ei_point(x, y);
      ei_size_t size = ei_size(w, h);
      ei_rect_t clipper = ei_rect(point, size);

      //Point
      int x_text = x;// + *widgetbutton->border_width + (*widgetbutton->corner_radius /2);
      int y_text = y;// + *widgetbutton->border_width + (*widgetbutton->corner_radius /2);
      int w_text;
      int h_text;
      hw_text_compute_size (widgetbutton->text, widgetbutton->text_font, &w_text, &h_text);

      apply_anchor_text(widgetbutton->text_anchor, &x_text, &y_text, &w_text, &h_text, &w, &h);

      ei_point_t text_point = ei_point(x_text, y_text);

    	ei_draw_text(surface, &text_point, widgetbutton->text, widgetbutton->text_font, &widgetbutton->text_color, &widget->screen_location);
      // printf("TEXT POS\n");
      // printf("%i\n", x_text);
      // printf("%i\n", y_text);
      // printf("%i\n", w_text);
      // printf("%i\n", h_text);

      // printf("SCREEN LOC PARENT\n");
      // printf("%i\n", widget->screen_location.top_left.x);
      // printf("%i\n", widget->screen_location.top_left.y);
      // printf("%i\n", widget->screen_location.size.width);
      // printf("%i\n", widget->screen_location.size.height);


    }

  }else{
    // printf("Avoid printing Button widget\n");
  }
}

void button_setdefaultsfunc(struct ei_widget_t*	widget){
  struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;
  ei_size_t	screen_size = {0, 0};
  widgetbutton->widget.requested_size = screen_size;
  widgetbutton->color = ei_default_background_color;
  widgetbutton->relief = ei_relief_none;
  widgetbutton->text = NULL;
  widgetbutton->text_font = ei_default_font;
  widgetbutton->text_color = ei_default_background_color;
  widgetbutton->text_anchor = ei_anc_center;
  widgetbutton->img = NULL;
  widgetbutton->img_rect = NULL;
  widgetbutton->img_anchor = ei_anc_center;

  // int corner_radius = 0;//k_default_button_corner_radius;
  // int border_width = 0; //k_default_button_border_width;
  widgetbutton->corner_radius = k_default_button_corner_radius;
  widgetbutton->border_width = k_default_button_border_width;

  // printf("Border : %i\n", widgetbutton->border_width);
  // printf("Corner : %i\n", widgetbutton->corner_radius);
}

void button_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

ei_bool_t button_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  // printf("button handle\n");
  struct ei_widget_button_t* widgetbutton = (struct ei_widget_button_t*)widget;

  if (event->type == ei_ev_mouse_buttondown && widgetbutton->relief != ei_relief_none)
  {
    printf("bring sally down \n");
    widgetbutton->relief = ei_relief_sunken;

    // TODO: (joel) à modifier ? lol parce que on dessine le bouton a chaque fois qu'on appuie
    button_drawfunc(&widgetbutton->widget, ei_app_root_surface(), NULL, NULL);

    return  EI_TRUE;
  }

  else if (event->type == ei_ev_mouse_buttonup && widgetbutton->relief != ei_relief_none)
  {
    printf("bring sally up \n");
    widgetbutton->relief = ei_relief_raised;

    // TODO: (joel) à modifier ? lol parce que on dessine le bouton a chaque fois qu'on appuie
    button_drawfunc(&widgetbutton->widget, ei_app_root_surface(), NULL, NULL);

    // TODO: user_param à modifier avec une vraie paramètre éventuellement
    int user_param = 52;
    widgetbutton->callback(&(widgetbutton->widget), NULL, &user_param);
    return  EI_TRUE;
  }
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
  struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  //Point
  int x =  widget->placer_params->x_data;
  int y =  widget->placer_params->y_data;
  int w = widget->placer_params->w_data;
  int h = widget->placer_params->h_data;
  int x_text = 10;
  int y_text = 0;
  ei_point_t text_point = {x_text, y_text};
  int* w_text = malloc(sizeof(int));
  int* h_text = malloc(sizeof(int));
  ei_fontstyle_t fontstyle = ei_style_normal;
  ei_font_t font = hw_text_font_create("fonts/BigCheese.ttf", fontstyle, 30);
  hw_text_compute_size (widgettoplevel->title, font, w_text, h_text);
  //Dessin de l'entête
  ei_rect_t rectangle = ei_rect(ei_point(x,y), ei_size(w, *h_text));
  float rayon = 20;
  ei_bool_t* bords = malloc(sizeof(ei_bool_t)*4);
	bords[0] = 1;
	bords[1] = 1;
	bords[2] = 0;
	bords[3] = 0;
  ei_linked_point_t* entete = rounded_frame(rectangle, rayon, bords);
  ei_color_t grey = {0x80, 0x80, 0x80, 0xff};
  ei_draw_polygon(surface, entete, grey, clipper);
  ei_draw_polygon(pick_surface, entete, *widget->pick_color, clipper);
  //Dessin du bouton rouge
  ei_color_t red = {0xed, 0x24, 0x09, 0xff};
  ei_linked_point_t* bouton_rouge = arc(ei_point(x+5,y+5), rayon - 1, 0, 360);
  ei_draw_polygon(surface, bouton_rouge, red, clipper);
  //On écrit le titre dans la meilleur police possible
  ei_color_t yellow = {0xf3, 0xf3, 0x0C, 0xff};
  ei_draw_text(surface, &text_point, widgettoplevel->title, font, &yellow, clipper);
  //Dessin du reste
  rectangle = ei_rect(ei_point(x,y + *h_text), ei_size(w, h - *h_text));
  bords[0] = 0;
	bords[1] = 0;
	bords[2] = 1;
	bords[3] = 1;
  // printf("x : %i\n",rectangle.top_left.x);
  // printf("y : %i\n",rectangle.top_left.y);
  // printf("w : %i\n",rectangle.size.width);
  // printf("h : %i\n",rectangle.size.height);
  ei_linked_point_t* corps = rounded_frame(rectangle, rayon, bords);
  // ei_linked_point_t* courant2 = corps;
  // while (courant2 != NULL)
  // {
  //   printf("%i\n", courant2->point.y);
  //   courant2 = courant2->next;
  // }
  ei_draw_polygon(surface, corps, widgettoplevel->color, clipper);
  //On dessine les enfants
  ei_widget_t* courant = widget->children_head;
  while (courant!=NULL)
  {
    courant->wclass->drawfunc(courant, surface, pick_surface, clipper);
    courant = courant->next_sibling;
  }
  //Le petit carré pour resize
  ei_linked_point_t	carre[5];
  ei_color_t black = {0xff, 0xff, 0xff, 0xff};
	carre[0].point.x = x+w-10; carre[0].point.y = y+h; carre[0].next = &carre[1];
	carre[1].point.x = x+w-10; carre[1].point.y = y+h-10; carre[1].next = &carre[2];
	carre[2].point.x = x+w; carre[2].point.y = y+h-10; carre[2].next = &carre[3];
  carre[3].point.x = x+w; carre[3].point.y = y+h; carre[3].next = &carre[4];
  carre[4].point.x = x+w-10; carre[4].point.y = y+h; carre[4].next = NULL;
  ei_draw_polygon(surface, carre, widgettoplevel->color, clipper);
  ei_draw_polyline(surface, carre, black, clipper);
}

void toplevel_setdefaultsfunc(struct ei_widget_t*	widget){
  struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  ei_size_t	screen_size = {0, 0};
  ei_color_t default_color = ei_default_background_color;
  widgettoplevel->widget.requested_size = screen_size;
  widgettoplevel->color = default_color;
  int default_width = 4;
  widgettoplevel->border_width = default_width;
  char* default_title = "TopLevel";
  widgettoplevel->title = default_title;
  //strcpy(*widgettoplevel->title, "TopLevel");
  ei_bool_t closable = EI_TRUE;
  widgettoplevel->closable = closable;
  ei_axis_set_t axis = ei_axis_both;
  widgettoplevel->resizable = axis;
  ei_size_t	min_size = {160, 120};
  widgettoplevel->min_size = &min_size;
}

void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect){

}

/* Vérifie que le point est dans un rectangle, dont (x1,y1) est topleft
 et (x2,y2) est bottom right */
ei_bool_t dedans(ei_point_t point, int x1, int y1, int x2, int y2)
{
  return (x1 < point.x) && (x2 > point.x) && (y1 < point.y) && (y2 > point.y);
}

ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget, struct ei_event_t* event){
  struct ei_widget_toplevel_t* widgettoplevel = (struct ei_widget_toplevel_t*)widget;
  int x1 = widget->placer_params->x_data;
  int y1 = widget->placer_params->y_data;
  int x2 = x1 + widget->placer_params->w_data;
  int y2 = y1 + widget->placer_params->h_data;
  int y3 = y1 + widgettoplevel->border_width; // border_width = taille de l'entête ??????
  ei_widget_t* actif = ei_event_get_active_widget();

  if (actif == NULL)   // On a pas de widget actif
  {
    if (event->type == ei_ev_mouse_buttondown
    && event->param.mouse.button_number == 1)
    {
      printf("appuye\n");
      if (dedans(event->param.mouse.where,x1,y1,x2,y3)) // Pour déplacer la fenetre
      {
        printf("selec\n");
        *position_precedente = event->param.mouse.where;
        ei_event_set_active_widget(widget);
        action = ei_deplace;
        return EI_TRUE;
      }
      else if (widgettoplevel->resizable != ei_axis_none
              && dedans(event->param.mouse.where,x2-10,y2-10,x2,y2))       // Pour redimensionner la fenetre
      {
        *position_precedente = event->param.mouse.where;
        ei_event_set_active_widget(widget);
        action = ei_redim;
        return EI_TRUE;
      }
      else if (widgettoplevel->closable == EI_TRUE
              && dedans(event->param.mouse.where,x1,y1,x1+10,y1+10))      // Pour fermer la fenetre
      {
        printf("Quit\n");
        ei_app_quit_request();
        return EI_TRUE;
      }
    }
  }

  else if (actif == widget)  // le widget actif est le widget sélectionné
  {
    if (event->type == ei_ev_mouse_buttonup
       && event->param.mouse.button_number == 1  )
    {
      ei_event_set_active_widget(NULL);
      return EI_TRUE;
    }
    else if (event->type == ei_ev_mouse_move)
    {
      int deltax = event->param.mouse.where.x - position_precedente->x;
      int deltay = event->param.mouse.where.y - position_precedente->y;
      if (action == ei_deplace)
      {
        widget->placer_params->x_data += deltax;
        widget->placer_params->y_data += deltay;
      }
      else if (action == ei_redim)
      {
        if (widgettoplevel->resizable == ei_axis_x || widgettoplevel->resizable == ei_axis_both)
        {
          widget->placer_params->w_data += deltax;
        }
        if (widgettoplevel->resizable == ei_axis_y || widgettoplevel->resizable == ei_axis_both)
        {
          widget->placer_params->h_data += deltay;
        }
      }
      *position_precedente = event->param.mouse.where;
      return EI_TRUE;
    }
  }
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

#include "../include/ei_application.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ei_widget_t* root;
static ei_surface_t main_window	= NULL;

extern ei_widget_t* ei_widget_create_root(ei_widgetclass_name_t	class_name, ei_widget_t* parent);

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen){
  //Initialize the hardware
  hw_init();

  //Register widgets
  ei_frame_register_class();
  //ei_button_register_class();
  //ei_toplevel_register_class();
  //...

  //Creates the root window
  main_window =	hw_create_window(main_window_size, fullscreen);

  //Creates the root widget
  root = ei_widget_create_root("frame", NULL);
  ei_frame_configure(ei_app_root_widget(), main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  ei_place(ei_app_root_widget(), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_free(){
  hw_quit();
}

/* Création d'un type de liste chainée de widget pour
le parcours en longueur de l'arbre de widget */
typedef struct ei_widget_chaine_t{
  ei_widget_t* widget;
  struct ei_widget_chaine_t* suivant;
} ei_widget_chaine_t;

typedef struct {
  struct ei_widget_chaine_t* tete;
  struct ei_widget_chaine_t* queue;
} ei_widget_liste_t;

// Ajoute en tête, plante si le pointeur de liste en entrée est nul
void ajouter_tete(ei_widget_liste_t* liste, ei_widget_t* widget)
{
  if (widget != NULL)
  {
    struct ei_widget_chaine_t* cellule = malloc(sizeof(struct ei_widget_chaine_t));
    cellule->widget = widget;
    cellule->suivant = liste->tete;
    liste->tete = cellule;
    if (liste->queue == NULL)
    {
      liste->queue = cellule;
    }
  }
}

// Ajoute en queue, plante si le pointeur de liste en entrée est nul,
void ajouter_queue(ei_widget_liste_t* liste, ei_widget_t* widget)
{
  if (widget != NULL)
  {
    struct ei_widget_chaine_t* cellule = malloc(sizeof(struct ei_widget_chaine_t));
    cellule->widget = widget;
    cellule->suivant = NULL;
    if (liste->tete == NULL)
    {
      liste->tete = cellule;
      liste->queue = cellule;
    }
    else
    {
    liste->queue->suivant = cellule;
    liste->queue = cellule;
    }
  }
}

void retirer_tete(ei_widget_liste_t* liste)
{
  if (liste->tete != NULL)
  {
    liste->tete = liste->tete->suivant;
    if (liste->tete == NULL)
    {
      liste->queue = NULL;
    }
  }
}


void ei_app_run(){
    //TODO : Parcours de la hiérarchie de widget
    ei_rect_t* clipper_ptr	= NULL;
    // Liste des widgets du niveau courant
    ei_widget_liste_t* courant = malloc(sizeof(ei_widget_liste_t));
    struct ei_widget_chaine_t* first = malloc(sizeof(struct ei_widget_chaine_t));
    first->widget = ei_app_root_widget();
    first->suivant = NULL;
    courant->tete = first;
    courant->queue = first;
    // Liste des widgets du niveau plus bas
    ei_widget_liste_t* suivante = malloc(sizeof(ei_widget_liste_t));
    suivante->tete = NULL;
    suivante->queue = NULL;
    while ((courant->tete->widget != NULL)) //|| (suivante->tete != NULL))
    {
        struct ei_widget_t* wcourant = courant->tete->widget;
        if(wcourant == NULL){
        }
        wcourant->wclass->drawfunc(wcourant,ei_app_root_surface(),ei_app_root_surface(),clipper_ptr);
        retirer_tete(courant);
        ajouter_tete(courant, wcourant->next_sibling);
        ajouter_queue(suivante, wcourant->children_head);
        if (courant->tete == NULL)
        {
          courant->tete = suivante->tete;
          courant->queue = suivante->queue;
          suivante->tete = NULL;
          suivante->queue = NULL;
        }
    }

    ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(),ei_app_root_surface(),ei_app_root_surface(),clipper_ptr);
    //ei_app_root_widget()->children_head->wclass->drawfunc(ei_app_root_widget()->children_head,ei_app_root_surface(),ei_app_root_surface(),clipper_ptr);


    printf("salut\n");

  //ei_widget_t* current = ei_app_root_widget()->children_head;

  //call drawfunc
  //ei_app_root_widget()->wclass->drawfunc(ei_app_root_widget(), ei_app_root_surface(), ei_app_root_surface(), clipper_ptr);

  /* Wait for a character on command line. */
	getchar();
}

void ei_app_invalidate_rect(ei_rect_t* rect){

}

void ei_app_quit_request(){
  printf("Je ferme");
}

ei_widget_t* ei_app_root_widget(){
  return root;
}

ei_surface_t ei_app_root_surface(){
  return main_window;
}

/**
 *  @file	GROSSEBIBLIOTHEQUE.h
 *  @brief	Fonctions implémentées à nous
 *
 *  \author
 *  Created by JOEL ZHU, PAUL CHALAS, THOMAS ZHANG
 *  Copyright 2040 Ensimag. All rights reserved.
 *
 */

#ifndef GROSSEBIBLIOTHEQUE_H
#define GROSSEBIBLIOTHEQUE_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"


#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

/**
 * \brief	Converti un integer de 32 bits en type couleur en utilisant l'ordre des couleurs
 * d'une surface.
 *
 * @param surface Surface où récupérer l'ordre des couleurs
 * @param color color en 32 bits à convertir
 *
 * @return une variable ei_color_t qui correspond au couleur 32 bits converti
 */
ei_color_t ei_map_color (ei_surface_t surface, const uint32_t* color);

/**
 * \brief	Génère une liste de points définissant un arc
 *
 * @param centre Centre de l'arc
 * @param rayon Rayon de l'arc
 * @param angle_debut Angle de début de l'arc
 * @param angle_fin Angle de fin de l'arc
 *
 * @return Retourne la liste de points définissant l'arc
 */
ei_linked_point_t* arc (const ei_point_t centre,
							float rayon,
							int angle_debut,
							int angle_fin);

/**
 * \brief	Génère une liste de points définissant un cadre aux bords arrondis
 *
 * @param rectangle  Rectangle de départ
 * @param rayon Rayon des arrondis
 * @param parties Bords du rectangle à arrondir, dans l'ordre : top-left, top-right,
 * bottom-right, bottom-left. Si NULL, arrondit tous les bords
 *
 * @return Retourne liste de points définissant un cadre aux bords arrondis
 */
ei_linked_point_t* rounded_frame (const ei_rect_t rectangle,
											float rayon,
											ei_bool_t* bords);

/**
 * \brief	Dessine un bouton enfoncé en relief
 *
 * @param surface Surface sur laquelle dessiner le bouton
 * @param first_point Point par lequel commencer à dessiner le bouton
 * @param coloor Couleur du bouton
 * @param clipper Clipper du dessin
 * @param rayon Rayon des arrondis du bouton
 * @param reliefType Type de relief, 0 enfonced, 1 releved, 2 aucun
 * @param distanceRectangle Distance entre les 2 rectangles (inner et outer) pour le relief
 */
void ei_draw_widget_with_relief_and_corner_radius_that_is_optional (ei_surface_t surface,
							 const ei_rect_t rect,
							 const ei_color_t color,
							 const ei_rect_t* clipper,
						   float rayon,
						 	 ei_relief_t reliefType,
						 	 int distanceRectangle);

/**
 * \brief	Dessine une image
 *
 * @param filename Nom du fichier de l'image
 * @param surface Surface sur laquelle dessiner l'image
 * @param where Point top_left où dessiner l'image
 * @param clipper Clipper pour l'image
 */
void ei_draw_image(const char* filename,
							ei_surface_t surface,
							ei_point_t* where,
							const ei_rect_t* clipper);


#endif


#ifndef GROSSEBIBLIOTHEQUE_H
#define GROSSEBIBLIOTHEQUE_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"


/**
 * Converti un integer de 32 bits en type couleur en utilisant l'ordre des couleurs
 * d'une surface.
 *
 * @param surface Surface où récupérer l'ordre des couleurs
 * @param color color en 32 bits à convertir
 *
 * @return une variable ei_color_t qui correspond au couleur 32 bits converti
 */
ei_color_t ei_map_color (ei_surface_t surface, const uint32_t* color);

/**
 * Génère une liste de points définissant un arc
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
 * Génère une liste de points définissant un cadre aux bords arrondis
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
 * Dessine un bouton enfoncé en relief
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



#endif

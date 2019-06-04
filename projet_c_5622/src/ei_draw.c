#include "../include/ei_draw.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef PI
	#define PI 3.14159265
#endif


/**
 * Prend une ei_color_t et renvoie sa représentation 32 bits
 */
uint32_t ei_map_rgba (ei_surface_t surface, const ei_color_t* color){
	int* ir = malloc(sizeof(int));
	int* ig = malloc(sizeof(int));
	int* ib = malloc(sizeof(int));
	int* ia = malloc(sizeof(int));
	hw_surface_get_channel_indices(surface, ir, ig, ib, ia);
	uint32_t ret = 0;
	if (*ia == -1)
	{
		ret = (color->red << (*ir)*8) + (color->green << (*ig)*8) + (color->blue << (*ib)*8) + 0xff000000;
	}
	else
	{
		ret = (color->red << (*ir)*8) + (color->green << (*ig)*8) + (color->blue << (*ib)*8) + (color->alpha << (*ia)*8);
	}
	return ret;
}

/**
 * Prend une couleur en représentation 32 bits et renvoie une ei_color_t
 */
ei_color_t ei_map_color (ei_surface_t surface, const uint32_t* color){
		int* ir = malloc(sizeof(int));
		int* ig = malloc(sizeof(int));
		int* ib = malloc(sizeof(int));
		int* ia = malloc(sizeof(int));
		hw_surface_get_channel_indices(surface, ir, ig, ib, ia);
		ei_color_t ret = {(*color >> (8*(*ir))) & 0xff, (*color >> (8*(*ig))) & 0xff, (*color >> (8*(*ib))) & 0xff, (*color >> (8*(*ia))) & 0xff};
		return ret;
}

void ei_draw_polyline (ei_surface_t surface, const ei_linked_point_t*	first_point, const ei_color_t color, const ei_rect_t* clipper){
	hw_surface_lock(surface);

	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
	// dimensions de la surface pour réaliser le clipping
	int surface_width = hw_surface_get_size(surface).width;
	int surface_height =  hw_surface_get_size(surface).height;
	// coordonnes bottom_right et top_left du clipper
	int clipping_x2 = surface_width;
	int clipping_y2 = surface_height;
	int clipping_x1 = 0;
	int clipping_y1 = 0;
	// si on a un clipper, on prend ses coordonnées
	if(clipper != NULL){
		clipping_x1 = clipper->top_left.x;
		clipping_y1 = clipper->top_left.y;
		clipping_x2 = clipping_x1+clipper->size.width;
		clipping_y2 = clipping_y1+clipper->size.height;
	}
	// si il n'y a qu'un seul point
	if(first_point->next == NULL){
		int coord_x = first_point->point.x;
		int coord_y = first_point->point.y;
		//on dessine seulement si le point est à l'intérieur du clipper
		if (coord_x < clipping_x2 && coord_x >= clipping_x1
		&& coord_y < clipping_y2 && coord_y >= clipping_y1){
				pixel_ptr +=  coord_x + coord_y*hw_surface_get_size(surface).width;
				*pixel_ptr = ei_map_rgba(surface, &color);
		}

	}
	// si on a plusieurs points
	else{
		// point de départ
		ei_point_t current = first_point->point;
		// suiveur d'arrivée
		ei_linked_point_t *suiveur_arrivee = first_point->next;

		// on dessine les segments pour tous les points
		while(suiveur_arrivee != NULL){
			int8_t incrementx = 1;
			int8_t incrementy = 1;
			// point d'arrivée
			ei_point_t arrivee = suiveur_arrivee->point;

			// si le point 1 est "avant" le point 2 en x
			if(current.x > arrivee.x){
				incrementx = -1;
			}
			if(current.y > arrivee.y){
				incrementy = -1;
			}
			int deltax = abs(arrivee.x - current.x);
			int deltay = abs(arrivee.y - current.y);
			float erreur = 0;

			// si on a une ligne verticale
			if(deltax == 0){
				// si les deux points ne sont pas connectés et on vérifie le clipping  par rapport à la surface
				while(current.y != arrivee.y && current.y != 0 && current.y != surface_height-1){
					// on incrémente y de 1
					current.y += incrementy;
					pixel_ptr += current.x + current.y*hw_surface_get_size(surface).width;
					//on dessine seulement si le point est à l'intérieur du clipper
					if (current.x < clipping_x2 && current.x >= clipping_x1
					&& current.y < clipping_y2 && current.y >= clipping_y1){
						*pixel_ptr = ei_map_rgba(surface, &color);
					}
					pixel_ptr -= current.x + current.y*hw_surface_get_size(surface).width;
				}
			}

			// si on a une ligne horizontale
			else if(deltay == 0){
				// si les deux points ne sont pas connectés et on vérifie le clipping par rapport à la surface
				while(current.x != arrivee.x && current.x != 0 && current.x != surface_width-1){
					// on incrémente x de 1
					current.x += incrementx;
					pixel_ptr += current.x + current.y*hw_surface_get_size(surface).width;
					//on dessine seulement si le point est à l'intérieur du clipper
					if (current.x < clipping_x2 && current.x >= clipping_x1
					&& current.y < clipping_y2 && current.y >= clipping_y1){
						*pixel_ptr = ei_map_rgba(surface, &color);
					}
					pixel_ptr -= current.x + current.y*hw_surface_get_size(surface).width;
				}
			}

			// si le segment est dirigé par x
			else if (deltax > deltay){
				// on incrémente x de 1
				while((current.x != arrivee.x) && (current.y != arrivee.y)){
					// on vérifie le clipping par rapport à la surface
					if ((current.x + incrementx) <= 0
					|| (current.y + incrementy) <= 0
					|| (current.x + incrementx) >= surface_width
					|| (current.y + incrementy) >= surface_height) {
						arrivee.x = current.x;
						arrivee.y = current.y;
						break;
					}

					current.x += incrementx;

					// calcul de l'erreur
					erreur += (float)deltay/deltax;
					if (erreur > 0.5){
						current.y += incrementy;
						erreur--;
					}
					pixel_ptr += current.x + current.y*hw_surface_get_size(surface).width;
					//on dessine seulement si le point est à l'intérieur du clipper
					if (current.x < clipping_x2 && current.x >= clipping_x1
					&& current.y < clipping_y2 && current.y >= clipping_y1){
						*pixel_ptr = ei_map_rgba(surface, &color);
					}
					pixel_ptr -= current.x + current.y*hw_surface_get_size(surface).width;
				}
			}
			// sinon, le segment est dirigé par y
			else{
				// on incrémente y de 1
				while((current.x != arrivee.x) && (current.y != arrivee.y)){
					// on vérifie le clipping  par rapport à la surface
					if ((current.x + incrementx) <= 0
					|| (current.y + incrementy) <= 0
					|| (current.x + incrementx) >= surface_width
					|| (current.y + incrementy) >= surface_height) {
						arrivee.x = current.x;
						arrivee.y = current.y;
						break;
					}

					current.y += incrementy;

					// calcul de l'erreur
					erreur += (float)deltax/deltay;
					if (erreur > 0.5){
						current.x += incrementx;
						erreur--;
					}
					pixel_ptr += current.x + current.y*hw_surface_get_size(surface).width;
					//on dessine seulement si le point est à l'intérieur du clipper
					if (current.x < clipping_x2 && current.x >= clipping_x1
					&& current.y < clipping_y2 && current.y >= clipping_y1){
						*pixel_ptr = ei_map_rgba(surface, &color);
					}
					pixel_ptr -= current.x + current.y*hw_surface_get_size(surface).width;
				}
			}
			current.x = arrivee.x;
			current.y = arrivee.y;
			suiveur_arrivee = suiveur_arrivee->next;
		}
	}

	hw_surface_unlock(surface);
}

typedef struct ei_cellule_t			// Définition du type cellule pour la fonction draw_polygon
	{
	int ymax;
	float	xymin;
	float pente;
	struct ei_cellule_t * suivant;
	} ei_cellule_t;

ei_cellule_t *derniere_cellule(ei_cellule_t* cellule) // Renvoie la derniere cellule d'une liste chainée de cellules
{
	if (cellule != NULL)
	{
		ei_cellule_t* courant = cellule;
		while (courant->suivant != NULL)
		{
			courant = courant->suivant;
		}
		return courant;
	}
	else{return NULL;}
}

int TC_non_vide(ei_cellule_t** TC, int taille)	// Renvoie 1 si TC n'est pas vide, 0 sinon
{
	int flag = 0;
	int compteur = 0;
	while ((compteur < taille) && (flag == 0))
	{
			if (TC[compteur] != NULL)
			{
					flag = 1;
			}
			compteur++;
	}
	return flag;
}

void supr_TCA(ei_cellule_t** TCA, int y)		// Supprime les cellules de TCA dont ymax est égal à y
{
	ei_cellule_t *courant = *TCA;
	if (courant != NULL)
	{
		while (courant->suivant != NULL)
		{
			if (courant->suivant->ymax == y)
			{
				courant->suivant = courant->suivant->suivant;
			}
			else
			{
				courant = courant->suivant;
			}
		}
		if ((*TCA)->ymax == y)
		{
				(*TCA) = (*TCA)->suivant;
		}
	}
}

void tri_TCA(ei_cellule_t** TCA)			// Tri le tableau TCA pas xymin croissant
{
	if (*TCA != NULL)
	{
		if ((*TCA)->suivant != NULL)
		{
			float xmin = (*TCA)->xymin;
			ei_cellule_t *courant = *TCA;
			while (courant != NULL)
			{
					xmin = (courant->xymin)<xmin?(courant->xymin):xmin;
					courant = courant->suivant;
			}
			courant = *TCA;
			while (courant->xymin != xmin)
			{
					courant = courant->suivant;
			}
			courant->xymin = (*TCA)->xymin;
			(*TCA)->xymin = xmin;
			int temp = courant->ymax;
			courant->ymax = (*TCA)->ymax;
			(*TCA)->ymax = temp;
			float temp2 = courant->pente;
			courant->pente = (*TCA)->pente;
			(*TCA)->pente = temp2;
			tri_TCA(&((*TCA)->suivant));
		}
	}
}

void maj_TCA(ei_cellule_t** TCA)		// Met à jour les abscisses d'intersection
{
	ei_cellule_t *courant = *TCA;
	if (courant != NULL)
	{
		while (courant != NULL)
		{
			courant->xymin = courant->xymin + courant->pente;
			courant = courant->suivant;
		}
	}

}

int nbr_TCA(ei_cellule_t** TCA)
{
	int ret = 0;
	ei_cellule_t *courant = *TCA;
	while (courant != NULL)
	{
		ret++;
		courant = courant->suivant;
	}
	return ret;
}

void ei_draw_polygon (ei_surface_t surface, const ei_linked_point_t* first_point, const ei_color_t color, const ei_rect_t* clipper)
{
	hw_surface_lock(surface);
	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
	// dimensions de la surface pour réaliser le clipping
	int surface_width = hw_surface_get_size(surface).width;
	int surface_height =  hw_surface_get_size(surface).height;
	// coordonnes bottom_right et top_left du clipper
	int clipping_x1 = (clipper == NULL)?0:(clipper->top_left.x);
	int clipping_y1 = (clipper == NULL)?0:(clipper->top_left.y);
	int clipping_x2 = (clipper == NULL)?surface_width:(clipping_x1+clipper->size.width);
	int clipping_y2 = (clipper == NULL)?surface_height:(clipping_y1+clipper->size.height);
	// Un point courant pour parcourir la liste de points
	ei_linked_point_t *courant = malloc(sizeof(ei_linked_point_t));
	courant->point = first_point->point;
	courant->next = first_point->next;
	// Déclaration de TC et de TCA
	ei_cellule_t** TC = malloc(sizeof(struct ei_cellule_t) * surface_height);
	for (size_t i = 0; i < surface_height; i++) {
		TC[i] = NULL;
	}
	ei_cellule_t* TCA = malloc(sizeof(struct ei_cellule_t));
	TCA = NULL;
	// Pour savoir où commencer le remplissage
	int ydepart = clipping_y2;
	while (courant->next != NULL)				// Remplissage de TC
	{
			float x1 = courant->point.x;
			float y1 = courant->point.y;
			float x2 = courant->next->point.x;
			float y2 = courant->next->point.y;
			if ((y2-y1)!=0)
			{
				float pente = (x2-x1)/(y2-y1);

				uint32_t ymin = y1<y2?y1:y2;
				ydepart = ymin<ydepart?ymin:ydepart;
				ei_cellule_t* nouveau = malloc(sizeof(struct ei_cellule_t));
				nouveau->ymax =  y1>y2?y1:y2;
				nouveau->xymin = y1<y2?x1:x2;
				nouveau->pente = pente;
				nouveau->suivant = NULL;
				if (TC[ymin] == NULL)
				{
					TC[ymin] = nouveau;
				}
				else
				{
					ei_cellule_t *last = derniere_cellule(TC[ymin]);
					last->suivant = nouveau;
				}
			}
			courant = courant->next;
	}
	while (((TC_non_vide(TC, hw_surface_get_size(surface).height) == 1) || (nbr_TCA(&TCA) != 0)) && (ydepart < clipping_y2)) // Algorithme de remplissage
	{
			if (derniere_cellule(TCA) == NULL)
			{
				TCA = TC[ydepart];
			}
			else { derniere_cellule(TCA)->suivant = TC[ydepart];}
			TC[ydepart] = NULL;
			supr_TCA(&TCA, ydepart);																			// supression
			tri_TCA(&TCA);																								// tri
			if ((ydepart < clipping_y2) && (ydepart > clipping_y1) )			// Si y est dans le rectangle de clipping
			{
				ei_cellule_t *borne1 = TCA;																		// coloration
				ei_cellule_t *borne2;
				int currentx;
				while (borne1 != NULL)
				{
					if (borne1->suivant != NULL)
					{
						borne2 = borne1->suivant;
						currentx = (int) borne1->xymin;
						currentx = currentx>clipping_x1?currentx:clipping_x1;
						while ((currentx < (int) borne2->xymin) && currentx < clipping_x2)
						{
							pixel_ptr += currentx + ydepart*hw_surface_get_size(surface).width;
							*pixel_ptr = ei_map_rgba(surface,&color);
							pixel_ptr -= currentx + ydepart*hw_surface_get_size(surface).width;
							currentx++;
						}
						borne1 = borne2->suivant;
					}
					else{borne1 = NULL;}
				}
			}
			ydepart++;
			maj_TCA(&TCA);																									// maj de TCA
	}
	for (size_t i = 0; i < surface_height; i++) {
		free(TC[i]);
	}
	free(TC);
	hw_surface_unlock(surface);
}

void ei_draw_text (ei_surface_t surface, const ei_point_t* where, const char* text, const ei_font_t font, const ei_color_t*	color, const ei_rect_t*	clipper){
	// crée une surface pour écrire le texte
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
	ei_rect_t text_surface_rect = hw_surface_get_rect(text_surface);
	// rectangle destination où afficher ledit texte
	ei_point_t dst_point = {where->x, where->y};
	ei_size_t dst_size = text_surface_rect.size;
	ei_rect_t dst_rect = {dst_point, dst_size};

	// rectangle source où copier le texte
	if (clipper != NULL){
		// si il y a un clipper, on va copier qu'une partie du texte :
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
		ei_copy_surface(surface, &dst_rect, text_surface, &src_rect, 1);
	}
	else{
		ei_copy_surface(surface, &dst_rect, text_surface, NULL, 1);
	}
	hw_surface_free(text_surface);
}


void ei_fill (ei_surface_t surface, const ei_color_t*	color, const ei_rect_t*	clipper){
	hw_surface_lock(surface);

	uint32_t color32 = ei_map_rgba(surface, color);
	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
	int i_min;
	int i_max;
	int j_min;
	int j_max;
	if (clipper == NULL){
		ei_size_t surface_size = hw_surface_get_size(surface);
		i_min = 0;
		i_max = surface_size.width;
		j_min = 0;
		j_max = surface_size.height;
	}
	else{
		i_min = clipper->top_left.x;
		i_max = clipper->size.width;
		j_min = clipper->top_left.y;
		j_max = clipper->size.height;
	}

	for(int i = i_min; i < i_max; i++){
		for(int j = j_min; j < j_max; j++){
			pixel_ptr += i + j*hw_surface_get_size(surface).width;
			*pixel_ptr = color32;
			pixel_ptr -= i + j*hw_surface_get_size(surface).width;
		}
	}

	hw_surface_unlock(surface);
}

int	ei_copy_surface (ei_surface_t destination, const ei_rect_t*	dst_rect, const ei_surface_t source, const ei_rect_t*	src_rect, const ei_bool_t	alpha){
	hw_surface_lock(destination);
	// @ du pixel courant de la surface source/destination
	uint32_t* pixel_ptr_dest = (uint32_t*)hw_surface_get_buffer(destination);
	uint32_t* pixel_ptr_src = (uint32_t*)hw_surface_get_buffer(source);

	// variables pour le parcours de la surface source
	int i_max = 0;
	int j_max = 0;
	ei_size_t size_src;
	ei_size_t size_dest;

	// variables pour le parcours de la surface destination
	int i_dest = 0;
	int j_dest = 0;

	// variables pour le parcours de la surface source
	int i_src = 0;
	int j_src = 0;


	// si on a un rectangle de destination
	if(dst_rect != NULL && src_rect == NULL){
		size_src = hw_surface_get_size(source);
		// si la surface source est plus grande que le rectangle de destination
		if (size_src.width > dst_rect->size.width || size_src.height > dst_rect->size.height){
			// impossible, on quitte
			printf("1 surface source trop grande, impossible de faire la copy gros beau gosse \n");
			return 0;
		}
		// on commence à parcourir la surface de destination,
		// à partir du rectangle de destination
		i_dest = dst_rect->top_left.x;
		j_dest = dst_rect->top_left.y;

		// on parcourt la surface source
		i_max = size_src.width;
		j_max = size_src.height;
	}

	// si on a un rectangle de source
	else if (dst_rect == NULL && src_rect != NULL){
		size_dest = hw_surface_get_size(destination);
		// si le rectangle de source est plus grand que la surface de destination
		if (src_rect->size.width > size_dest.width || src_rect->size.height > size_dest.height){
			// impossible, on quitte
			printf("2 rectangle source trop grande, impossible de faire la copy gros beau gosse \n");
			return 0;
		}
		// on commence à parcourir la surface de source,
		// à partir du rectangle de source
		i_src = src_rect->top_left.x;
		j_src = src_rect->top_left.y;

		// on parcourt le rectangle de source
		i_max = src_rect->size.width;
		j_max = src_rect->size.height;
	}

	// si on a un rectangle de destination et de source
	else if (dst_rect != NULL && src_rect != NULL){
		// si le rectangle de source est plus grand que le rectangle de destination
		if (src_rect->size.width > dst_rect->size.width || src_rect->size.height > dst_rect->size.height){
			// impossible, on quitte
			printf("3 rectangle source trop grande, impossible de faire la copy gros beau gosse \n");
		}
		// on commence à parcourir la surface de destination,
		// à partir du rectangle de destination
		i_dest = dst_rect->top_left.x;
		j_dest = dst_rect->top_left.y;

		// on commence à parcourir la surface de source,
		// à partir du rectangle de source
		i_src = src_rect->top_left.x;
		j_src = src_rect->top_left.y;

		// on parcourt le rectangle de source
		i_max = src_rect->size.width;
		j_max = src_rect->size.height;
	}

	// si on a pas de rectangle
	else{
		size_src = hw_surface_get_size(source);
		size_dest = hw_surface_get_size(destination);
		// si la surface de source est plus grande que la surface de destination
		if (size_src.width > size_dest.width || size_src.height > size_dest.height){
			// impossible, on quitte
			printf("4 surface source trop grande, impossible de faire la copy gros beau gosse \n");
		}
		// on parcourt la surface source
		i_max = size_src.width;
		j_max = size_src.height;
	}

	// parcourt la surface/rectangle source en fonction des parametres
	for (int i = 0; i < i_max; i++){
		for(int j = 0; j < j_max; j++){
			pixel_ptr_src += (i_src + i) + (j + j_src)*hw_surface_get_size(source).width;
			pixel_ptr_dest += (i_dest + i) + (j + j_dest)*hw_surface_get_size(destination).width;
			// si alpha est nul, on fait une copie exacte
			if (alpha == 0){
				*pixel_ptr_dest = *pixel_ptr_src;
			}
			// sinon, on fait une moyenne des couleurs sources et destination
			else{
				ei_color_t color_src = ei_map_color(source, pixel_ptr_src);
				ei_color_t color_dest = ei_map_color(source, pixel_ptr_dest);

				// calcule de la moyenne pour chaque couleur
				color_dest.red = ( color_src.alpha * color_src.red + (255 - color_src.alpha) * color_dest.red) / 255;
				color_dest.green = ( color_src.alpha * color_src.green + (255 - color_src.alpha) * color_dest.green) / 255;
				color_dest.blue = ( color_src.alpha * color_src.blue + (255 - color_src.alpha) * color_dest.blue) / 255;
				*pixel_ptr_dest = ei_map_rgba(destination, &color_dest);
			}
				pixel_ptr_src -= (i_src + i) + (j + j_src)*hw_surface_get_size(source).width;
				pixel_ptr_dest -= (i_dest + i) + (j + j_dest)*hw_surface_get_size(destination).width;
		}
	}

	hw_surface_unlock(destination);
	return 1;

}


ei_linked_point_t* arc(const ei_point_t centre, float rayon, int angle_debut, int angle_fin){
	// coordonnées x et y en fonction des paramètres
	int x = 0;
	int y = 0;
	int nbElems = abs(angle_debut-angle_fin);
	ei_linked_point_t* pts = malloc(sizeof(ei_linked_point_t)*nbElems+1);
	int i = 0;
	// on dessine un point pour chaque angle
	while(i < (nbElems-2)){
			x = rayon * cos((i+angle_debut)*(PI/180)) + centre.x;
			y = rayon * sin((i+angle_debut)*(PI/180)) + centre.y;
			pts[i].point.x = x; pts[i].point.y = y; pts[i].next = &pts[i+1];
			i++;
	}
	// avant dernier point pointe vers dernier
	pts[i-2].next = &pts[i-1];
	// dernier point
	x = rayon * cos((i+angle_debut)*(PI/180)) + centre.x;
	y = rayon * sin((i+angle_debut)*(PI/180)) + centre.y;
	pts[i-1].point.x = x; pts[i-1].point.y = y; pts[i-1].next = NULL;
	return pts;
}


ei_linked_point_t* rounded_frame(const ei_rect_t rectangle, float rayon, ei_bool_t* bords){
	// on va calculer l'arc pour chaque bord qu'on souhaite affrondir
	int angle_debut;
	int angle_fin;
	ei_point_t centre;
	// les bords, dans l'ordre : top-left, top-right, bottom-right, bottom-left
	ei_linked_point_t* bord1 = NULL;
	ei_linked_point_t* bord2 = NULL;
	ei_linked_point_t* bord3 = NULL;
	ei_linked_point_t* bord4 = NULL;

	// nombre d'de points dans un arc
	int nbElemsArc = 87;

	// si bords est nul, on arrondit tout les bords
	if(bords == NULL){
		bords = malloc(sizeof(ei_bool_t)*4);
		bords[0] = 1;
		bords[1] = 1;
		bords[2] = 1;
		bords[3] = 1;
	}

	// pour chaque bord, on va créer l'arc
	if(bords[0] == 1){
		angle_debut = 180;
		angle_fin = 270;
		centre.x = rectangle.top_left.x + rayon;
		centre.y = rectangle.top_left.y + rayon;
		bord1 = arc(centre, rayon, angle_debut, angle_fin);
		// printf("%i %i \n", bord1[0].point.x, bord1[0].point.y);
		// printf("%i %i \n", bord1[nbElemsArc].point.x, bord1[nbElemsArc].point.y);

	}
	if(bords[1] == 1){
		angle_debut = 270;
		angle_fin = 360;
		centre.x = rectangle.top_left.x + rectangle.size.width - rayon;
		centre.y = rectangle.top_left.y + rayon;
		bord2 = arc(centre, rayon, angle_debut, angle_fin);
		// printf("%i %i \n", bord2[0].point.x, bord2[0].point.y);
		// printf("%i %i \n", bord2[nbElemsArc].point.x, bord2[nbElemsArc].point.y);
	}
	if(bords[2] == 1){
		angle_debut = 0;
		angle_fin = 90;
		centre.x = rectangle.top_left.x + rectangle.size.width - rayon;
		centre.y = rectangle.top_left.y + rectangle.size.height - rayon;
		bord3 = arc(centre, rayon, angle_debut, angle_fin);
		// printf("%i %i \n", bord3[0].point.x, bord3[0].point.y);
		// printf("%i %i \n", bord3[nbElemsArc].point.x, bord3[nbElemsArc].point.y);
	}
	if(bords[3] == 1){
		angle_debut = 90;
		angle_fin = 180;
		centre.x = rectangle.top_left.x + rayon;
		centre.y = rectangle.top_left.y + rectangle.size.height - rayon;
		bord4 = arc(centre, rayon, angle_debut, angle_fin);
		// printf("%i %i \n", bord4[0].point.x, bord4[0].point.y);
		// printf("%i %i \n", bord4[nbElemsArc].point.x, bord4[nbElemsArc].point.y);
	}

	// rectangle de base sous forme de linked_point
	ei_linked_point_t* rectangle_rounded = malloc(sizeof(ei_linked_point_t)*4);
	// les points du rectangle dans l'ordre : topleft, topright, bottomleft, bottomright
	rectangle_rounded[0].point.x = rectangle.top_left.x; rectangle_rounded[0].point.y = rectangle.top_left.y; rectangle_rounded[0].next = &rectangle_rounded[1];
	rectangle_rounded[1].point.x = rectangle.top_left.x+rectangle.size.width; rectangle_rounded[1].point.y = rectangle.top_left.y; rectangle_rounded[1].next = &rectangle_rounded[2];
	rectangle_rounded[2].point.x = rectangle.top_left.x+rectangle.size.width; rectangle_rounded[2].point.y = rectangle.top_left.y + rectangle.size.height; rectangle_rounded[2].next = &rectangle_rounded[3];
	rectangle_rounded[3].point.x = rectangle.top_left.x; rectangle_rounded[3].point.y = rectangle.top_left.y + rectangle.size.height; rectangle_rounded[3].next = NULL;

	// dernier point du rectangle, pour ne pas boucler infiniment
	ei_linked_point_t* last_point = malloc(sizeof(ei_linked_point_t));
	last_point[0].point.x = rectangle_rounded[0].point.x; last_point[0].point.y = rectangle_rounded[0].point.y; last_point[0].next = NULL;

	// on relie les points pour créer le rectangle avec les bords arrondis
	// si on a souhaité arrondir les deux bords qui se suivent
	// if (bord1 != NULL && bord2 != NULL){
	// 	// alors on relie les listes de points
	// 	rectangle_rounded[0].point.x = bord1[0].point.x;
	// 	rectangle_rounded[0].point.y = bord1[0].point.y;
	// 	rectangle_rounded[0].next = &bord1[0];
	// 	bord1[nbElemsArc].next = &bord2[0];
	// 	printf("%i %i \n", bord1[nbElemsArc].next->point.x, bord1[nbElemsArc].next->point.y);
	// }
	// // sinon, on va relier avec le bord non arrondi
	// else{
	// 	rectangle_rounded[3].next = &rectangle_rounded[0];
	// }
	//
	// if(bord2 != NULL && bord3 != NULL){
	// 	bord2[nbElemsArc].next = &bord3[0];
	// 	printf("%i %i \n", bord2[nbElemsArc].next->point.x, bord2[nbElemsArc].next->point.y);
	// }
	// else{
	// 	bord2[nbElemsArc].next = NULL;
	// }
	//
	// if(bord3 != NULL && bord4 != NULL){
	// 	bord3[nbElemsArc].next = &bord4[0];
	// 	printf("%i %i \n", bord3[nbElemsArc].next->point.x, bord3[nbElemsArc].next->point.y);
	// }
	// else{
	// 	bord3[nbElemsArc].next = NULL;
	// }
	//
	// if(bord4 != NULL && bord1 != NULL){
	// 	bord4[nbElemsArc].next = NULL;
	// }
	// else{
	// 	bord4[nbElemsArc].next = NULL;
	// }
	if (bord1 != NULL){
		rectangle_rounded[0].point.x = bord1[0].point.x; rectangle_rounded[0].point.y = bord1[0].point.y;  rectangle_rounded[0].next = &bord1[0];
		last_point[0].point.x = rectangle_rounded[0].point.x; last_point[0].point.y = rectangle_rounded[0].point.y; last_point[0].next = NULL;
		bord1[nbElemsArc].next = &rectangle_rounded[1];
	}

	if (bord2 != NULL){
		rectangle_rounded[1].point.x = bord2[0].point.x; rectangle_rounded[1].point.y = bord2[0].point.y; rectangle_rounded[1].next = &bord2[0];
		bord2[nbElemsArc].next = &rectangle_rounded[2];
	}

	if(bord3 != NULL){
		rectangle_rounded[2].point.x = bord3[0].point.x; rectangle_rounded[2].point.y = bord3[0].point.y; rectangle_rounded[2].next = &bord3[0];
		bord3[nbElemsArc].next = &rectangle_rounded[3];
	}

	if(bord4 != NULL){
		rectangle_rounded[3].point.x = bord4[0].point.x; rectangle_rounded[3].point.y = bord4[0].point.y; rectangle_rounded[3].next = &bord4[0];
		bord4[nbElemsArc].next = last_point;
	}
	else{
		rectangle_rounded[3].next = last_point;
	}

	return rectangle_rounded;
}


void ei_draw_button (ei_surface_t surface, const ei_linked_point_t* first_point, const ei_color_t color, const ei_rect_t* clipper, float rayon){
	hw_surface_lock(surface);


	hw_surface_unlock(surface);
}

#include "../include/ei_draw.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


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

void ei_draw_polyline (ei_surface_t surface, const ei_linked_point_t*	first_point, const ei_color_t color, const ei_rect_t* clipper){
	hw_surface_lock(surface);

	uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);

	// while(first_point->next != NULL){
	// 	printf("%i\n", first_point->point.x);
	// 	first_point = first_point->next;
	// }
	// first_point = first_point->next;
	// point de départ
	ei_point_t current = first_point->point;
	// point d'arrivée
	ei_point_t arrivee = first_point->next->point;

	float erreur = 0;
	int deltax = abs(arrivee.x - current.x);
	int deltay = abs(arrivee.y - current.y);

	if(deltax == 0){

	}else if(deltay == 0){

	}else if ( deltax > deltay){
		// on incrémente x de 1

		while((current.x != arrivee.x) && (current.y != arrivee.y)){
			current.x++;
			erreur += (float)deltay/deltax;

			if (erreur > 0.5){
				current.y++;
				erreur--;
			}
			pixel_ptr += current.x + current.y*800;
			*pixel_ptr = ei_map_rgba(surface,&color);
			pixel_ptr -= current.x + current.y*800;
		}
	}
	else{
		// on incrémente y de 1
		while((current.x != arrivee.x) && (current.y != arrivee.y)){
			current.y++;
			erreur += (float)deltax/deltay;

			if (erreur > 0.5){
				current.x++;
				erreur--;
			}
			pixel_ptr += current.x + current.y*800;
			*pixel_ptr = ei_map_rgba(surface,&color);
			pixel_ptr -= current.x + current.y*800;
		}
	}

	hw_surface_unlock(surface);
	hw_surface_update_rects(surface, NULL);
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

ei_linked_point_t *dernier_point(ei_linked_point_t* cellule)	// Renvoie le dernier point d'une liste chainée de points
{
		ei_linked_point_t* courant = cellule;
		while (courant->next != NULL)
		{
			courant = courant->next;
		}
		return courant;
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
			tri_TCA(&((*TCA)->suivant));
		}
	}
}

void maj_TCA(ei_cellule_t** TCA)		// Met à jour les abscisses d'intersection
{
	ei_cellule_t *courant = *TCA;
	if (courant != NULL)
	{
		while (courant->suivant != NULL)
		{
			courant->xymin = courant->xymin - courant->pente;
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

	ei_linked_point_t *courant = malloc(sizeof(ei_linked_point_t));
	courant->point = first_point->point;
	courant->next = first_point->next;

	ei_linked_point_t *retour_au_premierpoint = malloc(sizeof(ei_linked_point_t));
	retour_au_premierpoint->point = first_point->point;
	retour_au_premierpoint->next = NULL;
	dernier_point(courant)->next = retour_au_premierpoint;

	ei_cellule_t** TC = malloc(sizeof(struct ei_cellule_t) * hw_surface_get_size(surface).height);
	ei_cellule_t* TCA = malloc(sizeof(struct ei_cellule_t));
	TCA = NULL;
	int ydepart = hw_surface_get_size(surface).height;
	printf("%i\n", hw_surface_get_size(surface).height);

	while (courant->next != NULL)
	{
			int x1 = courant->point.x;
			int y1 = courant->point.y;
			int x2 = courant->next->point.x;
			int y2 = courant->next->point.y;
			if ((y2-y1)!=0)
			{
				uint32_t ymin = y1<y2?y1:y2;
				ydepart = ymin<ydepart?ymin:ydepart;
				ei_cellule_t* nouveau = malloc(sizeof(struct ei_cellule_t));
				nouveau->ymax =  y1>y2?y1:y2;
				nouveau->xymin = y1<y2?x1:x2;
				nouveau->pente = (x2-x1)/(y2-y1);
				nouveau->suivant = NULL;
				//printf("%i\n", ymin);

				if (TC[ymin] == NULL)
				{
					printf("a\n");
					TC[ymin] = nouveau;
				}
				else
				{
					printf("b\n");
					derniere_cellule(TC[ymin])->suivant = nouveau;
				}
			}
			courant = courant->next;
			printf("%p\n",courant->next);
	}
		//printf("%i\n",TC[200]->ymax);
	printf("%p\n",TC);
	printf("%p\n",TCA);
	while ((TC_non_vide(TC, hw_surface_get_size(surface).height) == 1) || (nbr_TCA(&TCA) != 0))
	{
			printf("entre\n");
			if (TC[ydepart] != NULL)
			{
				printf("%i\n",TC[ydepart]->ymax);
			}
			printf("entre2\n");
			if (derniere_cellule(TCA) == NULL)
			{
				TCA = TC[ydepart];
			}
			else { derniere_cellule(TCA)->suivant = TC[ydepart];}
			printf("entre3\n");
			TC[ydepart] = NULL;
			printf("entre4\n");
			supr_TCA(&TCA, ydepart);
			printf("entre5\n");
			tri_TCA(&TCA);
			printf("entre6\n");
			ei_cellule_t *borne1 = TCA;
			printf("entre7\n");
			ei_cellule_t *borne2;
			printf("entre8\n");
			int currentx;
			while (borne1 != NULL)
			{
				borne2 = borne1->suivant;
				currentx = (int) borne1->xymin;
				while (currentx < (int) borne2->xymin)
				{
					pixel_ptr += currentx + ydepart*800;
					*pixel_ptr = ei_map_rgba(surface,&color);
					pixel_ptr -= currentx + ydepart*800;
					currentx++;
				}
				borne1 = borne2->suivant;
			}
			ydepart++;
			printf("%i\n", ydepart);
			maj_TCA(&TCA);
			printf("derniere etape %i, %i\n", TC_non_vide(TC, hw_surface_get_size(surface).height), nbr_TCA(&TCA));
	}
	printf("sort\n");
	hw_surface_unlock(surface);
	hw_surface_update_rects(surface, NULL);
}

void ei_draw_text (ei_surface_t surface, const ei_point_t* where, const char* text, const ei_font_t font, const ei_color_t*	color, const ei_rect_t*	clipper){

}


void ei_fill (ei_surface_t surface, const ei_color_t*	color, const ei_rect_t*	clipper){

}

int	ei_copy_surface (ei_surface_t destination, const ei_rect_t*	dst_rect, const ei_surface_t source, const ei_rect_t*	src_rect, const ei_bool_t	alpha){
	return 0;
}

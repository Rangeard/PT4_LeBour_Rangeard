#ifndef H_JDV_SDL
#define H_JDV_SDL

#include "../h/jdv_util.h"

#include <SDL/SDL.h>

static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 25;

void initSDL(void);
void affJeuSDL(void);


#endif

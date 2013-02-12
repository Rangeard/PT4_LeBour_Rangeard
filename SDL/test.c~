#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
  
int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
 
    SDL_Init(SDL_INIT_VIDEO);
 
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE); // On tente d'ouvrir une fenêtre
    if (ecran == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
     
    SDL_WM_SetCaption("Ma super fenêtre SDL !", NULL);
 
    pause();
 
    SDL_Quit();
 
    return EXIT_SUCCESS;
}

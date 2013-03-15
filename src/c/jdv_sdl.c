#include "../h/jdv_sdl.h"

void initSDL(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(N*TAILLE+10, N*TAILLE+10, 32, SDL_HWSURFACE); // On tente d'ouvrir une fenêtre
    if (ecran == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("Jeu de la vie", NULL);
}

void affJeuSDL(void)
{

    int i,j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            cellule = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE-2, TAILLE-2, 32, 0, 0, 0, 0);
            position.x = (i)*TAILLE+1;
            position.y = (j)*TAILLE+1;
            if(board[i][j].vivant)
                SDL_FillRect(cellule, NULL, SDL_MapRGB(ecran->format, 51, 255, 51));
            else
                SDL_FillRect(cellule, NULL, SDL_MapRGB(ecran->format, 20, 20, 20));

            SDL_BlitSurface(cellule, NULL, ecran, &position); // Collage de la surface sur l'écran
        }
    }
    SDL_Flip(ecran); // Mise à jour de l'écran
}

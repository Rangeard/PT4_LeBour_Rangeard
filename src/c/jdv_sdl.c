#if defined(AFF)
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

void affJeuSDL(struct cellule **board)
{

    int i,j;
    for(i=1; i<N-1; i++)
    {
        for(j=1; j<N-1; j++)
        {
            cellules = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE-2, TAILLE-2, 32, 0, 0, 0, 0);
            position.x = (i)*TAILLE+1;
            position.y = (j)*TAILLE+1;
            if(board[i][j].vivant)
                SDL_FillRect(cellules, NULL, SDL_MapRGB(ecran->format, 51, 255, 51));
            else
                SDL_FillRect(cellules, NULL, SDL_MapRGB(ecran->format, 20, 20, 20));

            SDL_BlitSurface(cellules, NULL, ecran, &position); // Collage de la surface sur l'écran
        }
    }
    
    while(SDL_PollEvent(&event) )
	{
		switch(event.type)
		{
			case SDL_QUIT:
				SDL_Quit();
			break;
		}
	}
    SDL_Flip(ecran); // Mise à jour de l'écran
}
#endif

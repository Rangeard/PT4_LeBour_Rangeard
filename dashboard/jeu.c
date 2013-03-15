#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>


struct element{
	bool vivant;
	int nbVoisins;
};

static struct element **board = NULL;

static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule[2][2]; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 10;
static int N; //soit de N-2*N-2 affichee a cause du tore

void erreur(char *s)
{
	perror(s);
	exit(-1);
}

void initSDL(void)
{
	SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE); // On tente d'ouvrir une fenêtre
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
	for(i=0;i<N-2;i++)
	{
		for(j=0;j<N-2;j++)
		{
			cellule[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE*2, TAILLE*2, 32, 0, 0, 0, 0);
			position.x = (i+1)*20;
			position.y = (j+1)*20;
			if(board[i][j].vivant)				
				SDL_FillRect(cellule[i][j], NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
			else
				SDL_FillRect(cellule[i][j], NULL, SDL_MapRGB(ecran->format, 0, 0, 0));	
		
			SDL_BlitSurface(cellule[i][j], NULL, ecran, &position); // Collage de la surface sur l'écran
		}
	}
    SDL_Flip(ecran); // Mise à jour de l'écran 
}

//Mise en en place du Tor
void tor(void)
{
	int i;
	//copie de la première ligne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		board[N-1][i] = board[1][i];
	//copie de la dernière ligne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		board[0][i] = board[N-2][i];
	//copie de la première colonne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		board[i][N-1] = board[i][1];
	//copie de la dernière colonne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		board[i][0] = board[i][N-2];
}

int getNbVoisins(int i, int j)
{
	int res = 0;
	if (board[i][j-1].vivant) res++; //gauche
	if (board[i][j+1].vivant) res++; //droite
	if (board[i-1][j-1].vivant) res++; //haut gauche 
	if (board[i-1][j].vivant) res++;  //haut
	if (board[i-1][j+1].vivant) res++; //haut droite	
	if (board[i+1][j-1].vivant) res++; //bas gauche
	if (board[i+1][j].vivant) res++; //bas
	if (board[i+1][j+1].vivant) res++; //bas droite	
	return res;
}

void initJeu(void)
{
	int i,j;
	srandom(time(NULL));
	
	for (i=1;i<N-1;i++)
		for (j=1;j<N-1;j++)
		{
			if (random()%2)
				board[i][j].vivant = true;
			else
				board[i][j].vivant = false;
		}
	
	for (i = 0; i < N-1; i++)
	{
		for (j = 0; j < N-1; j++)
		{
			board[i][j].nbVoisins = getNbVoisins(i,j);
		}	
	}
	
	tor();
}

void boucle(void)
{
	int i,j;
	int changement = 0;
	while(changement >= 0)
	{
		changement = 0;
		for(i=1; i<N-1; i++)
		{
			for(j=1;j<N-1;j++)
			{
				board[i][j].nbVoisins = getNbVoisins(i,j);
				if (board[i][j].nbVoisins == 3 && !board[i][j].vivant)
					board[i][j].vivant = true;
				else if ((board[i][j].nbVoisins<=1 || board[i][j].nbVoisins>=4) && board[i][j].vivant)
					board[i][j].vivant = false;
				else
				{
					changement++;
				}
			}
		}
		SDL_WaitEvent(&event); /* On attend un événement qu'on récupère dans event */
        
		tor();
		sleep(1);
		affJeuSDL();
		//Condition d'arrêt
		if(changement == (N-2)*(N-2))
			changement = -1;
		switch(event.type) /* On teste le type d'événement */
        {
            case SDL_QUIT: /* Si c'est un événement QUITTER */
                changement = -1; 
			break;
        }
	}
}

int main(int argc, char ** argv)
{
	/*
	switch (argc)
	{
		case 0:
			printf("Synopsis : ./jeu <taille> <nbThreads> <graphiquesON/OFF> /n(voir --help pour plus d'informations)\n");
			exit(0);
			break;
		case 1:
			if(strcmp(argv[i],"--help")==0){
				printf("####### AIDE ########\n\nSynopsis : ./jeu <taille> <nbThreads> <graphiquesON/OFF> \nDefaut :\n	- nbThreads = X\n	- graphiques = ON\n\n");
				exit(0);
			}
			else if
		default:
			
	}*/
		int i,j;
		
	N = atoi(argv[1]);
	board = (struct element **) malloc(N*sizeof(struct element *));
	for (i=0;i<N;i++){
		board[i] = (struct element *) malloc(N*sizeof(struct element));
	}
	
	initJeu();
	initSDL();
	affJeuSDL();
	boucle();
		//Libération des surfaces
	for(i=0;i<N-2;i++)
		for(j=0;j<N-2;j++)
			SDL_FreeSurface(cellule[i][j]); 
	// desallocation matrice
	for (i=0;i<N;i++){
		free(board[i]);
	}
	free(board);
	
	SDL_Quit();	
	return 0; 
}

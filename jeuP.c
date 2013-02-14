#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <SDL/SDL.h>
#define N 10 //soit de 8x8
#define true (1==1)
#define false !true
static char mAff[N][N];
static char mCal[N][N];
static int mUni[N][N];
/*
static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule[(N-2)*(N-2)]; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static const int TAILLE = 10;
*/

/*
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
	
	for(i=1;i<N-1;i++)
	{
		for(j=1;j<N-1;j++)
		{
			if(mCal[i][j])
			{
				cellule[((i-1)*N)+j] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE, TAILLE, 32, 0, 0, 0, 0);
				position.x = i*10;
				position.y = j*10;
				SDL_FillRect(cellule[((i-1)*N)+j], NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
				SDL_BlitSurface(cellule[((i-1)*N)+j], NULL, ecran, &position); // Collage de la surface sur l'écran
			}
		}
	}
    
    SDL_Flip(ecran); // Mise à jour de l'écran 
}
*/

void erreur(char *s)
{
	perror(s);
	exit(-1);
}

void affJeu(void)
{
	int i,j;
	printf("\033[H");
	printf("JEU DE LA VIE :\n\n");
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			//affiche coté test tore
			if((i==0 || i==N-1 || j==0 || j==N-1) && mUni[i][j])
				printf("()");
			else if((i==0 || i==N-1 || j==0 || j==N-1) && !mUni[i][j])
				printf("--");
			//aff std
			else if(mUni[i][j])
				printf("{}");
			else
				printf("  ");
		}	
		printf("\n");
	}
}


//Mise en en place du Tor
void tor(void)
{
	int i;
	//Les 4 coins sont fixé à FAUX (pas de cellules possible)
	/*mCal[0][0] = false; //Coin haut gauche
	mCal[0][N-1] = false; //Coin haut droit
	mCal[N-1][0] = false; //Coin bas gauche
	mCal[N-1][N-1] = false; //Coin bas droit*/
	//copie de la première ligne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		mUni[N-1][i] = mUni[1][i];
	//copie de la dernière ligne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		mUni[0][i] = mUni[N-2][i];
	//copie de la première colonne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		mUni[i][N-1] = mUni[i][1];
	//copie de la dernière colonne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		mUni[i][0] = mUni[i][N-2];
}
/*
void echange(void)
{
	int i,j;
	for(i=0; i<N; i++)
		for(j=0;j<N;j++)
			mCal[i][j] = mAff[i][j];
}
*/
void initJeu(void)
{
	int i,j;
	srandom(time(NULL));
	for (i=1;i<N-1;i++)
		for (j=1;j<N-1;j++)
		{
			if (random()%2)
				mUni[i][j] = 0;
			else
				mUni[i][j] = -1;
		}
	
	tor();
	echange();
	
}

int nbVoisins(int i, int j)
{
	int res = 0;
	if (mUni[i][j-1]) res++; //gauche
	if (mUni[i][j+1]) res++; //droite
	if (mUni[i-1][j-1]) res++; //haut gauche 
	if (mUni[i-1][j]) res++;  //haut
	if (mUni[i-1][j+1]) res++; //haut droite	
	if (mUni[i+1][j-1]) res++; //bas gauche
	if (mUni[i+1][j]) res++; //bas
	if (mUni[i+1][j+1]) res++; //bas droite	
	return res;
}


void boucle(void)
{
	int i,j,nbv;
	int changement = 0;
	while(changement >= 0)
	{
		changement = 0;
		for(i=1; i<N-1; i++)
		{
			for(j=1;j<N-1;j++)
			{
				nbv = nbVoisins(i,j);
				if (nbv == 3 && !mCal[i][j])
					mAff[i][j] = true;
				else if ((nbv<=1 || nbv>=4) && mCal[i][j])
					mAff[i][j] = false;
				else
				{
					mAff[i][j] = mCal[i][j];
					changement++;
					
				}
			
			}
		}
		
		tor();
		//Echange des matrices
		echange();
		sleep(1);
		//affJeu();
		affJeuSDL();
		//Condition d'arrêt
		if(changement == (N-2)*(N-2))
			changement = -1;
		
	}
	/*for(i=1;i<N-1;i++)
		for(j=1;j<N-1;j++)
			SDL_FreeSurface(cellule[((i-1)*N)+j]); // Libération de la surface
    SDL_Quit();	*/
}

int main()
{
	pid_t pid;
	if ((pid = fork()) < 0)
		erreur("Erreur lors de la creation du processus fils");

	if (pid == 0) 		
		execlp("clear","clear",NULL);

	wait(NULL);

	
	initJeu();
	affJeu();
	//initSDL();
	//affJeuSDL();
	boucle();
	return 0; 
}

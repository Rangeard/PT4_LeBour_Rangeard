#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#define N 30 //soit de 8x8
static char mAff[N][N];
static char mCal[N][N];
static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const float TAILLE = 30.0;

void erreur(char *s)
{
	perror(s);
	exit(-1);
}

void initSDL(void)
{
	SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(N*TAILLE, N*TAILLE, 32, SDL_OPENGL); // On tente d'ouvrir une fenêtre
    if (ecran == NULL) // Si l'ouverture a échoué, on le note et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
	SDL_WM_SetCaption("Jeu de la vie", NULL);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(70,(double)N*TAILLE/N*TAILLE,1,1000);
	gluOrtho3D(0,N*TAILLE,0,N*TAILLE);
}

void affJeuSDL(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluLookAt(3,3,3,0,0,0,0,0,1);
	glPointSize(TAILLE);
	int i,j,x,y;
	glBegin(GL_POINTS);
	for(i=0;i<N-2;i++)
	{
		x = (i+1)*TAILLE+1;
		for(j=0;j<N-2;j++)
		{
			y = ((j+1)*TAILLE)+1;
			if(mCal[i][j])				
				{
	    			 glColor3ub(0,255,15);    glVertex3d(x,y,0);
        		}
			else
				{
					glColor3ub(20,20,20);    glVertex3d(x,y,0);
				}
		}
	}
	glEnd();
	glFlush();
   	SDL_GL_SwapBuffers();
    
}

//Mise en en place du Tor
void tor(void)
{
	int i;
	//Les 4 coins sont fixé à FAUX (pas de cellules possible)
	mCal[0][0] = false; //Coin haut gauche
	mCal[0][N-1] = false; //Coin haut droit
	mCal[N-1][0] = false; //Coin bas gauche
	mCal[N-1][N-1] = false; //Coin bas droit
	//copie de la première ligne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		mAff[N-1][i] = mAff[1][i];
	//copie de la dernière ligne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		mAff[0][i] = mAff[N-2][i];
	//copie de la première colonne (1) sur la dernière (N-1)
	for(i=1;i<N-1;i++)
		mAff[i][N-1] = mAff[i][1];
	//copie de la dernière colonne (N-1) sur la première (0)
	for(i=1;i<N-1;i++)
		mAff[i][0] = mAff[i][N-2];
}

void echange(void)
{
	int i,j;
	for(i=0; i<N; i++)
		for(j=0;j<N;j++)
			mCal[i][j] = mAff[i][j];
}

void initJeu(void)
{
	int i,j;
	srandom(time(NULL));
	for (i=1;i<N-1;i++)
		for (j=1;j<N-1;j++)
		{
			if (random()%2)
				mAff[i][j] = true;
			else
				mAff[i][j] = false;
		}	
	tor();
	echange();	
}

int nbVoisins(int i, int j)
{
	int res = 0;
	if (mCal[i][j-1]) res++; //gauche
	if (mCal[i][j+1]) res++; //droite
	if (mCal[i-1][j-1]) res++; //haut gauche 
	if (mCal[i-1][j]) res++;  //haut
	if (mCal[i-1][j+1]) res++; //haut droite	
	if (mCal[i+1][j-1]) res++; //bas gauche
	if (mCal[i+1][j]) res++; //bas
	if (mCal[i+1][j+1]) res++; //bas droite	
	return res;
}

void boucle(void)
{
	int i,j,nbv,changement = 0;
	while(changement>-1)
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
		echange(); //Echange des matrices
		sleep(1);
		affJeuSDL();
		if(changement == (N-2)*(N-2)) //Condition d'arrêt
			changement = -1;
	
		while(SDL_PollEvent(&event) )
			if( event.type == SDL_QUIT ) 
					changement = -1;//On quitte le programme
	SDL_FreeSurface(cellule);
	}
	SDL_FreeSurface(cellule);
    SDL_Quit();	
}

int main()
{
	initJeu();
	initSDL();
	affJeuSDL();
	boucle();
	return 0; 
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include <math.h>
#define N 38 //soit de 8x8
static char mAff[N][N];
static char mCal[N][N];
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 15;
static float angleX = 0, angleY = 0, angleZ = 0;
void erreur(char *s)
{
	perror(s);
	exit(-1);
}

void initSDL(void)
{
	SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("Jeu de la Vie (3D Torus)", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
	
	glMatrixMode( GL_PROJECTION );
    //glLoadIdentity();
	
    gluPerspective(10,(double)640/480,1,100);
    //glEnable(GL_DEPTH_TEST);
}

void affJeuSDL(void)
{
	
	int i,j;
	float x,y,z,angleG,angleP,vectorY;
	float rayonInt = 1;
	float rayonExt = 3;
	float xCentre=0,yCentre=0,zCentre=0; //Centre du Tor
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(2,4,7,-0.5,-0.25,-1,0,0,1);
	/*
	angleX+=5;
	angleZ+=1;
	glRotated(angleX,1,0,0);
	glRotated(angleY,0,1,0);
	glRotated(angleZ,0,0,1);  
	*/
	glPointSize(3);
    glBegin(GL_POINTS);
		x = xCentre;
		y = yCentre;
		z = zCentre;
	 	for(i=1; i<N-1; i++)
		{
			angleG = (360/((N-1)-1))*(i-1);
			x = x + (0.5*(float)sin((double)angleG));
			y = y + (0.5*(float)cos((double)angleG));
			z = 0;
			glColor3ub(255,255,255);
			glVertex3d(x,y,z);
			for(j=1; j<N-1; j++)
			{
				if(mCal[i][j])
					glColor3ub(0,255,0);
				else
					glColor3ub(20,20,20);
				angleP = (360/((N-1)-1))*(j-1);
				vectorY = y +(0.25*(float)cos((double)angleP));
				z = z + (0.25*(float)sin((double)angleP));
				glVertex3d(x,vectorY,z);
			}
		}
	glEnd();	
	/*
 	glBegin(GL_LINES);
		glColor3ub(255,255,255); glVertex3d(0,0,0); glVertex3d(1,0,0);//X
		glColor3ub(0,250,0); glVertex3d(0,0,0); glVertex3d(0,1,0); //Y
		glColor3ub(0,0,250); glVertex3d(0,0,0); glVertex3d(0,0,1); //Z
	glEnd();
	*/
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
		{
			switch(event.type)
       			 {
			    case SDL_QUIT:
			    	changement = -1;//On quitte le programme
			    break;
			    case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_UP: 
							angleZ += 1;
						break;
						case SDLK_LEFT: 
							angleX += 1;
						break;
						case SDLK_RIGHT: 
							angleY += 1;
						break;
		    		}
				}
		}
	}
    SDL_Quit();	
}

int main (int argc, char **argv)
{
	initJeu();
	initSDL();
	affJeuSDL();
	boucle();
	return 0; 
}

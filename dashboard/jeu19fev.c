#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>
//#include <fcntl.h>
//#include <sys/ipc.h>
//#include <sys/shm.h> 
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>
#define N 10 //soit de 8x8
/* Nombre de threads (attention ce nombre doit etre un multiple de N*N) */
#define NB_THREADS 2

/* Nombre de cellules traitees par un thread */
#define NB_LIGNES_PAR_THREADS (N-2)/NB_THREADS
static bool mAff[N][N];
static bool mCal[N][N];
static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 25;

/* Nombre de threads lances */
static int nb_th_lance = 0;
/* Nombre de threads ayant termines leur traitement */
static int nb_th_finis = 0;
/* Mutex 1 */
static pthread_mutex_t m1;
/* Mutex 2 */
static pthread_mutex_t m2;
/* Variable condition */
static pthread_cond_t cond;

void erreur(char *s)
{
  perror(s);
	exit(-1);
}

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
	for(i=1;i<N-1;i++)
	{
		for(j=1;j<N-1;j++)
		{
			cellule = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE-2, TAILLE-2, 32, 0, 0, 0, 0);
			position.x = (i)*TAILLE+1;
			position.y = (j)*TAILLE+1;
			if(mCal[i][j])				
				SDL_FillRect(cellule, NULL, SDL_MapRGB(ecran->format, 51, 255, 51));
			else
				SDL_FillRect(cellule, NULL, SDL_MapRGB(ecran->format, 20, 20, 20));	
		
			SDL_BlitSurface(cellule, NULL, ecran, &position); // Collage de la surface sur l'écran
		}
	}
    SDL_Flip(ecran); // Mise à jour de l'écran
}

//Mise en en place du Tor
void tor(void)
{
	int i;
	//Coin bas droite
	mAff[N][N] = mAff[1][1];
	//Coin haut gauche
	mAff[0][0] = mAff[N-1][N-1];
	//Coin haut droite
	mAff[0][N] = mAff[N-1][1];
	//Coin bas gauche
	mAff[N][0] = mAff[1][N-1];
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
			//~ if (random()%2)
				//~ mAff[i][j] = true;
			//~ else
				mAff[i][j] = false;
		}
		
		mAff[1][1]=true;	
		mAff[1][2]=true;	
		mAff[1][3]=true;	
		mAff[3][1]=true;	
		mAff[4][1]=true;	
		mAff[4][4]=true;	
		mAff[1][4]=true;	
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

void * thread(void *a)
{
	int i,j,nbv;
	int numThread;
	numThread = -1;
	bool fin = true;

	/* On incremente le nombre de threads lances
	 * et on assigne un numero au thread courant
	 */
	pthread_mutex_lock(&m1);
	numThread = nb_th_lance ;
	nb_th_lance++;
	//printf("MUTEX1\n  numThread = %d\n  nb_th_lance = %d\n",numThread,nb_th_lance);
	pthread_mutex_unlock(&m1);
	
	while(fin)
	{
		//printf("WHILE\n    TID : %d - Num : %d\n",(unsigned int)pthread_self(),numThread);
		for(i=(numThread*NB_LIGNES_PAR_THREADS)+1; i<(numThread*NB_LIGNES_PAR_THREADS)+NB_LIGNES_PAR_THREADS+1; i++)
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
				}
			}
		}
	
		pthread_mutex_lock(&m2);
	    //printf("MUTEX\n    TID : %d - Num : %d\n",(unsigned int)pthread_self(),numThread);
		nb_th_finis++;
		
		if (nb_th_finis == NB_THREADS)
		{
			tor();
			echange();
			affJeuSDL();
			nb_th_finis = 0;
			pthread_cond_broadcast(&cond);
		}
		else{
			pthread_cond_wait(&cond,&m2);
		}
		
		while(SDL_PollEvent(&event) ){
			if( event.type == SDL_QUIT ) 
					fin = false;//On quitte le programme
		}
		
		pthread_mutex_unlock(&m2);
		//sleep(1);
	}
	SDL_FreeSurface(cellule);
    SDL_Quit();
    pthread_exit(NULL);
    return NULL;
}

int main()
{
	int i=0;
	struct timeval tpDeb;
	struct timeval tpFin;
	//Creation des threads
	pthread_t th[NB_THREADS];

	/* Creation des mutex et de la variable condition */
	pthread_mutex_init(&m1,NULL);
	pthread_mutex_init(&m2,NULL);
	pthread_cond_init(&cond,NULL);
	
	initJeu();
	initSDL();
	affJeuSDL();
	gettimeofday(&tpDeb,NULL);
	printf("Tps dbt: %d\n" ,tpDeb);
	/* Creation des threads */
	for (i=0;i<NB_THREADS;i++){
		pthread_create(&(th[i]),NULL,thread,NULL);
	}

	/* Attente de la fin de tous les threads */
	for (i=0;i<NB_THREADS;i++){
		pthread_join(th[i],NULL);
	}
	gettimeofday(&tpFin,NULL);
	printf("Tps fin: %d\n" ,tpFin);
	
	long tt = tpFin.tv_usec - tpDeb.tv_usec;
	
	printf("TOTAL = %d\n",(int)tt);
	/* Destruction des mutex et de la variable condition */
	pthread_mutex_destroy(&m1);
	pthread_mutex_destroy(&m2);

	pthread_cond_destroy(&cond);
	return 0; 
}

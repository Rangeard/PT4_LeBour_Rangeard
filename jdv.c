#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>

/* Nombre de threads (attention ce nombre doit etre un multiple de N*N) */
#define NB_THREADS 4

/* Nombre de cellules traitees par un thread */
#define NB_LIGNES_PAR_THREADS (N-2)/NB_THREADS

struct element{
	bool vivant;
	int nbVoisins;
};

static struct element **board = NULL;


static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 25;
static int N; //soit de N-2*N-2 affichee a cause du tore

/* Nombre de cases (cellules) n'ayant pas changees entre 2 instants t 
   cette variable permet d'arreter le jeu lorsque celui-ci ne change plus */
static int cases_nonchangees = 0;
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
			if(board[i][j].vivant)				
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
	board[N][N] = board[1][1];
	
	//Coin haut gauche
	board[0][0] = board[N-1][N-1];
	
	//Coin haut droite
	board[0][N] = board[N-1][1];
	
	//Coin bas gauche
	board[N][0] = board[1][N-1];
	
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

void echange(void)
{
	int i,j;
	for(i=0; i<N; i++)
		for(j=0;j<N;j++)
			board[i][j] = board[i][j];
}

void majVoisins(int i, int j, bool vivant){
	if(vivant){
		board[i][j-1].nbVoisins++; //gauche
		board[i][j+1].nbVoisins++; //droite
		board[i-1][j-1].nbVoisins++; //haut gauche 
		board[i-1][j].nbVoisins++;  //haut
		board[i-1][j+1].nbVoisins++; //haut droite	
		board[i+1][j-1].nbVoisins++; //bas gauche
		board[i+1][j].nbVoisins++; //bas
		board[i+1][j+1].nbVoisins++;
	}
	else{
		board[i][j-1].nbVoisins--; //gauche
		board[i][j+1].nbVoisins--; //droite
		board[i-1][j-1].nbVoisins--; //haut gauche 
		board[i-1][j].nbVoisins--;  //haut
		board[i-1][j+1].nbVoisins--; //haut droite	
		board[i+1][j-1].nbVoisins--; //bas gauche
		board[i+1][j].nbVoisins--; //bas
		board[i+1][j+1].nbVoisins--;
	}
}


int nbVoisins(int i, int j)
{
	if(1>i || i>N) printf("caca\n");	
	
	int res = 0;
	/*
	if (board[i][j-1].vivant) res++; //gauche
	if (board[i][j+1].vivant) res++; //droite
	if (board[i-1][j-1].vivant) res++; //haut gauche 
	if (board[i-1][j].vivant) res++;  //haut
	if (board[i-1][j+1].vivant) res++; //haut droite	
	if (board[i+1][j-1].vivant) res++; //bas gauche
	if (board[i+1][j].vivant) res++; //bas
	if (board[i+1][j+1].vivant) res++; //bas droite	*/
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
		
		//~ board[1][1]=true;	
		//~ board[1][2]=true;	
		//~ board[1][3]=true;	
		//~ board[3][1]=true;	
		//~ board[4][1]=true;	
		//~ board[4][4]=true;	
		//~ board[1][4]=true;
	for (i=1;i<N;i++)
		for (j=1;j<N-1;j++)
		{
			board[i][j].nbVoisins = nbVoisins(i,j);
		}
		
	tor();
}

void * thread(void *a)
{
	int i,j;
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
				/* OLD
				nbv = nbVoisins(i,j);
				if (nbv == 3 && !board[i][j].vivant)
					board[i][j].vivant = true;
				else if ((nbv<=1 || nbv>=4) && board[i][j].vivant)
					board[i][j].vivant = false;
				else
				{} //maff == mcal (old)
				 * */
				 
				 // mort
				 if( board[i][j].vivant && (board[i][j].nbVoisins <= 1 || board[i][j].nbVoisins >= 4) ){
					 board[i][j].vivant = false;
					 majVoisins(i,j,false);
				 }
				 
				 // naissance
				 else if ( board[i][j].nbVoisins == 3 && !board[i][j].vivant){
					 board[i][j].vivant = true;
					 majVoisins(i,j,true);
				 }
				 else {
					 pthread_mutex_lock(&m1);
					 cases_nonchangees++;
					 pthread_mutex_unlock(&m1);
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
		
		if (cases_nonchangees >= (N*N))
			fin=1;
		else cases_nonchangees = 0;
		
		while(SDL_PollEvent(&event) ){
			if( event.type == SDL_QUIT ) 
					fin = false;//On quitte le programme
		}
		
		pthread_mutex_unlock(&m2);
		sleep(1);
	}
	SDL_FreeSurface(cellule);
    SDL_Quit();
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char ** argv)
{
	int i=0;
	int j;
	N = atoi(argv[1])+2;
	
	struct timeval tpDeb;
	struct timeval tpFin;
	//Creation des threads
	pthread_t th[NB_THREADS];

	/* Creation des mutex et de la variable condition */
	pthread_mutex_init(&m1,NULL);
	pthread_mutex_init(&m2,NULL);
	pthread_cond_init(&cond,NULL);
	
	/* Creation de la matrice */
	board = (struct element **) malloc(N*sizeof(struct element *));
	for (i=0;i<N;i++){
		board[i] = (struct element *) malloc(N*sizeof(struct element));
	}
	
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
	
	// desallocation matrice
	for (i=0;i<N;i++){
		free(board[i]);
	}
	free(board);
	
	return 0; 
}

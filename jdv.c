/**
 *   - Utiliser sémaphore
 *   - Noms plus explicites
 **/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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

struct element {
    bool vivant;
    int nbVoisins;
};

static struct element **board = NULL;
static int N; //soit de N-2*N-2 affichee a cause du tore


#if defined(AFF)
static SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran
static SDL_Surface *cellule; //Tableau de surface pour afficher les cellules
static SDL_Rect position;
static SDL_Event event; /* La variable contenant l'événement */
static const int TAILLE = 25;
#endif


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

#if defined(AFF)
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
#endif


/* TO DO
void initFromFile( char *filePath )
{
	int i,j;
	// au cas ou ...
	i = -1;
	j = -1;
	
	FILE * boardFile = NULL;
	boardFile = fopen(filePath,,"r");
	
	if ( boardFile != NULL ){
		do
		{
			
		} while ()
		
		
		
		
		fclose(boardFile)
	}
	else
		erreur("Impossible d'ouvrir le fichier");
}
*/	

//Mise en en place du Tor
void tor(void)
{
    int i;

    //Coin bas droite
    board[N-1][N-1] = board[1][1];

    //Coin haut gauche
    board[0][0] = board[N-2][N-2];

    //Coin haut droite
    board[0][N-1] = board[N-2][1];

    //Coin bas gauche
    board[N-1][0] = board[1][N-2];

    //copie de la première ligne (1) sur la dernière (N-1)
    for(i=1; i<N-1; i++) {

        //copie de la première ligne (1) sur la dernière (N-1)
        board[N-1][i] = board[1][i];

        //copie de la dernière ligne (N-1) sur la première (0)
        board[0][i] = board[N-2][i];

        //copie de la première colonne (1) sur la dernière (N-1)
        board[i][N-1] = board[i][1];

        //copie de la dernière colonne (N-1) sur la première (0)
        board[i][0] = board[i][N-2];
    }
}

void majVoisins(int i, int j, bool vivant) {
    if(vivant) {
        board[i][j-1].nbVoisins++; //gauche
        board[i][j+1].nbVoisins++; //droite
        board[i-1][j-1].nbVoisins++; //haut gauche
        board[i-1][j].nbVoisins++;  //haut
        board[i-1][j+1].nbVoisins++; //haut droite
        board[i+1][j-1].nbVoisins++; //bas gauche
        board[i+1][j].nbVoisins++; //bas
        board[i+1][j+1].nbVoisins++;
    }
    else {
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
    for (i=1; i<N-1; i++)
        for (j=1; j<N-1; j++)
        {
            if (random()%2)
                board[i][j].vivant = true;
            else
                board[i][j].vivant = false;
        }

    for (i=1; i<N-1; i++)
        for (j=1; j<N-1; j++)
        {
            board[i][j].nbVoisins = nbVoisins(i,j);
        }

tor();

}

void * thread(void *a)
{
    int i,j;
    int numThread;
    numThread = *((int *) a);
    bool fin = false;
        pthread_mutex_lock(&m2);

for (i = 0; i < 10; i++)
{
	
        for(i=(numThread*NB_LIGNES_PAR_THREADS)+1; i<(numThread*NB_LIGNES_PAR_THREADS)+NB_LIGNES_PAR_THREADS+1; i++)
        {
            for(j=1; j<N-1; j++)
            {
                // mort
                if( board[i][j].vivant && (board[i][j].nbVoisins < 2 || board[i][j].nbVoisins > 3) ) {
                    board[i][j].vivant = false;
                    majVoisins(i,j,false);
                }

                // naissance
                else if ( board[i][j].nbVoisins == 3 && !board[i][j].vivant) {
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

        nb_th_finis++;

        if (nb_th_finis == NB_THREADS)
        {
			//~ printf("avant tore\n");
//~ #if defined(AFF)
            //~ affJeuSDL();
//~ #endif
//~ 
			//~ sleep(5);
			
            tor();
			//~ printf("après tore\n");
#if defined(AFF)
            affJeuSDL();
#endif

            nb_th_finis = 0;
            pthread_cond_broadcast(&cond);
        }
        else {
            pthread_cond_wait(&cond,&m2);
        }

        if (cases_nonchangees >= (N*N))
            fin=true;
        else cases_nonchangees = 0;

#if defined(AFF)
        while(SDL_PollEvent(&event) ) {
            if( event.type == SDL_QUIT )
                fin = true;//On quitte le programme
        }
        sleep(1);
#endif

    
}
        pthread_mutex_unlock(&m2);

 //   pthread_exit(NULL);
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
    for (i=0; i<N; i++) {
        board[i] = (struct element *) malloc(N*sizeof(struct element));
    }

    initJeu();

#if defined(AFF)
    initSDL();
    affJeuSDL();
#endif

    gettimeofday(&tpDeb,NULL);
    printf("Tps dbt: %d\n" ,tpDeb);

    /* Creation des threads */
    for (i=0; i<NB_THREADS; i++) {
        pthread_create(&(th[i]),NULL,thread,(void *) &i);
    }

    /* Attente de la fin de tous les threads */
    for (i=0; i<NB_THREADS; i++) {
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
    for (i=0; i<N; i++) {
        free(board[i]);
    }
    free(board);

#if defined(AFF)
    SDL_FreeSurface(cellule);
    SDL_Quit();
#endif

    return 0;
}

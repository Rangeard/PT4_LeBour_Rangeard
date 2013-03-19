#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "../h/jdv_util.h"

#if defined(AFF)
#include "../h/jdv_sdl.h"
#endif
#if defined(OGL)
#include "../h/jdv_ogl.h"
#endif

/**
 * Retourne le nombre de voisins(vivants) d'une case
 * selon ses coordonnées.
 * 
 * Les booléens en C étant implémentés via un entier bolqué
 * à 1 ou 0 on peut les additionner
 * */
int nbVoisins(int i, int j)
{    
    return board[i][j+1].vivant + board[i][j-1].vivant +
	  board[i+1][j].vivant + board[i-1][j].vivant +
	  board[i+1][j+1].vivant + board[i-1][j-1].vivant +
	  board[i+1][j-1].vivant + board[i-1][j+1].vivant;
}

/**
 * Initialise le plateau de façon aléatoire
 * */
void initJeu(void)
{
    int i,j;
    srandom(1);
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
}

/**
 * Routine exécutée par les threads
 * */
void * thread(void *a)
{
    int iteration,i,j;
    int voisinPrecedent, voisinSuivant;
     
    /* Numero du thread courant */
    int numThread = (intptr_t)a;
    
    /* Indice (ligne) de debut de zone */
    int dbtZone = (numThread*NB_LIGNES_PAR_THREADS)+1;
    
    /* Indice (ligne) de fin de zone */
    int finZone = dbtZone+NB_LIGNES_PAR_THREADS-1;
    
    /* Determination du numero des voisins */
    if(numThread == 0)
		voisinPrecedent = NB_THREADS-1;
	else
		voisinPrecedent = numThread-1;
		
	if(numThread == NB_THREADS-1)
		voisinSuivant = 0;
	else
		voisinSuivant = numThread+1;

	for (iteration = 0; iteration < NB_ITERATIONS; iteration++)
	{
		/* Si le thread courant est celui qui s'occupe de la derniere bande de la matrice 
		 * On met en place le tore sur les lignes horizontales et les coins pour son voisinSuivant soit le thread 0 */
        if (numThread == NB_THREADS-1) 
        {
			//coin supérieur gauche
			board[0][0].vivant = board[N-2][N-2].vivant;
			board[0][0].nbVoisins = board[N-2][N-2].nbVoisins;
			
			//coin supérieur droit			
			board[0][N-1].vivant = board[N-2][1].vivant;				
			board[0][N-1].nbVoisins = board[N-2][1].nbVoisins;
			
			//premiere ligne de la matrice			
			for (i = 1; i < N-1; i++)
			{
				board[0][i].vivant = board[N-2][i].vivant;					
				board[0][i].nbVoisins = board[N-2][i].nbVoisins;			
			}
		} 
		
		/* Si le thread courant est celui qui s'occupe de la premiere bande de la matrice
		 * On met en place le tore sur les lignes horizontales et les coins pour son voisinPrecedent soit le thread NB_THREADS-1 */
		if (numThread == 0) 
		{   
			//coin inférieur gauche
			board[N-1][0].vivant = board[1][N-2].vivant;     
			board[N-1][0].nbVoisins = board[1][N-2].nbVoisins;
			
			//coin inférieur droit   
			board[N-1][N-1].vivant = board[1][1].vivant;   
			board[N-1][N-1].nbVoisins = board[1][1].nbVoisins;
			
			//derniere ligne de la matrice
			for (i = 1; i < N-1; i++) 
			{
				board[N-1][i].vivant = board[1][i].vivant;   
				board[N-1][i].nbVoisins = board[1][i].nbVoisins;   
			}
		}
		
		/* On met en place le tore sur les bords pour le thread courant */
		board[dbtZone][0].vivant = board[dbtZone][N-2].vivant;
		board[dbtZone][0].nbVoisins = board[dbtZone][N-2].nbVoisins;

		board[dbtZone][N-1].vivant = board[dbtZone][1].vivant;
		board[dbtZone][N-1].nbVoisins = board[dbtZone][1].nbVoisins;

		board[finZone][0].vivant = board[finZone][N-2].vivant;     
		board[finZone][0].nbVoisins = board[finZone][N-2].nbVoisins;

		board[finZone][N-1].vivant = board[finZone][1].vivant;
		board[finZone][N-1].nbVoisins = board[finZone][1].nbVoisins;
		
		/* On préviens les voisins qu'on a mis en place les bordures */
		sem_post(&sem1[voisinSuivant]);
		sem_post(&sem1[voisinPrecedent]);
		
		/* On met en place le tore pour la zone interieure du thread courant */
		for (i = dbtZone+1; i < finZone; i++) 
		{
		  board[i][0].vivant = board[i][N-2].vivant;     
		  board[i][0].nbVoisins = board[i][N-2].nbVoisins;     
		  board[i][N-1].vivant = board[i][1].vivant;
		  board[i][N-1].nbVoisins = board[i][1].nbVoisins;
		}
		
		/* On attend que les voisins aient fini de traiter leurs bordures */
		sem_wait(&sem1[numThread]);
		sem_wait(&sem1[numThread]);

		/* Calcul du nombre de voisins sur les bordures */
		for(j=1; j<N-1; j++)
		{
			board[dbtZone][j].nbVoisins = nbVoisins(dbtZone,j); //Bordure superieure
			board[finZone][j].nbVoisins = nbVoisins(finZone,j); //Bordure inferieure
		}

		/* On préviens les voisins qu'on a fini de traiter les bordures, maj des voisins comprises */
		sem_post(&sem2[voisinSuivant]);
		sem_post(&sem2[voisinPrecedent]);

		/* Calcul du nombre de voisins vivants à l'intérieur */
		for (i = dbtZone+1; i < finZone; i++) 
			for (j = 1; j < N-1; j++)
				board[i][j].nbVoisins = nbVoisins(i,j);

		/* On attend que les voisins aient fini de traiter leurs bordures */
		sem_wait(&sem2[numThread]);
		sem_wait(&sem2[numThread]);
		
		/* Mise a jour des cellules dans la zone du thread */
		for(i = dbtZone; i <= finZone; i++)
		{
			for(j = 1; j < N-1; j++)
			{
				//mort d'une cellule
				if(board[i][j].nbVoisins < 2 || board[i][j].nbVoisins > 3)
					board[i][j].vivant = false;
				//naissance
				else if(board[i][j].nbVoisins == 3 && !board[i][j].vivant)
					board[i][j].vivant = true;	
			}
		}
		
		//~ //Le thread 0 se charge de l'affichage (SDL)
		#if defined(AFF)
		if(numThread==1)
		{
			affJeuSDL(board);
			sleep(1);
		}
		
		#endif
		
		//Le thread 0 se charge de l'affichage (3D)
		#if defined(OGL)
		if(numThread==0)
		{
			affJeuOGL(board);
			sleep(1);
		}
		#endif
	}

    return NULL;
}

int main()
{
    int i;
	struct timeval tpDeb;
    struct timeval tpFin;
    struct timeval result;
    
	/* Creation de la matrice */
	board = (struct cellule **) malloc(N*sizeof(struct cellule *));
    for (i=0; i<N; i++) 
    {
        board[i] = (struct cellule *) malloc(N*sizeof(struct cellule));
    }

    //Creation des threads
    pthread_t th[NB_THREADS];
    
	//Creation des semaphores
	sem1 = malloc(NB_THREADS * sizeof(sem_t));
	sem2 = malloc(NB_THREADS * sizeof(sem_t));
	
	//initialisation de la board
	initJeu();
	
#if defined(AFF)
    initSDL();
#endif
#if defined(OGL)
    initOGL();
#endif

	
	/* Initialisation des sémaphores */
	for(i = 0; i < NB_THREADS; i++)
	{
		sem_init(&sem1[i], 0, 0);
		sem_init(&sem2[i], 0, 0); 
	}
	
	// récupération du temps du démarage
	gettimeofday(&tpDeb,NULL);
		
    /* Creation des threads */
    for (i=0; i<NB_THREADS; i++) {
        pthread_create(&(th[i]),NULL,thread,(void*)(intptr_t)i);
    }

    /* Attente de la fin de tous les threads */
    for (i=0; i<NB_THREADS; i++) {
        pthread_join(th[i],NULL);
    }
    
    // récupération du temps de la fin d'exécution
    gettimeofday(&tpFin,NULL);
    
	timeval_subtract(&result,&tpFin,&tpDeb);
	printf("tpDeb : %ld,%ld\ntpFin : %ld,%ld\ntotal : %ld,%ld\n",tpDeb.tv_sec,tpDeb.tv_usec,tpFin.tv_sec,tpFin.tv_usec,result.tv_sec,result.tv_usec);
    
//~ #if defined(AFF)
    //~ affJeuSDL(board);
    //~ sleep(1);
//~ #endif
    
    /* Destruction des semaphores */
    free(sem1);
	free(sem2);

#if defined(AFF)
	/* Liberation de l'espace memoire utilise par la SDL et fermeture de l'affichage */
    SDL_FreeSurface(cellules);
    SDL_Quit();
#endif
#if defined(OGL)
    SDL_Quit();
#endif

    // desallocation matrice
    for (i=0; i<N; i++)
        free(board[i]);
    free(board);

    return EXIT_SUCCESS;
}

/**
 *   - Utiliser sémaphore
 *   - Noms plus explicites
 **/

#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "../h/jdv_util.h"

#if defined(AFF)
#include "../h/jdv_sdl.h"
#endif

//Mise en en place du Tor
void tore(void)
{
    int i;
       
    //Coin bas droite
    board[N-1][N-1].vivant = board[1][1].vivant;
    board[N-1][N-1].nbVoisins = board[1][1].nbVoisins;
 
    //Coin haut gauche
    board[0][0].vivant = board[N-2][N-2].vivant;
    board[0][0].nbVoisins = board[N-2][N-2].nbVoisins;

    //Coin haut droite
    board[0][N-1].vivant = board[N-2][1].vivant;
    board[0][N-1].nbVoisins = board[N-2][1].nbVoisins;

    //Coin bas gauche
    board[N-1][0].vivant = board[1][N-2].vivant;
    board[N-1][0].nbVoisins = board[1][N-2].nbVoisins;
    
    //copie de la première ligne (1) sur la dernière (N-1)
    for(i=1; i<N-1; i++) 
    {
        //copie de la première ligne (1) sur la dernière (N-1)
        board[N-1][i].vivant = board[1][i].vivant;
		board[N-1][i].nbVoisins = board[1][i].nbVoisins;
        //copie de la dernière ligne (N-1) sur la première (0)
        board[0][i].vivant = board[N-2][i].vivant;
		board[0][i].nbVoisins = board[N-2][i].nbVoisins;
        //copie de la première colonne (1) sur la dernière (N-1)
        board[i][N-1].vivant = board[i][1].vivant;
		board[i][N-1].nbVoisins = board[i][1].nbVoisins;
        //copie de la dernière colonne (N-1) sur la première (0)
        board[i][0].vivant = board[i][N-2].vivant;
        board[i][0].nbVoisins = board[i][N-2].nbVoisins;
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
    srandom(1);
    for (i=1; i<N-1; i++)
        for (j=1; j<N-1; j++)
        {
            if (random()%2)
                board[i][j].vivant = true;
            else
                board[i][j].vivant = false;
        }
     
    tore();
   
    for (i=1; i<N-1; i++)
        for (j=1; j<N-1; j++)
        {
            board[i][j].nbVoisins = nbVoisins(i,j);
        }
 
}

void * thread(void *a)
{
    int it,i,j;
    int numThread;
    numThread = *((int *) a);
    
    
    struct util *tab_util;
	int cpt_util = 0;
	
	tab_util = (struct util *) malloc(N*N*sizeof(struct util));
    
    pthread_mutex_lock(&m2);

for (it = 0; it < NB_ITERATIONS; it++)
{
        for(i=(numThread*NB_LIGNES_PAR_THREADS)+1; i<(numThread*NB_LIGNES_PAR_THREADS)+NB_LIGNES_PAR_THREADS+1; i++)
        {
            for(j=1; j<N-1; j++)
            {
                // mort
                if( board[i][j].vivant && (board[i][j].nbVoisins < 2 || board[i][j].nbVoisins > 3) ) {
                    tab_util[cpt_util].i = i;
                    tab_util[cpt_util].j = j;
                    tab_util[cpt_util].state = false;
                    cpt_util++;
                }

                // naissance
                else if ( board[i][j].nbVoisins == 3 && !board[i][j].vivant) {
                    tab_util[cpt_util].i = i;
                    tab_util[cpt_util].j = j;
                    tab_util[cpt_util].state = true;
                    cpt_util++;
                }
            }
        }
		
		//on effectue les modif
		for (i = 0; i < cpt_util; i++)
		{
				board[tab_util[i].i][tab_util[i].j].vivant = tab_util[i].state;
                majVoisins(tab_util[i].i,tab_util[i].j,tab_util[i].state);
		}

        nb_th_finis++;

        if (nb_th_finis == NB_THREADS)
        {
			
            tore();
#if defined(AFF)
            affJeuSDL();
#endif

            nb_th_finis = 0;
            pthread_cond_broadcast(&cond);
        }
        else {
            pthread_cond_wait(&cond,&m2);
        }   
}
        pthread_mutex_unlock(&m2);
	free(tab_util);
    return NULL;
}

 int main(int argc, char ** argv)
{
    int i=0;
    int j;
	struct timeval tpDeb;
    struct timeval tpFin;
    struct timeval result;

	/* Creation de la matrice */
	board = (struct element **) malloc(N*sizeof(struct element *));
    for (i=0; i<N; i++) 
    {
        board[i] = (struct element *) malloc(N*sizeof(struct element));
    }

    //Creation des threads
    pthread_t th[NB_THREADS];

    /* Creation des mutex et de la variable condition */
    pthread_mutex_init(&m1,NULL);
    pthread_mutex_init(&m2,NULL);
    pthread_cond_init(&cond,NULL);

	//initJeuTest();
	initJeu();
	
#if defined(AFF)
    initSDL();
    affJeuSDL();
    sleep(1);
#endif

	gettimeofday(&tpDeb,NULL);
	
    /* Creation des threads */
    for (i=0; i<NB_THREADS; i++) {
        pthread_create(&(th[i]),NULL,thread,(void *) &i);
    }

    /* Attente de la fin de tous les threads */
    for (i=0; i<NB_THREADS; i++) {
        pthread_join(th[i],NULL);
    }
    
    gettimeofday(&tpFin,NULL);
	timeval_subtract(&result,&tpFin,&tpDeb);
	printf("tpDeb : %ld,%ld\ntpFin : %ld,%ld\ntotal : %ld,%ld\n",tpDeb.tv_sec,tpDeb.tv_usec,tpFin.tv_sec,tpFin.tv_usec,result.tv_sec,result.tv_usec);
    
    #if defined(AFF)
    affJeuSDL();sleep(10);
#endif
    
    //~ verification();

    /* Destruction des mutex et de la variable condition */
    pthread_mutex_destroy(&m1);
    pthread_mutex_destroy(&m2);

    pthread_cond_destroy(&cond);

#if defined(AFF)
    SDL_FreeSurface(cellule);
    SDL_Quit();
#endif

    // desallocation matrice
    for (i=0; i<N; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}

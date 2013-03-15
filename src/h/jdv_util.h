#ifndef H_JDV_UTIL
#define H_JDV_UTIL

#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>


/* Nombre de threads (attention ce nombre doit etre un multiple de N*N) */
#define NB_THREADS 4

/* Nombre de cellules traitees par un thread */
#define NB_LIGNES_PAR_THREADS (N-2)/NB_THREADS

/* Taille board ( taille effective N-2 ) */
#define N 402

static const int NB_ITERATIONS = 1000;

struct element {
    bool vivant;
    int nbVoisins;
};

struct util {
	int i,j;
	bool state;
};

//~ static struct element board[N][N];
static struct element **board;
static struct element verif[8][8];

/* Nombre de cases (cellules) n'ayant pas changees entre 2 instants t
   cette variable permet d'arreter le jeu lorsque celui-ci ne change plus */
static int cases_nonchangees = 0;

/* Nombre de threads lances */
static int nb_th_lance = 0;

/* Nombre de threads ayant termines leur traitement */
static int nb_th_finis = 0;

static pthread_mutex_t m1;
/* Mutex 2 */
static pthread_mutex_t m2;
/* Variable condition */
static pthread_cond_t cond;

void erreur(char *s);
void verification();
void initBoardFromFile( char *filePath );
void initVerifFromFile(char *filePath );
void initJeuTest(void);
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);

#endif

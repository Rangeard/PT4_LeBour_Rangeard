#ifndef H_JDV_UTIL
#define H_JDV_UTIL

#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <semaphore.h>

/* Nombre de threads (attention ce nombre doit etre un multiple de N*N) */
#define NB_THREADS 4

/* Nombre de cellules traitees par un thread */
#define NB_LIGNES_PAR_THREADS (N-2)/NB_THREADS

/* Taille board ( taille effective N-2 ) */
#define N 82

/* Nombre de generations a calculer */
static int NB_ITERATIONS = 1000;

/* Case du plateau */
struct cellule {
    bool vivant;
    int nbVoisins;
};

/* Plateau */
static struct cellule **board;


/* Nombre de threads lances */
static int nb_th_lance = 0;

/* Nombre de threads ayant termines leur traitement */
static int nb_th_finis = 0;

/* Semaphores */
sem_t *sem1, *sem2;

void erreur(char *s);
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);

#endif

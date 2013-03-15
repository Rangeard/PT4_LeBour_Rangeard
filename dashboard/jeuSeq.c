#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
<<<<<<< HEAD
#include <stdbool.h>
#include <sys/time.h>

#define N 10 //soit de 8x8

struct element {
    bool vivant;
    int nbVoisins;
};

struct util {
	int i,j;
	bool state;
};

static struct element board[N][N];
static struct element verif[8][8];

static const int NB_ITERATIONS = 1;

/* Subtract the `struct timeval' values X and Y,
        storing the result in RESULT.
        Return 1 if the difference is negative, otherwise 0. */
     
     int
     timeval_subtract (result, x, y)
          struct timeval *result, *x, *y;
     {
       /* Perform the carry for the later subtraction by updating y. */
       if (x->tv_usec < y->tv_usec) {
         int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
         y->tv_usec -= 1000000 * nsec;
         y->tv_sec += nsec;
       }
       if (x->tv_usec - y->tv_usec > 1000000) {
         int nsec = (x->tv_usec - y->tv_usec) / 1000000;
         y->tv_usec += 1000000 * nsec;
         y->tv_sec -= nsec;
       }
     
       /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
       result->tv_sec = x->tv_sec - y->tv_sec;
       result->tv_usec = x->tv_usec - y->tv_usec;
     
       /* Return 1 if result is negative. */
       return x->tv_sec < y->tv_sec;
     }
=======
//prout
#define N 10 //soit de 8x8
#define true (1==1)
#define false !true
static char mAff[N][N];
static char mCal[N][N];
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58

void erreur(char *s)
{
  perror(s);
	exit(-1);
}

<<<<<<< HEAD
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

void affJeu(int test)
{
	int i,j;
	printf("\033[2J");
	printf("JEU DE LA VIE :%d\n\n",test);
=======
void affJeu(void)
{
	int i,j;
	printf("\033[H");
	printf("JEU DE LA VIE :\n\n");
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
<<<<<<< HEAD
			if((i==0 || i==N-1 || j==0 || j==N-1) && board[i][j].vivant)
				printf("+ ");
			else if((i==0 || i==N-1 || j==0 || j==N-1) && !board[i][j].vivant)
				printf("- ");
			else if(board[i][j].vivant)
				printf("0 ");
			else
				printf("X ");
		}	
		printf("\n");
	}
	/*
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			printf("CELL %d - %d\n    vivant = %d\n    nbvoisins = %d\n",i,j,board[i][j].vivant,board[i][j].nbVoisins);
		}
	}
	*/
=======
			if((i==0 || i==N-1 || j==0 || j==N-1) && mCal[i][j])
				printf("()");
			else if((i==0 || i==N-1 || j==0 || j==N-1) && !mCal[i][j])
				printf("--");
			else if(mCal[i][j])
				printf("{}");
			else
				printf("  ");
		}	
		printf("\n");
	}
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
}

//Mise en en place du Tor
void tor(void)
{
	int i;
<<<<<<< HEAD
	
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

//~ void echange(void)
//~ {
	//~ int i,j;
	//~ for(i=0; i<N; i++)
		//~ for(j=0;j<N;j++)
			//~ mCal[i][j] = mAff[i][j];
//~ }

void initJeuTest(void)
{
	int i,j;
	for (i = 1; i < N-1; i++)
	{
		for (j = 1; j < N-1; j++)
		{
			board[i][j].vivant = false;
			
		}
		
	}

	board[1][1].vivant = (1==1);
	board[1][2].vivant = true;
	board[1][8].vivant = true;
	board[2][1].vivant = true;
	board[2][6].vivant = true;
	board[3][4].vivant = true;
	board[3][6].vivant = true;
	board[3][7].vivant = true;
	board[4][4].vivant = true;
	board[5][4].vivant = true;
	board[6][3].vivant = true;
	board[6][6].vivant = true;
	board[6][8].vivant = true;
	board[7][3].vivant = true;
	board[7][4].vivant = true;
	board[7][7].vivant = true;
	board[7][8].vivant = true;
	board[8][6].vivant = true;
	
	for (i = 0; i < 8; i++)
	{
		for (j = 1; j < 8; j++)
		{
			verif[i][j].vivant = false;
			
		}
		
	}
	
	verif[1][1].vivant = true;
	verif[1][2].vivant = true;
	verif[1][7].vivant = true;
	verif[1][8].vivant = true;
	verif[2][1].vivant = true;
	verif[2][2].vivant = true;
	verif[2][5].vivant = true;
	verif[2][6].vivant = true;
	verif[3][6].vivant = true;
	verif[3][7].vivant = true;
	verif[4][3].vivant = true;
	verif[4][4].vivant = true;
	verif[5][3].vivant = true;
	verif[5][4].vivant = true;
	verif[5][5].vivant = true;
	verif[6][3].vivant = true;
	verif[6][5].vivant = true;
	verif[6][8].vivant = true;
	verif[7][3].vivant = true;
	verif[7][4].vivant = true;
	verif[7][5].vivant = true;
	verif[7][6].vivant = true;
	verif[7][8].vivant = true;
	verif[8][2].vivant = true;
	verif[8][3].vivant = true;	
	
	tor();
	for (i=1; i<N-1; i++)
        for (j=1; j<N-1; j++)
        {
            board[i][j].nbVoisins = nbVoisins(i,j);
        }
 
       
} 

void verification(){
	printf("============================\n=  VERIFICATION\n============================\n");
	int i,j,cpt;
	cpt=0;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			 if(board[i+1][j+1].vivant != verif[i][j].vivant){
				 cpt++;
				 printf("ERR %d - %d\n  board = %d\n  verif = %d\n",i,j,board[i+1][j+1].vivant,verif[i][j].vivant);
			 }
		}
		
	}
	printf("TOTAL : %d\n",cpt);
	
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

void boucle(void)
{
	/**
	 * Tableau utilitaire
	 * taille N*N == Nb cases board
	 * Permet de sauvegarder changements à effectuer, 
	 * éviter l'effet domino lorsque l'on modifie l'état
	 * d'une cellule.
	 * 
	 * /!\ THREADS
	 * chaque threads devra avoir SON tab_util sinon gros problèmes de concurrence
	 * */
	struct util tab_util[N*N];
	int cpt_util = 0;
	
	
	int it,i,j,nbv;
	for (it = 0; it < NB_ITERATIONS; it++)
	{
=======
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
	int i,j,nbv;
	int changement = 0;
	while(changement >= 0)
	{
		changement = 0;
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
		for(i=1; i<N-1; i++)
		{
			for(j=1;j<N-1;j++)
			{
<<<<<<< HEAD
				// mort
                if( board[i][j].vivant && (board[i][j].nbVoisins < 2 || board[i][j].nbVoisins > 3) ) {
                    //~ board[i][j].vivant = false;
                    //~ majVoisins(i,j,false);
                    tab_util[cpt_util].i = i;
                    tab_util[cpt_util].j = j;
                    tab_util[cpt_util].state = false;
                    cpt_util++;
                }

                // naissance
                else if ( board[i][j].nbVoisins == 3 && !board[i][j].vivant) {
                    //~ board[i][j].vivant = true;
                    //~ majVoisins(i,j,true);
                    
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
		// copies pour le tore	
		tor();
		
		//affichage	
#if defined(AFF)
		affJeu(2);
		sleep(1);

#endif
=======
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
		affJeu();
		//Condition d'arrêt
		if(changement == (N-2)*(N-2))
			changement = -1;
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
		
	}

}

int main()
{
<<<<<<< HEAD
	
	struct timeval tpDeb;
    struct timeval tpFin;
    struct timeval result;
    long int Rsec,Rusec;
	initJeuTest();
	affJeu(1);
//~ #if defined(AFF)
	//~ pid_t pid;
	//~ if ((pid = fork()) < 0)
		//~ erreur("Erreur lors de la creation du processus fils");
//~ 
	//~ if (pid == 0) 		
		//~ execlp("clear","clear",NULL);
//~ 
	//~ wait(NULL);
	//~ //affJeu();
//~ #endif
	gettimeofday(&tpDeb,NULL);
	boucle();
	gettimeofday(&tpFin,NULL);
	timeval_subtract(&result,&tpFin,&tpDeb);
	printf("tpDeb : %ld,%ld\ntpFin : %ld,%ld\ntotal : %ld,%ld\n",tpDeb.tv_sec,tpDeb.tv_usec,tpFin.tv_sec,tpFin.tv_usec,result.tv_sec,result.tv_usec);
	//~ printf("%d,%d\n",Rsec,Rusec);
	//verification();
//	sleep(1);
	affJeu(3);
=======
	pid_t pid;
	if ((pid = fork()) < 0)
		erreur("Erreur lors de la creation du processus fils");

	if (pid == 0) 		
		execlp("clear","clear",NULL);

	wait(NULL);

	
	initJeu();
	affJeu();
	boucle();
>>>>>>> 6ef8b0ae6813e862adbf98f1226167649cbf3a58
	return 0; 
}

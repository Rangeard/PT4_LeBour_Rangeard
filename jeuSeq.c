#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 10 //soit de 8x8
#define true (1==1)
#define false !true
static char mAff[N][N];
static char mCal[N][N];

void erreur(char *s)
{
  perror(s);
	exit(-1);
}

void affJeu(void)
{
	int i,j;
	printf("\033[H");
	printf("JEU DE LA VIE :\n\n");
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
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
	int i,j,nbv;
	int changement = 0;
	while(changement >= 0)
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
		//Echange des matrices
		echange();
		sleep(1);
		affJeu();
		//Condition d'arrêt
		if(changement == (N-2)*(N-2))
			changement = -1;
		
	}

}

int main()
{
	pid_t pid;
	if ((pid = fork()) < 0)
		erreur("Erreur lors de la creation du processus fils");

	if (pid == 0) 		
		execlp("clear","clear",NULL);

	wait(NULL);

	
	initJeu();
	affJeu();
	boucle();
	return 0; 
}

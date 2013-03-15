#include "../h/jdv_util.h"

void erreur(char *s)
{
    perror(s);
    exit(-1);
}

void verification(){
	printf("============================\n=  VERIFICATION\n============================");
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
	printf("TOTAL : %d",cpt);
	
}

// TO DO
void initBoardFromFile( char *filePath )
{
	int i,j;
	char c;
	
	i = 0;
	j = -1;
	FILE * boardFile = NULL;
	boardFile = fopen(filePath,"r");
	
	if ( boardFile != NULL )
	{
		do
		{
			j++;
			c = fgetc(boardFile);
			
			if(c=='\n')
			{
				j=-1;
				i++;
			}
			else
			{
				if(i<N)
				{
					if(c == '1')
						board[i][j].vivant = true;
					else
						board[i][j].vivant = false;
					board[i][j].nbVoisins = -1;
				}
				else
					c = 'p';
				
			}
				
		} while (c!='p');
		fclose(boardFile);
	}
	else
		erreur("Impossible d'ouvrir le fichier");
}


void initVerifFromFile(char *filePath )
{
	int i,j,taille;
	char c;
	
	i = 0;
	j = -1;
	taille = 8;
	FILE * boardFile = NULL;
	boardFile = fopen(filePath,"r");
	
	if ( boardFile != NULL )
	{
		do
		{
			j++;
			c = fgetc(boardFile);
			
			if(c=='\n'){
				j=-1;
				i++;
			}
			else
			{
				if(i<=taille && j<=taille)
				{
					
					if(c == '1')
						verif[i][j].vivant = true;
					else
						verif[i][j].vivant = false;
					verif[i][j].nbVoisins = -1;
				}
				else
					c = 'p';
			}
			
		} while (c!='p');
		fclose(boardFile);
	}
	else
		erreur("Impossible d'ouvrir le fichier");
}

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

	board[1][1].vivant = true;
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

/* Subtract the `struct timeval' values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0. 
 * http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 * */
     
int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
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

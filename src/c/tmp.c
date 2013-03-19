
// SI les bords des voisins sont à la même génération que moi 
// 	-> je détermine naissances / morts
// 		-> quand mes bords sont terminés je le dis à mes voisins

// STOP

// SI voisinS notifient que leurs bords sont déterminés à la génération suivant
// 	-> application des modifications


long tab_gen[NB_THREADS];
tab_gen = {1,1,1,1};

void * thread(void *a)
{
    int it,i,j,prev,next;
    int numThread;
    numThread = *((int *) a);
    
    if (numThread == 1)
    {
        prev=NB_THREADS;
        next=2;
    }
    else if (numThread == NB_THREADS)
    {
        prev=NB_THREADS-1;
        next=1;
    }

    
    struct util *tab_util;
	int cpt_util = 0;
	
	tab_util = (struct util *) malloc(N*NB_LIGNES_PAR_THREADS*sizeof(struct util));
    
	for (it = 0; it < NB_ITERATIONS; it++)
	{
        do
            {
                wait();
            } while ( 
                      (numThread==1 &&  tab_gen[2]!=tab_gen[1] && tab_gen[NB_THREADS]!=tab_gen[1]) ||
                      (numThread==NB_THREADS &&  tab_gen[1]!=tab_gen[NB_THREADS] && tab_gen[NB_THREADS-1]!=tab_gen[NB_THREADS]) ||
                      (tab_gen[numThread]!=tab_gen[numThread+1] && tab_gen[NB_THREADS]!=tab_gen[numThread-1] ))
                );	

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
	}
		
	free(tab_util);
    return NULL;
}
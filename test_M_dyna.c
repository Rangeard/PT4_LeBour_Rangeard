#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct prout
{
    bool vivant;
    int nbVoisins;
};

static struct prout **board = NULL;
static int N = 10;


int main(int argc, char ** argv) {
    int i,j;
    N = atoi(argv[1]);
    board = (struct prout **) malloc(N*sizeof(struct prout *));
    for (i=0; i<N; i++) {
        board[i] = (struct prout *) malloc(N*sizeof(struct prout));
    }
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            board[i][j].vivant=true;
            board[i][j].nbVoisins=10;
        }
    }
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            printf("%d - %d : %d / %d\n",i,j,board[i][j].vivant,board[i][j].nbVoisins);
        }
    }
    for (i=0; i<N; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}

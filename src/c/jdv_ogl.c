#include "../h/jdv_ogl.h"


void initOGL(void)
{
	SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("Jeu de la Vie (3D Tore)", NULL);
    SDL_SetVideoMode(1280, 920, 32, SDL_OPENGL);
	
	glMatrixMode( GL_PROJECTION );
    //glLoadIdentity();
	
    gluPerspective(10,(double)1280/920,1,100);
    glEnable(GL_DEPTH_TEST);
}

void affJeuOGL(struct cellule **board)
{
	
	int i,j;
	float x,y,z,angleG,angleP,vectorY;
	float rayonInt = 1;
	float rayonExt = 3;
	float xCentre=0,yCentre=0,zCentre=0; //Centre du Tor
	float angleX = 0, angleY = 0, angleZ = 0;
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(2,4,7,-0.5,-0.25,-1,0,0,1);
	
	angleX+=5;
	angleZ+=1;
	glRotated(angleX,1,0,0);
	glRotated(angleY,0,1,0);
	glRotated(angleZ,0,0,1);  
	
	glPointSize(3);
    glBegin(GL_POINTS);
		x = xCentre;
		y = yCentre;
		z = zCentre;
	 	for(i=1; i<N-1; i++)
		{
			angleG = (360/((N-1)-1))*(i-1);
			x = x + (0.5*(float)sin((double)angleG));
			y = y + (0.5*(float)cos((double)angleG));
			z = 0;
			glColor3ub(255,255,255);
			glVertex3d(x,y,z);
			for(j=1; j<N-1; j++)
			{
				if(board[i][j].vivant)
					glColor3ub(0,255,0);
				else
					glColor3ub(20,20,20);
				angleP = (360/((N-1)-1))*(j-1);
				vectorY = y +(0.25*(float)cos((double)angleP));
				z = z + (0.25*(float)sin((double)angleP));
				glVertex3d(x,vectorY,z);
			}
		}
	glEnd();	
	
 	glBegin(GL_LINES);
		glColor3ub(255,255,255); glVertex3d(0,0,0); glVertex3d(1,0,0);//X
		glColor3ub(0,250,0); glVertex3d(0,0,0); glVertex3d(0,1,0); //Y
		glColor3ub(0,0,250); glVertex3d(0,0,0); glVertex3d(0,0,1); //Z
	glEnd();
	
    glFlush();
    SDL_GL_SwapBuffers(); 
}

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <math.h>
void Dessiner();
double angleZ = 0;
double angleX = 0;
double angleY = 0;
int NB = 72;
float angle;
int main(int argc, char *argv[])
{
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
 
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(15,(double)640/480,1,100);
    glEnable(GL_DEPTH_TEST);
 
    Dessiner();
 
    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
 
            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;
            }
        }
 
        angleZ += 0.5;
        angleX += 0.5;
 		angleY += 0.5;
        Dessiner();
 
    }
 
    return 0;
}
 
void Dessiner()
{
	int i,j;
	float x,y,z,angleG,angleP,vectorY;
	float rayonInt = 1;
	float rayonExt = 3;
	float xCentre=0,yCentre=0,zCentre=0; //Centre du Tor

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(2,4,7,-0.5,-0.25,-1,0,0,1);
	//glRotated(angleX,1,0,0);
	//glRotated(angleY,0,1,0);
	//glRotated(angleZ,0,0,1);
    
 	//glPointSize(1);
	
    glBegin(GL_POINTS);
	glColor3ub(255,0,0); //face rouge
	x = xCentre;
	y = yCentre;
	z = zCentre;
 	for(i=0; i<NB; i++)
	{
		angleG = (360/(NB-1))*i;
		x = x + (0.5*(float)sin((double)angleG));
		y = y + (0.5*(float)cos((double)angleG));
		z = 0;
		for(j=0; j<NB; j++)
		{
			glColor3ub(0,255,0);
			angleP = (360/(NB-1))*j;
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

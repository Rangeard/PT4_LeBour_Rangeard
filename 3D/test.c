#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <math.h>

void Dessiner();
 
double angleZ = 0;
double angleX = 0;
 
int main(int argc, char *argv[])
{
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
 
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)640/480,1,1000);
 
    glEnable(GL_DEPTH_TEST);
 
    Dessiner();
 
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
 
    for (;;)
    {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
 
            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;
            }
        }
 
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
 
        angleZ += 0.05 * ellapsed_time;
        angleX += 0.05 * ellapsed_time;
 
        Dessiner();
 
        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 10)
        {
            SDL_Delay(10 - ellapsed_time);
        }
 
    }
 
    return 0;
}
 
void Dessiner()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 	glPushMatrix();
	glTranslatef(-0.75, 0.5, 0.0);
	//glRotatef(90.0, 1.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity( );
 
    gluLookAt(3,1,0,0,0,0,0,0,10);
 
    //glRotated(angleZ,0,0,1);
    glRotated(angleX,5,0,0);
 
	float innerRadius = 0.275;
	float outerRadius = 0.85;
	int sides = 20;
	int rings = 20;
	int i, j;
	GLfloat theta, phi, theta1;
	GLfloat cosTheta, sinTheta;
	GLfloat cosTheta1, sinTheta1;
	const GLfloat ringDelta = 2.0 * 4 / rings;
	const GLfloat sideDelta = 2.0 * 4 / sides;

	theta = 0.0;
	cosTheta = 1.0;
	sinTheta = 0.0;
	for (i = rings - 1; i >= 0; i--) 
	{
		theta1 = theta + ringDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		glBegin(GL_QUAD_STRIP);
		phi = 0.0;
		for (j = sides; j >= 0; j--) 
		{
			GLfloat cosPhi, sinPhi, dist;

			phi += sideDelta;
			cosPhi = cos(phi);
			sinPhi = sin(phi);
			dist = outerRadius + innerRadius * cosPhi;

			glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
			glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, innerRadius * sinPhi);
			glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
			glVertex3f(cosTheta * dist, -sinTheta * dist, innerRadius * sinPhi);
		}
		glEnd();
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}

 
    glFlush();
    SDL_GL_SwapBuffers();
}

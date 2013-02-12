#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GL/osmesa.h"
#include "GL/glu.h"

#include <SDL/SDL.h>

static int Width = 400;
static int Height = 400;

static void
Sphere(float radius, int slices, int stacks)
{
GLUquadric *q = gluNewQuadric();
gluQuadricNormals(q, GLU_SMOOTH);
gluSphere(q, radius, slices, stacks);
gluDeleteQuadric(q);
}

static void
Cone(float base, float height, int slices, int stacks)
{
GLUquadric *q = gluNewQuadric();
gluQuadricDrawStyle(q, GLU_FILL);
gluQuadricNormals(q, GLU_SMOOTH);
gluCylinder(q, base, 0.0, height, slices, stacks);
gluDeleteQuadric(q);
}

static void
Torus(float innerRadius, float outerRadius, int sides, int rings)
{
/* from GLUT... */
int i, j;
GLfloat theta, phi, theta1;
GLfloat cosTheta, sinTheta;
GLfloat cosTheta1, sinTheta1;
const GLfloat ringDelta = 2.0 * M_PI / rings;
const GLfloat sideDelta = 2.0 * M_PI / sides;

theta = 0.0;
cosTheta = 1.0;
sinTheta = 0.0;
for (i = rings - 1; i >= 0; i--) {
theta1 = theta + ringDelta;
cosTheta1 = cos(theta1);
sinTheta1 = sin(theta1);
glBegin(GL_QUAD_STRIP);
phi = 0.0;
for (j = sides; j >= 0; j--) {
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
}

static void
render_image(void)
{
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat red_mat[] = { 1.0, 0.2, 0.2, 1.0 };
GLfloat green_mat[] = { 0.2, 1.0, 0.2, 1.0 };
GLfloat blue_mat[] = { 0.2, 0.2, 1.0, 1.0 };

glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-2.5, 2.5, -2.5, 2.5, -10.0, 10.0);
glMatrixMode(GL_MODELVIEW);

glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

glPushMatrix();
glRotatef(20.0, 1.0, 0.0, 0.0);

glPushMatrix();
glTranslatef(-0.75, 0.5, 0.0);
glRotatef(90.0, 1.0, 0.0, 0.0);
glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red_mat );
Torus(0.275, 0.85, 20, 20);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.75, -0.5, 0.0);
glRotatef(270.0, 1.0, 0.0, 0.0);
glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green_mat );
Cone(1.0, 2.0, 16, 1);
glPopMatrix();

glPushMatrix();
glTranslatef(0.75, 0.0, -1.0);
glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue_mat );
Sphere(1.0, 20, 20);
glPopMatrix();

glPopMatrix();

/* This is very important!!!
* Make sure buffered commands are finished!!!
*/
glFinish();
}

int
main(int argc, char *argv[])
{
OSMesaContext ctx;
char *filename = NULL;
SDL_Surface *screen, * buffer;
Uint32 video_flags;
SDL_Rect dest;
SDL_Rect oclip;

if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
exit(1);
}

/* Initialize the display */
screen = SDL_SetVideoMode(Width, Height, 32, 0);
if ( screen == NULL ) {
exit(1);
}
ctx = OSMesaCreateContextExt( GL_RGBA, 16, 0, 0, NULL );
if (!ctx) {
printf("OSMesaCreateContext failed!\n");
return 0;
}

// buffer for x86 systems
buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, Width, Height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);

/* Bind the buffer to the context and make it current */
if (!OSMesaMakeCurrent( ctx, buffer->pixels, GL_UNSIGNED_BYTE, Width, Height )) {
printf("OSMesaMakeCurrent failed!\n");
return 0;
}

OSMesaPixelStore(OSMESA_Y_UP,0);

{
int z, s, a;
glGetIntegerv(GL_DEPTH_BITS, &z);
glGetIntegerv(GL_STENCIL_BITS, &s);
glGetIntegerv(GL_ACCUM_RED_BITS, &a);
printf("Depth=%d Stencil=%d Accum=%d\n", z, s, a);
}

render_image();

printf("Image rendered");
dest.y = 0;
dest.x = 0;
dest.w = Width;
dest.h = Height;

/* make backup of clipping area */
SDL_GetClipRect(screen,&oclip);

/* clip to full screen */
SDL_SetClipRect(screen,NULL);
SDL_BlitSurface(buffer,NULL,screen,NULL);
SDL_SetClipRect(screen,&oclip);
SDL_Flip(screen);

getchar();

/* free the image buffer */
SDL_FreeSurface(buffer);

/* destroy the context */
OSMesaDestroyContext( ctx );

return 0;
} 

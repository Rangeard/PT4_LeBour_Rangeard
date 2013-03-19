#ifndef H_JDV_OGL
#define H_JDV_OGL

#include "../h/jdv_util.h"
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>	
#include <math.h>

void initOGL(void);
void affJeuOGL(struct cellule **board);

#endif

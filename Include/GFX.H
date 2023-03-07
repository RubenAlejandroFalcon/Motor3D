#ifndef _GFX_
#define _GFX_

#include <stdlib.h>
#include <dpmi.h>
#include <sys\nearptr.h>
#include "misc.h"

#define put_pixel(x,y,c,Screen) Screen[(y<<8)+(y<<6)+x]=c

//#define get_pixel(x,y,Screen) Screen[(y<<8)+(y<<6)+x]


//Libera la memoria de las pantallas virtuales
void free_screen();

//Inicializar
void InitGraphicMode();

//Finalizar
void ShutDownGraphicMode();

//Funci¢n que copia un el contenido de una pantalla a otra
void screen_copy(unsigned char *dest, unsigned char *src);

//Funci¢n que inicializa el valor del zbuffer;
inline void ClearZbuffer();

#endif

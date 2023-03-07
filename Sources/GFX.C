#include "gfx.h"

//Pantalla f¡sica
unsigned char *Screen;

//color
char color;

//Pantallas virtuales
unsigned char *VScreen,*VScreen2;

//Pantalla de profundidad
unsigned int *zbuffer;

//texture buffer
extern unsigned char *tbuffer;


//Origen de coordenadas dentro de la pantalla f¡sica
//Siempre se refieren al centro de la pantalla donde estamos pintando
//Por ejemplo, en una pantalla de 320*200 el origen estar  en el centro
//de dicha pantalla, es decir en las coordenas 160,100
float origenx_3d=160,origeny_3d=100;

//Lente en X e Y,indican la amplitud del campo de vision
//a mayor lente, menor campo de vision
float lenx=200,leny=200;


//Libera la memoria de las pantallas virtuales y de la pantalla de
//profundidades (zbuffer)
void free_screen()
{
        free(VScreen);
        free(VScreen2);
        free(zbuffer);
}

//Inicializar
void InitGraphicMode()
{
        //Inicializa modo gr fico 13h
        //(320 * 200 de resoluci¢n con 256 colores)
        __dpmi_regs regs;
        memset(&regs,0,sizeof regs);
        regs.x.ax=0x13;
        __dpmi_int(0x10,&regs);

        //Asigna memoria a las pantallas virtuales y las inicializa
        VScreen=(unsigned char*)malloc(64000);
        memset(VScreen,0,64000);
        VScreen2=(unsigned char*)malloc(64000);
        memset(VScreen2,0,64000);

        //Asigna memoria al zbuffer
        zbuffer=(unsigned int *)malloc(64000*sizeof(float));
        __djgpp_nearptr_enable();

        //Asigna la posici¢n de la memoria de video
        Screen=(unsigned char *)0xA0000+__djgpp_conventional_base;
}

//Finalizar
void ShutDownGraphicMode()
{
        //Retorna al modo texto
        __dpmi_regs regs;
        __djgpp_nearptr_disable();
        regs.x.ax=0x03;
        __dpmi_int(0x10,&regs);

        //Libero memoria de las pantallas virtuales
        free_screen();
}

//Funci¢n que copia un el contenido de una pantalla a otra
inline void screen_copy(unsigned char *dest, unsigned char *src)
{
        memcpy(dest,src,64000);
}

//Funci¢n que inicializa el valor del zbuffer;
inline void ClearZbuffer()
{
        memset(zbuffer,INT_MAX,256000);
}



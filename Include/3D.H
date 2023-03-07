#ifndef _3D_
#define _3D_

#include <math.h>
#include "gfx.h"

#define NUM_TEXTURAS 28

//Estructura de datos que contiene un v‚rtice
struct vertex_type
{
        float rx,ry,rz; //coordenadas relativas al punto central de un pol¡gono
        float wx,wy,wz; //coordenadas relativas al punto central del mundo 3D
        float cx,cy,cz; //coordenadas relativas al punto central de la c mara
        float sx,sy,zbuff; //coordenas en pantalla
        float mapx,mapy;
};

#define maxvertex 10
#define maxpolygon 10
#define maxalto 200

#define POLY_SOLID 1
#define POLY_TEXTUREMAPPED 2

//Estructura de datos que contiene un pol¡gono
struct polygon_type
{
        short int color;
        short int n_vertex; //N£mero de vertices del pol¡gono
        struct vertex_type vertex[maxvertex];
        unsigned char mask; //Tipo de pol¡gono
        int texture;
};

//Estructura de datos que contiene un objeto 3D
struct object_type
{
        short int n_polygon; //N£mero de pol¡gonos del objeto 3D
        struct polygon_type polygon[maxpolygon];
};

//Esta funci¢n permite conocer si un pol¡gono est  de cara o de espaldas
//a la camara, seg£n devuelva un valor mayor o menor cero.
float Hidden(struct polygon_type *pol);

//Importante funci¢n que calcula las coordenadas de un pol¡gono en una
//zona del mundo tridimensional y con un determiando  ngulo de giro
void PolygonCalcule(struct polygon_type *pol,float wx,float wy,float wz,float a, float b, float c);

//Transforma las coordenas 3D en coordenadas 2D de pantalla
void ProjectPolygon(struct polygon_type *pol);

//Esta funci¢n que textura un poligono a partir de sus coordenadas
//en 2 dimensiones
void PolygonDrawFilledTextured(struct polygon_type *poligono);

//Esta funci¢n pinta un poligono s¢lido a partir de sus
//coordenadas en 2 dimensiones
void PolygonDrawFilledSolid(struct polygon_type *poligono);

#endif


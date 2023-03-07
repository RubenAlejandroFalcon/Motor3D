#ifndef _CLIP3D_
#define _CLIP3D_

#include "3d.h"

struct tvector
{
        float x,y,z;             //vector est ndar
};

struct plane_type
{
        struct tvector  normal; // vector normal del plano
        float  distance;        // la menor distancia del plano al origen
};                              // del espacio de la c mara

struct frustrum_type
{
        struct plane_type sides[4];   // representan 4 lados del frustrum
        struct plane_type znear;      // el plano z cercano al observador
};

//Inicializa el frustrum
void setup_frustrum ();

//Producto punto
double dotproduct(struct vertex_type *vec1, struct tvector *vec2);

//Corte contra un plano del frustrum
void ClipToPlane(struct	polygon_type *polyin, struct polygon_type *polyout,
		 struct	plane_type *plane);

//Corte contra el frustrum
int ClipToFrustrum(struct polygon_type *polyin, struct polygon_type *polyout);

#endif

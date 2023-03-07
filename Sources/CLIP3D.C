#include "clip3d.h"

extern float origenx_3d;
extern float origeny_3d;
extern float lenx;
extern float leny;

struct frustrum_type frustrum;

//Inicializa el frustrum
void setup_frustrum ()
{
        float angle_horizontal =  atan2(origenx_3d,lenx)-0.0001;
        float angle_vertical   =  atan2(origeny_3d,leny)-0.0001;
        float sh	       =  sin(angle_horizontal);
        float sv	       =  sin(angle_vertical);
        float ch	       =  cos(angle_horizontal);
        float cv	       =  cos(angle_vertical);

        // plano izquierdo
        frustrum.sides[0].normal.x = ch;
        frustrum.sides[0].normal.y=0;
        frustrum.sides[0].normal.z=sh;
        frustrum.sides[0].distance = 0;

        // plano derecho
        frustrum.sides[1].normal.x = -ch;
        frustrum.sides[1].normal.y = 0;
        frustrum.sides[1].normal.z = sh;
        frustrum.sides[1].distance = 0;

        // plano de abajo
        frustrum.sides[2].normal.x = 0;
        frustrum.sides[2].normal.y = cv;
        frustrum.sides[2].normal.z = sv;
        frustrum.sides[2].distance = 0;

        // plano de arriba
        frustrum.sides[3].normal.x=0;
        frustrum.sides[3].normal.y=-cv;
        frustrum.sides[3].normal.z=sv;
        frustrum.sides[3].distance = 0;

        // plano de corte en z cercano
        frustrum.znear.normal.x=0;
        frustrum.znear.normal.y=0;
        frustrum.znear.normal.z=1;
        frustrum.znear.distance	= 35;
}


//Producto punto
double dotproduct(struct vertex_type *vec1, struct tvector *vec2)
{
        return vec1->cx * vec2->x +
               vec1->cy * vec2->y +
               vec1->cz * vec2->z;
}

//Corte contra un plano
void ClipToPlane(struct	polygon_type *polyin, struct polygon_type *polyout,
		 struct	plane_type *plane)
{
	int i;
	struct	vertex_type *verta, *vertb;

	float da;   // distancia del plano al v‚rtice a
	float db;   // distancia del plano al v‚rtice b
	float t;    // par metro

	int curvert = 0;  //v‚rtice actual

	verta = &(polyin->vertex[polyin->n_vertex - 1]);
	da = dotproduct(verta,	&plane->normal) - plane->distance;

	for(i=0; i < polyin->n_vertex;	i++)
	{
	        vertb =	&(polyin->vertex[i]);
		db = dotproduct(vertb, &plane->normal) - plane->distance;

		if(da>=0 && db>=0)
		{
			polyout->vertex[curvert].cx = vertb->cx;
			polyout->vertex[curvert].cy = vertb->cy;
			polyout->vertex[curvert].cz = vertb->cz;

			polyout->vertex[curvert].mapx = vertb->mapx;
			polyout->vertex[curvert].mapy = vertb->mapy;
			curvert++;
		}
               	if(da>0	&& db<0)
		{
			t = da/(da-db);	 //factor de intersecci¢n (0...1)

			polyout->vertex[curvert].cx = verta->cx	+
						      t*(vertb->cx-verta->cx);
			polyout->vertex[curvert].cy = verta->cy	+
						      t*(vertb->cy-verta->cy);
			polyout->vertex[curvert].cz = verta->cz	+
						      t*(vertb->cz-verta->cz);

                       	polyout->vertex[curvert].mapx = verta->mapx +
                       	                                t * (vertb->mapx -
                       	                                verta->mapx);
                       	polyout->vertex[curvert].mapy = verta->mapy +
                       	                                t * (vertb->mapy -
                       	                                verta->mapy);
			curvert++;
		}
		if(da<0	&& db>0)
		{
			t = da/(da-db);	 //factor de intersecci¢n (0...1)

			polyout->vertex[curvert].cx = verta->cx	+
						      t*(vertb->cx-verta->cx);
			polyout->vertex[curvert].cy = verta->cy	+
						      t*(vertb->cy-verta->cy);
			polyout->vertex[curvert].cz = verta->cz	+
						      t*(vertb->cz-verta->cz);

                       	polyout->vertex[curvert].mapx = verta->mapx +
                       	                                t * (vertb->mapx -
                       	                                verta->mapx);
                       	polyout->vertex[curvert].mapy = verta->mapy +
                       	                                t * (vertb->mapy -
                       	                                verta->mapy);

			curvert++;

			polyout->vertex[curvert].cx = vertb->cx;
			polyout->vertex[curvert].cy = vertb->cy;
			polyout->vertex[curvert].cz = vertb->cz;

			polyout->vertex[curvert].mapx = vertb->mapx;
			polyout->vertex[curvert].mapy = vertb->mapy;
                        curvert++;

		}
		verta =	&(polyin->vertex[i]);
                da = db;
	}
	polyout->n_vertex = curvert;
}

//Corte contra el Frustrum
int ClipToFrustrum(struct polygon_type *polyin, struct polygon_type *polyout)
{
        struct polygon_type polytemp;
        memcpy(polyout,polyin,(sizeof(struct polygon_type)));
        ClipToPlane(polyin,polyout,&frustrum.sides[0]);
        ClipToPlane(polyout,&polytemp,&frustrum.sides[1]);
        ClipToPlane(&polytemp,polyout,&frustrum.sides[2]);
	ClipToPlane(polyout,&polytemp,&frustrum.sides[3]);
	ClipToPlane(&polytemp,polyout,&frustrum.znear);
        return (polyout->n_vertex);
}

#include "3d.h"

extern unsigned char *VScreen,*VScreen2;

extern unsigned int *zbuffer;

//centro de la pantalla
extern float origenx_3d,origeny_3d;

//lente
extern float lenx,leny;

//textura
extern unsigned char texture[NUM_TEXTURAS][128][128];


//Esta funci¢n permite conocer si un pol¡gono est  de cara o de espaldas
//a la camara, seg£n devuelva un valor mayor o menor cero.
float Hidden(struct polygon_type *pol)
{
        struct vertex_type *v3,*v1,*v2;
        v1=&(pol->vertex[0]);
        v2=&(pol->vertex[1]);
        v3=&(pol->vertex[2]);

        return ((v3->cx*((v1->cz*v2->cy)-(v1->cy*v2->cz)))+
                (v3->cy*((v1->cx*v2->cz)-(v1->cz*v2->cx)))+
                (v3->cz*((v1->cy*v2->cx)-(v1->cx*v2->cy))));
}

//Importante funci¢n que calcula las coordenadas de un pol¡gono en una
//zona del mundo tridimensional y con un determiando  ngulo de giro
void PolygonCalcule(struct polygon_type *pol,float wx,float wy,float wz,float a, float b, float c)
{
        short int n;
        float x,y,z,nx,ny,nz;

        for(n=0;n<(pol->n_vertex);n++)
        {
        //Uso de variables temporales para simplificar
               x=(pol->vertex[n].rx);
               y=(pol->vertex[n].ry);
               z=(pol->vertex[n].rz);

        //Rotaciones

        //Rotaci¢n en el eje Z
               nx=x*cos(c)-y*sin(c);
               ny=y*cos(c)+x*sin(c);

        //Rotaci¢n en el eje X
               y=ny*cos(a)-z*sin(a);
               z=ny*sin(a)+z*cos(a);

        //Rotaci¢n en el eje Y
               nz=z*cos(b)-nx*sin(b);
               nx=z*sin(b)+nx*cos(b);

        //Suma al poligono sus coordenas en el mundo tridimensional
               (pol->vertex[n].wx)=nx+wx;
               (pol->vertex[n].wy)=y+wy;
               (pol->vertex[n].wz)=nz+wz;
        }
}

//Transforma las coordenas 3D en coordenadas 2D de pantalla
void ProjectPolygon(struct polygon_type *pol)
{
        short int n;
        for(n=0;n<(pol->n_vertex);n++)
        {
          //Transforma las coordenadas 3D en coordenadas 2D
         (pol->vertex[n].sx)=((pol->vertex[n].cx)*lenx)/(pol->vertex[n].cz)+origenx_3d;
         (pol->vertex[n].sy)=((-pol->vertex[n].cy)*leny)/(pol->vertex[n].cz)+origeny_3d;

         //Coordenada Z para el c lculo del Zbuffer
         (pol->vertex[n].zbuff)=pol->vertex[n].cz;
        }
}




// TABLAS USADAS POR "POLYGONDRAWFILLED"

//Contiene el pol¡gono desconpuesto en l¡neas horizontales
float minx2d[maxalto],maxx2d[maxalto];

//Contiene la profundidad en Z del pol¡gono desconpuesto en
//l¡neas horizontales
float minx2dz[maxalto],maxx2dz[maxalto];

//Contiene las coordenadas del texture mapping descompuestas en
//l¡neas horizontales
float minx2dmx[maxalto],maxx2dmx[maxalto];
float minx2dmy[maxalto],maxx2dmy[maxalto];


//Esta funci¢n que textura un poligono a partir de sus coordenadas
//en 2 dimensiones
void PolygonDrawFilledTextured(struct polygon_type *poligono)
{
        //Variables de tipo entero que se usan dentro de la funci¢n
        short int n,n2,n5,n6,x,y;

        //Punteros a vertices, se usan para acceder m s rapidamente a los datos
        struct vertex_type *ver1,*ver2,*veraux;

        //Coordenadas m¡nima y m xima en altura del poligono
        float miny2d=10000,maxy2d=-10000;

        //Contador y acumulador
        float inc,cont,zbuffinc,zbuffcont;

        //Contador y acumulador para el texture mapping
        float contmx,contmy,incmx,incmy;

        //Se inicializa la tabla de l¡neas horizontales
        for(n=0;n<maxalto;n++)
        {
                minx2d[n]=10000;
                maxx2d[n]=-10000;
        }

        //Se A¤ade al final un vertice m s al pol¡gono que ser  el mismo que el
        //primero para que al hacer el bucle quede completamente cerrado
        poligono->vertex[poligono->n_vertex].sx=poligono->vertex[0].sx;
        poligono->vertex[poligono->n_vertex].sy=poligono->vertex[0].sy;

        //Coordena de Zbuffer
        poligono->vertex[poligono->n_vertex].zbuff=poligono->vertex[0].zbuff;
        //Coordena de Texture mapping
        poligono->vertex[poligono->n_vertex].mapx=poligono->vertex[0].mapx;
        poligono->vertex[poligono->n_vertex].mapy=poligono->vertex[0].mapy;

        //Se Carga ver1 y ver2 con los dos primeros vertices del pol¡gono

        ver1=&(poligono->vertex[0]);
        ver2=&(poligono->vertex[1]);

        //Este bucle convierte el pol¡gono en una serie de l¡neas horizontales
        for(n=0;n<((poligono->n_vertex));n++)
        {
             //Si el primer vertice est  por debajo del segundo los invertimos, para
             //as¡ evitar errores en los c lculos
             if((ver1->sy)>(ver2->sy))
             {
                   veraux=ver1;
                   ver1=ver2;
                   ver2=veraux;
             }

             //Limita la altura del pol¡gono
             if((ver1->sy)<(miny2d))
                   miny2d=ver1->sy;
             if((ver2->sy)>(maxy2d))
                   maxy2d=ver2->sy;

             //C lculo de los puntos intemedios para componer una l¡nea
             cont=ver1->sx;
             inc=((ver2->sx)-(ver1->sx))/((ver2->sy)-(ver1->sy)+1);

             //C lculo de puntos intermedios de profundidad para calcular
             //el Z buffer
             zbuffcont=ver1->zbuff;
             zbuffinc=((ver2->zbuff)-(ver1->zbuff))/((ver2->sy)-(ver1->sy)+1);

             //C lculo de puntos intermedios para texture mapping
             contmx=(ver1->mapx);
             incmx=(((ver2->mapx)-(ver1->mapx))/((ver2->sy)-(ver1->sy)+1));
             contmy=(ver1->mapy);
             incmy=(((ver2->mapy)-(ver1->mapy))/((ver2->sy)-(ver1->sy)+1));

             //Almacena el pol¡gono en la tabla de l¡neas horizontales
             n5=ver1->sy;
             n6=ver2->sy;
             for(n2=n5;n2<=n6;n2++)
             {
                  if(cont<minx2d[n2])
                  {
                       minx2d[n2]=cont;
                       minx2dz[n2]=zbuffcont;
                       minx2dmx[n2]=contmx;
                       minx2dmy[n2]=contmy;
                  }
                  if(cont>maxx2d[n2])
                  {
                       maxx2d[n2]=cont;
                       maxx2dz[n2]=zbuffcont;
                       maxx2dmx[n2]=contmx;
                       maxx2dmy[n2]=contmy;
                  }
                  cont+=inc;
                  zbuffcont+=zbuffinc;
                  contmx+=incmx;
                  contmy+=incmy;
                  }
                  //Se pasa a los siguientes vertices
                  ver1++;
                  ver2++;
             }

             //Pinta en la pantalla el pol¡gono descompuesto en l¡neas
             //horizontales comprobando el Zbuffer;
             if(miny2d<maxy2d)
             {
                  for(y=miny2d;y<=maxy2d;y++)
                  {
                       //Profundidad en Z de cada uno de las l¡neas
                       //horizontales
                       zbuffcont=minx2dz[y];
                       zbuffinc=((maxx2dz[y]-minx2dz[y])/
                                 (maxx2d[y]-minx2d[y]+1));

                       //Texture mapping de cada una de las l¡neas
                       //horizontales
                       contmx=minx2dmx[y];
                       incmx=((maxx2dmx[y]-minx2dmx[y])/
                              (maxx2d[y]-minx2d[y]+1));
                       contmy=minx2dmy[y];
                       incmy=((maxx2dmy[y]-minx2dmy[y])/
                              (maxx2d[y]-minx2d[y]+1));

                       for(x=minx2d[y];x<=maxx2d[y];x++)
                       {
                              //Si la profundidad del nuevo punto es menor
                              //que la del que ocupaba dicha posicion
                              //en el fondo de la pantalla se pinta el
                              //nuevo punto y se actualiza la profundidad.
                              //En caso contrario, no se pintar  el punto
                              //y tampoco se actualizar  la profundidad.
                              if(zbuffcont<(zbuffer[(y<<8)+(y<<6)+x]))
                              {
                                put_pixel(x,y,texture[poligono->texture]
                                                     [(int)contmx]
                                                     [(int)contmy], VScreen);
                                zbuffer[(y<<8)+(y<<6)+x]=zbuffcont;
                              }
                              zbuffcont+=zbuffinc;
                              contmx+=incmx;
                              contmy+=incmy;
                       }
                  }
             }
        }

//Esta funci¢n pinta un poligono s¢lido a partir de sus
//coordenadas en 2 dimensiones
void PolygonDrawFilledSolid(struct polygon_type *poligono)
{
        //Variables de tipo entero que se usan dentro de la funci¢n
        short int n,n2,n5,n6,x,y;

        //Punteros a vertices, se usan para acceder m s rapidamente
        //a los datos
        struct vertex_type *ver1,*ver2,*veraux;

        //Coordenadas m¡nima y m xima en altura del poligono
        float miny2d=10000,maxy2d=-10000;

        //Contador y acumulador
        float inc,cont,zbuffinc,zbuffcont;

        //Se inicializa la tabla de l¡neas horizontales
        for(n=0;n<maxalto;n++)
        {
                minx2d[n]=10000;
                maxx2d[n]=-10000;
        }

        //Se A¤ade al final un vertice m s al pol¡gono que ser  el mismo
        //que el primero para que al hacer el bucle quede completamente
        //cerrado
        poligono->vertex[poligono->n_vertex].sx=poligono->vertex[0].sx;
        poligono->vertex[poligono->n_vertex].sy=poligono->vertex[0].sy;

        //Coordena de Zbuffer
        poligono->vertex[poligono->n_vertex].zbuff=poligono->vertex[0].zbuff;

        //Se Carga ver1 y ver2 con los dos primeros vertices del pol¡gono
        ver1=&(poligono->vertex[0]);
        ver2=&(poligono->vertex[1]);

        //Este bucle convierte el pol¡gono en una serie de l¡neas horizontales
        for(n=0;n<((poligono->n_vertex));n++)
        {
                //Si el primer vertice est  por debajo del segundo los
                //invertimos, para as¡ evitar errores en los c lculos
                if((ver1->sy)>(ver2->sy))
                {
                       veraux=ver1;
                       ver1=ver2;
                       ver2=veraux;
                }

                //Limita la altura del pol¡gono
                if((ver1->sy)<(miny2d))
                       miny2d=ver1->sy;
                if((ver2->sy)>(maxy2d))
                       maxy2d=ver2->sy;

                //C lculo de los puntos intemedios para componer una l¡nea
                cont=ver1->sx;
                inc=((ver2->sx)-(ver1->sx))/((ver2->sy)-(ver1->sy)+1);

                //C lculo de puntos intermedios de profundidad para
                //calcular el Z buffer
                zbuffcont=ver1->zbuff;
                zbuffinc=((ver2->zbuff)-(ver1->zbuff))/
                          ((ver2->sy)-(ver1->sy)+1);

                //Almacena el pol¡gono en la tabla de l¡neas horizontales
                n5=ver1->sy;
                n6=ver2->sy;
                for(n2=n5;n2<=n6;n2++)
                {
                       if(cont<minx2d[n2])
                       {
                            minx2d[n2]=cont;
                            minx2dz[n2]=zbuffcont;
                       }
                       if(cont>maxx2d[n2])
                       {
                            maxx2d[n2]=cont;
                            maxx2dz[n2]=zbuffcont;
                       }
                       cont+=inc;
                       zbuffcont+=zbuffinc;
                }

                //Se pasa a los siguientes vertices
                ver1++;
                ver2++;
        }

        //Pinta en la pantalla el pol¡gono descompuesto en l¡neas horizontales
        //Comprobando el Zbuffer;
        if(miny2d<maxy2d)
        {
                for(y=miny2d;y<=maxy2d;y++)
                {
                       //Profundidad en Z de cada uno de las l¡neas
                       //horizontales
                       zbuffcont=minx2dz[y];
                       zbuffinc=((maxx2dz[y]-minx2dz[y])/
                                 (maxx2d[y]-minx2d[y]+1));

                       for(x=minx2d[y];x<=maxx2d[y];x++)
                       {

                             //Si la profundidad del nuevo punto es menor
                             //que la del que ocupaba dicha posicion en el
                             //fondo de la pantalla se pinta el nuevo punto
                             //y se actualiza la profundidad.
                             //En caso contrario, no se pintar  el punto
                             //y tampoco se actualizar  la profundidad.
                             if(zbuffcont<(zbuffer[(y<<8)+(y<<6)+x]))
                             {
                                   put_pixel(x,y,poligono->color,VScreen);
                                   zbuffer[(y<<8)+(y<<6)+x]=zbuffcont;
                             }
                             zbuffcont+=zbuffinc;
                       }
                }
        }
}

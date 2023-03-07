/****************************************************************************
 Proyecto:
           Clipping 3D mediante frustrum

****************************************************************************/

#include <time.h>
#include <conio.h>
#include "3d.h"
#include "camera.h"
#include "input.h"
#include "tga.h"
#include "clip3d.h"

//pantallas
extern unsigned char *Screen, *VScreen, *VScreen2;

//teclado
extern unsigned char kbd[128];

//textura
extern unsigned char texture[80][80];

//matriz de la c mara
extern float matrixcamera[3][3];

//posici¢n de la c mara con respecto al mundo tridimensional
struct Point3D camera;


//Objeto de prueba
struct object_type ob1;

struct object_type ob2;

//contador de frames (para testear el nivel de optimizaci¢n alcanzado)
long frame_count=0;


/*
Dibuja un objeto 3D de acuerdo a:
1ro) la rotaci¢n en su propio espacio u "object space" para cada uno de los
     ejes seg£n los  ngulos a, b y c;
2do) el cambio de posici¢n en el espacio del mundo o "world space" seg£n
     las traslaciones dadas por wx, wy, wz;
3ro) la transformaci¢n de las coordenadas dadas en "world space" al
     del "camera space" por medio de matrixrotated y camera;
4to) Se crea un nuevo pol¡gono con los cortes necesarios contra el frustrum;
5to) la proyecci¢n de las coordenadas en el espacio de la c mara al del
     espacio 2D de la pantalla;
6to) Y por £ltimo de acuerdo a las coordenadas del la pantalla efectua el
     llenado del pol¡gono (clippoly).
*/
void DrawObject3D(struct object_type *obj, float wx, float wy, float wz,
             float a, float b, float c)
{
        short int aux;
        struct polygon_type clippoly;

        //Se calculan y pintan los pol¡gonos del objeto en la pantalla l¢gica
        for(aux=0;aux<(obj->n_polygon);aux++)
        {
                //En el mundo 3D
                PolygonCalcule(&obj->polygon[aux],wx,wy,wz,a,b,c);

                //camera movida y rotada
                World2Camera(matrixcamera,&camera,&obj->polygon[aux]);

                //Comprueba si el pol¡gono est  de frente o
                //de espaldas a la c mara
                if( (Hidden(&obj->polygon[aux])<0) || (obj->n_polygon==1))
                {
                     if(ClipToFrustrum(&obj->polygon[aux],&clippoly))
                     {
                         //Pasa las coordenadas 3D (camera space) a
                         //coordenadas 2D de pantalla
                         ProjectPolygon(&clippoly);

                         if((obj->polygon[aux].mask) & POLY_TEXTUREMAPPED)
                              PolygonDrawFilledTextured(&clippoly);

                         //Es Solido
                         else
                              PolygonDrawFilledSolid(&clippoly);
                     }
                }
        }
}

//Funci¢n que actualiza la pantalla
void Draw(struct object_type *obj, float wx, float wy, float wz,
          float a, float b, float c, float anglex, float angley, float anglez)
{

        float matrixr[3][3];           //Matrix de rotaci¢n

        //Se inicializa la pantalla l¢gica de trabajo con el contenido de
        //la pantalla l¢gica de fondo
        screen_copy(VScreen, VScreen2);

        RotateMatrix(matrixr, anglex, angley, anglez);

        ProductMatrix(matrixr, matrixcamera);

        //Inicializa la pantalla de profundidades o zbuffer
        ClearZbuffer();

        //Dibujan el mismo objeto en distintas posiciones y con diferentes
        // ngulos de rotaci¢n
        DrawObject3D(obj,wx,wy,wz,a,b,c);

        //floor
        DrawObject3D(&ob2,0,-190,750,0,0,0);
        DrawObject3D(obj,(wx+180),(wy-50),(wz+450),a/2,b,c/2);

        //Se pasa el contenido de la pantalla l¢gica de trabajo a la pantalla
        //f¡sica, es decir, al monitor propiamente dicho
        screen_copy(Screen,VScreen);

        frame_count++;
}


//Principal
int main(int argc, char * argv[] )
{
        //¡ndice
        unsigned char x;

        //movimientos de la c mara
        float xx =0,yy=0,zz=0;

        // ngulos de giro de la c mara
        float anglex = 0,angley = 0,anglez = 0;

        //movimientos o rotaciones del objeto
        float a = 0, b = 0, c = 0;

        int text = 0;       //permite el cambio de texturas de las
                            //caras del cubo

        clock_t start_time, end_time;

        //Inicializan matriz y c mara
        InitMatrix(matrixcamera);
        InitCamera(&camera);

        setup_frustrum();

        clrscr();
        printf("\n\n\n  Corte 3D mediante frustrum, simulando la visi¢n con una pir mide truncada\n\n\n\n     Teclas:\n
            flecha izq. y der.     -->   sobre el eje x
            flecha arriba y abajo  -->   sobre el eje y
            a, z                   -->   sobre el eje z (acercarse, alejarse)
            Home, End              -->   rotaci¢n sobre el eje x
            Insert, Page Up        -->   rotaci¢n sobre el eje y
            Delete, Page Down      -->   rotaci¢n sobre el eje z\n\n
            S                      -->   rotaci¢n de texturas\n");
        getch();

        InitGraphicMode();

        if(argc>1)
                //Carga la textura
                LoadTarga(argv[1]);
        else
        {
                ShutDownGraphicMode();
                printf("\n\tOoops falta par metro requerido: \n\n
                nombre_archivo.tga\n\n\n      con 28 texturas dispuestas verticalmente\n      de 128 x 128 x 256  colores \n");
                exit(1);
        }

        //Inicializa el objeto 3D de prueba
        //N£mero de pol¡gonos del objeto 3D
        ob1.n_polygon=6;

        //Color del pol¡gono 0
        ob1.polygon[0].color=10;
        //Tipo del pol¡gono 0
        ob1.polygon[0].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[0].texture = 0;
        //N£mero de v‚rtices del pol¡gono 0
        ob1.polygon[0].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[0].vertex[0].rx=-50;
        ob1.polygon[0].vertex[0].ry=-50;
        ob1.polygon[0].vertex[0].rz=-50;
        ob1.polygon[0].vertex[1].rx=50;
        ob1.polygon[0].vertex[1].ry=-50;
        ob1.polygon[0].vertex[1].rz=-50;
        ob1.polygon[0].vertex[2].rx=50;
        ob1.polygon[0].vertex[2].ry=50;
        ob1.polygon[0].vertex[2].rz=-50;
        ob1.polygon[0].vertex[3].rx=-50;
        ob1.polygon[0].vertex[3].ry=50;
        ob1.polygon[0].vertex[3].rz=-50;


        //Color del pol¡gono 1
        ob1.polygon[1].color=11;
        //Tipo del pol¡gono 1
        ob1.polygon[1].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[1].texture = 1;
        //N£mero de v‚rtices del pol¡gono 1
        ob1.polygon[1].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[1].vertex[0].rx=-50;
        ob1.polygon[1].vertex[0].ry=-50;
        ob1.polygon[1].vertex[0].rz=50;
        ob1.polygon[1].vertex[1].rx=50;
        ob1.polygon[1].vertex[1].ry=-50;
        ob1.polygon[1].vertex[1].rz=50;
        ob1.polygon[1].vertex[2].rx=50;
        ob1.polygon[1].vertex[2].ry=-50;
        ob1.polygon[1].vertex[2].rz=-50;
        ob1.polygon[1].vertex[3].rx=-50;
        ob1.polygon[1].vertex[3].ry=-50;
        ob1.polygon[1].vertex[3].rz=-50;


        //Color del pol¡gono 2
        ob1.polygon[2].color=12;
        //Tipo del pol¡gono 2
        ob1.polygon[2].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[2].texture = 2;
        //N£mero de v‚rtices del pol¡gono 2
        ob1.polygon[2].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[2].vertex[0].rx=-50;
        ob1.polygon[2].vertex[0].ry=50;
        ob1.polygon[2].vertex[0].rz=50;
        ob1.polygon[2].vertex[1].rx=50;
        ob1.polygon[2].vertex[1].ry=50;
        ob1.polygon[2].vertex[1].rz=50;
        ob1.polygon[2].vertex[2].rx=50;
        ob1.polygon[2].vertex[2].ry=-50;
        ob1.polygon[2].vertex[2].rz=50;
        ob1.polygon[2].vertex[3].rx=-50;
        ob1.polygon[2].vertex[3].ry=-50;
        ob1.polygon[2].vertex[3].rz=50;

        //Color del pol¡gono 3
        ob1.polygon[3].color=13;
        //Tipo del pol¡gono 3
        ob1.polygon[3].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[3].texture = 3;
        //N£mero de v‚rtices del pol¡gono 3
        ob1.polygon[3].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[3].vertex[0].rx=-50;
        ob1.polygon[3].vertex[0].ry=50;
        ob1.polygon[3].vertex[0].rz=-50;
        ob1.polygon[3].vertex[1].rx=50;
        ob1.polygon[3].vertex[1].ry=50;
        ob1.polygon[3].vertex[1].rz=-50;
        ob1.polygon[3].vertex[2].rx=50;
        ob1.polygon[3].vertex[2].ry=50;
        ob1.polygon[3].vertex[2].rz=50;
        ob1.polygon[3].vertex[3].rx=-50;
        ob1.polygon[3].vertex[3].ry=50;
        ob1.polygon[3].vertex[3].rz=50;

        //Color del pol¡gono 4
        ob1.polygon[4].color=14;
        //Tipo del pol¡gono 4
        ob1.polygon[4].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[4].texture = 4;
        //N£mero de v‚rtices del pol¡gono 4
        ob1.polygon[4].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[4].vertex[0].rx=50;
        ob1.polygon[4].vertex[0].ry=-50;
        ob1.polygon[4].vertex[0].rz=-50;
        ob1.polygon[4].vertex[1].rx=50;
        ob1.polygon[4].vertex[1].ry=-50;
        ob1.polygon[4].vertex[1].rz=50;
        ob1.polygon[4].vertex[2].rx=50;
        ob1.polygon[4].vertex[2].ry=50;
        ob1.polygon[4].vertex[2].rz=50;
        ob1.polygon[4].vertex[3].rx=50;
        ob1.polygon[4].vertex[3].ry=50;
        ob1.polygon[4].vertex[3].rz=-50;

        //Color del pol¡gono 5
        ob1.polygon[5].color=15;
        //Tipo del pol¡gono 5
        ob1.polygon[5].mask = POLY_TEXTUREMAPPED;
        //N£mero de textura asignada
        ob1.polygon[5].texture = 5;
        //N£mero de v‚rtices del pol¡gono 5
        ob1.polygon[5].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob1.polygon[5].vertex[0].rx=-50;
        ob1.polygon[5].vertex[0].ry=-50;
        ob1.polygon[5].vertex[0].rz=50;
        ob1.polygon[5].vertex[1].rx=-50;
        ob1.polygon[5].vertex[1].ry=-50;
        ob1.polygon[5].vertex[1].rz=-50;
        ob1.polygon[5].vertex[2].rx=-50;
        ob1.polygon[5].vertex[2].ry=50;
        ob1.polygon[5].vertex[2].rz=-50;
        ob1.polygon[5].vertex[3].rx=-50;
        ob1.polygon[5].vertex[3].ry=50;
        ob1.polygon[5].vertex[3].rz=50;


        //Asigna las coordenadas de texture maping en el engine 3D
        //Para
        for(x = 0; x < ob1.n_polygon; x++)
        {
                ob1.polygon[x].vertex[0].mapx=1;
                ob1.polygon[x].vertex[0].mapy=127;
                ob1.polygon[x].vertex[1].mapx=127;
                ob1.polygon[x].vertex[1].mapy=127;
                ob1.polygon[x].vertex[2].mapx=127;
                ob1.polygon[x].vertex[2].mapy=1;
                ob1.polygon[x].vertex[3].mapx=1;
                ob1.polygon[x].vertex[3].mapy=1;
        }


        //Inicializa el segundo objeto 3D
        //N£mero de pol¡gonos del objeto 3D
        ob2.n_polygon=1;

        //Color del pol¡gono 0
        ob2.polygon[0].color=67;
        //Tipo del pol¡gono 0
        ob2.polygon[0].mask = POLY_SOLID;
        //N£mero de v‚rtices del pol¡gono 0
        ob2.polygon[0].n_vertex=4;
        //Coordenadas X,Y y Z de cada uno de los v‚rtices de cada pol¡gono
        ob2.polygon[0].vertex[0].rx=-500;
        ob2.polygon[0].vertex[0].ry=0;
        ob2.polygon[0].vertex[0].rz=-500;
        ob2.polygon[0].vertex[1].rx=500;
        ob2.polygon[0].vertex[1].ry=0;
        ob2.polygon[0].vertex[1].rz=-500;
        ob2.polygon[0].vertex[2].rx=500;
        ob2.polygon[0].vertex[2].ry=0;
        ob2.polygon[0].vertex[2].rz=500;
        ob2.polygon[0].vertex[3].rx=-500;
        ob2.polygon[0].vertex[3].ry=0;
        ob2.polygon[0].vertex[3].rz=500;

        //Inicialiaci¢n del manejador del teclado
        key_init();

        start_time=clock();

        while(!kbd[MAKE_ESC])
        {
                if(kbd[MAKE_DOWN])
                       yy=-5.5;
                if(kbd[MAKE_RIGHT])
                       xx=5.5;
                if(kbd[MAKE_UP])
                       yy=5.5;
                if(kbd[MAKE_LEFT])
                       xx=-5.5;
                if(kbd[MAKE_A])
                       zz=5.5;
                if(kbd[MAKE_Z])
                       zz=-5.5;
                if(kbd[MAKE_END])
                       anglex=-0.055;
                if(kbd[MAKE_HOME])
                       anglex=0.055;
                if(kbd[MAKE_INS])
                       angley=0.055;
                if(kbd[MAKE_PGUP])
                       angley=-0.055;
                if(kbd[MAKE_DEL])
                       anglez=-0.055;
                if(kbd[MAKE_PGDWN])
                       anglez=0.055;
                if(kbd[MAKE_T])
                       InitCamera(&camera);
                if(kbd[MAKE_Y])
                       InitMatrix(matrixcamera);
                if(kbd[MAKE_S])
                       text = 1;    //rotar texturas

                //Convierte la nueva posici¢n de la c mara
                //(dada en el "space camera") a su correspondiente posici¢n
                //en el "world space"
                Camera2World(matrixcamera,&camera,xx,yy,zz);

                //Para rotar las texturas sobre las caras del cubo
                if(text)
                {
                       for(x=0;x<(ob1.n_polygon);x++)
                       {
                            if(ob1.polygon[x].texture == (NUM_TEXTURAS-1))
                                   ob1.polygon[x].texture = x;
                            ob1.polygon[x].texture += text;
                       }
                       text = 0;
                }

                a+=0.2; b+=0.05; c+=0.002;
                Draw(&ob1,0,0,750,c,c,b,anglex,angley,anglez);


                xx=yy=zz=0;             //Retorna los cambios de posici¢n
                                        //de la c mara a 0, para que estos
                anglex=angley=anglez=0; //no sigan moviendo y rotando la
                                        //c mara.
        }

        end_time=clock();

        //Retorno al modo texto
        ShutDownGraphicMode();

        // deshabilita el manejador del teclado
	key_delete();

        clrscr();
        printf("\n\n\tframes =   %f\n\n", (float)(frame_count));
        printf("\tsegs.  =   %f\n\n",
                ((float)(end_time-start_time)/CLOCKS_PER_SEC));

        printf("\tfps    =   %f\n\n",
        (float)(frame_count)/((float)(end_time-start_time)/CLOCKS_PER_SEC));

        printf("\n  Realizando el corte 3D de los pol¡gonos.\n                              ");
        exit(0);
}

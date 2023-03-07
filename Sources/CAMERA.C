#include "camera.h"

//matriz de la c mara
float matrixcamera[3][3];

//matriz inversa
float matrixinverse[3][3];

//Matriz Identidad
void InitMatrix(float matrixcam[3][3])
{
        matrixcam[0][0]=1; matrixcam[0][1]=0; matrixcam[0][2]=0; matrixcam[1][0]=0;
        matrixcam[1][1]=1; matrixcam[1][2]=0; matrixcam[2][0]=0; matrixcam[2][1]=0;
        matrixcam[2][2]=1;
}

//Inicializo la posici¢n de la c mara en el mundo 3D
void InitCamera(struct Point3D *pcamera)
{
        pcamera->x=0;
        pcamera->y=0;
        pcamera->z=0;
}

//Roto la matriz seg£n los angulos a, b, c
void RotateMatrix(float camera_rotation[3][3], float a, float b, float c)
{

        float xsin, xcos, ysin, ycos, zsin, zcos;
        float sxsz, sxcz, szcx, cxcz;

        xsin = sin(a);
        xcos = cos(a);
        ysin = sin(b);
        ycos = cos(b);
        zsin = sin(c);
        zcos = cos(c);

        sxsz = xsin * zsin;
        sxcz = xsin * zcos;
        szcx = zsin * xcos;
        cxcz = xcos * zcos;

        camera_rotation[0][0]=ycos * zcos;
        camera_rotation[1][0]=ycos * zsin;
        camera_rotation[2][0]=-ysin;

        camera_rotation[0][1]=ysin * sxcz - szcx;
        camera_rotation[1][1]=ysin * sxsz + cxcz;
        camera_rotation[2][1]=xsin * ycos;

        camera_rotation[0][2]=ysin * cxcz + sxsz;
        camera_rotation[1][2]=ysin * szcx - sxcz;
        camera_rotation[2][2]=xcos * ycos;
}

//producto matricial
void ProductMatrix(float m1[3][3], float m2[3][3])
{
        float mtmp[3][3];

        mtmp[0][0]= m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0];
        mtmp[0][1]= m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1];
        mtmp[0][2]= m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2];

        mtmp[1][0]= m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0];
        mtmp[1][1]= m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1];
        mtmp[1][2]= m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2];

        mtmp[2][0]= m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0];
        mtmp[2][1]= m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1];
        mtmp[2][2]= m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2];

        memcpy(m2,mtmp,sizeof(mtmp));

}

//Convierte las coordenadas de cada v‚rtice del pol¡gono dadas en el
//espacio 3D a sus correspondientes coordenadas en el espacio de la
//c mara
void World2Camera(float matrix[3][3],struct Point3D *camera,
                  struct polygon_type *pol)
{
        short int n;
        struct Point3D temp;

        for(n=0;n<(pol->n_vertex);n++)
        {
                temp.x = pol->vertex[n].wx - camera->x;
                temp.y = pol->vertex[n].wy - camera->y;
                temp.z = pol->vertex[n].wz - camera->z;

                pol->vertex[n].cx = (matrix[0][0] * temp.x) +
                                    (matrix[0][1] * temp.y) +
                                    (matrix[0][2] * temp.z);

                pol->vertex[n].cy = (matrix[1][0] * temp.x) +
                                    (matrix[1][1] * temp.y) +
                                    (matrix[1][2] * temp.z);

                pol->vertex[n].cz = (matrix[2][0] * temp.x) +
                                    (matrix[2][1] * temp.y) +
                                    (matrix[2][2] * temp.z);

        }
}

//determinante de una matriz
double Determinant(float matrix[3][3])
{
        return matrix[0][0] *
              (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
              -matrix[0][1] *
              (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
              +matrix[0][2] *
              (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

//traspone la matriz
void Transpose (float matrix[3][3])
{
        double swap;

        swap = matrix[0][1]; matrix[0][1] = matrix[1][0]; matrix[1][0] = swap;
        swap = matrix[0][2]; matrix[0][2] = matrix[2][0]; matrix[2][0] = swap;
        swap = matrix[1][2]; matrix[1][2] = matrix[2][1]; matrix[2][1] = swap;
}

//calcula la inversa de una matriz
void Inverse (float matrix[3][3], float inverse[3][3])
{
        double s = 1.0/Determinant(matrix);

        inverse[0][0] = (matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1]);
        inverse[0][1] =-(matrix[1][0]*matrix[2][2]-matrix[1][2]*matrix[2][0]);
        inverse[0][2] = (matrix[1][0]*matrix[2][1]-matrix[1][1]*matrix[2][0]);
        inverse[1][0] =-(matrix[0][1]*matrix[2][2]-matrix[0][2]*matrix[2][1]);
        inverse[1][1] = (matrix[0][0]*matrix[2][2]-matrix[0][2]*matrix[2][0]);
        inverse[1][2] =-(matrix[0][0]*matrix[2][1]-matrix[0][1]*matrix[2][0]);
        inverse[2][0] = (matrix[0][1]*matrix[1][2]-matrix[0][2]*matrix[1][1]);
        inverse[2][1] =-(matrix[0][0]*matrix[1][2]-matrix[0][2]*matrix[1][0]);
        inverse[2][2] = (matrix[0][0]*matrix[1][1]-matrix[0][1]*matrix[1][0]);

        Transpose (inverse);

        inverse[0][0] *= s; inverse[0][1]  *= s; inverse[0][2]  *= s;
        inverse[1][0] *= s; inverse[1][1]  *= s; inverse[1][2]  *= s;
        inverse[2][0] *= s; inverse[2][1]  *= s; inverse[2][2]  *= s;
}


//Convierte la posici¢n de la c mara dada en el "camera space" (x,y,z)
//a sus correspondientes coordenadas en el "world space"
void Camera2World(float cameraro[3][3],struct Point3D *camera,
                  float x,float y,float z)
{
        struct Point3D temp;

        Inverse(cameraro,matrixinverse);


        temp.x = (matrixinverse[0][0] * x) +
                 (matrixinverse[0][1] * y) +
                 (matrixinverse[0][2] * z);

        temp.y = (matrixinverse[1][0] * x) +
                 (matrixinverse[1][1] * y) +
                 (matrixinverse[1][2] * z);

        temp.z = (matrixinverse[2][0] * x) +
                 (matrixinverse[2][1] * y) +
                 (matrixinverse[2][2] * z);

        camera->x += temp.x;
        camera->y += temp.y;
        camera->z += temp.z;
}

#ifndef _CAMERA_
#define _CAMERA_

#include "3d.h"

struct Point3D
{
        float x,y,z;
};


void InitMatrix(float matrixcamera[3][3]);

void InitCamera(struct Point3D *pcamera);

void RotateMatrix(float camera_rotation[3][3], float a, float b, float c);

void ProductMatrix(float m1[3][3], float m2[3][3]);


void World2Camera(float matrix[3][3],struct Point3D *camera,
                  struct polygon_type *pol);

double Determinant(float matrix[3][3]);

void Transpose (float matrix[3][3]);

void Inverse (float matrix[3][3], float inverse[3][3]);

void Camera2World(float cameramatrix[3][3],struct Point3D *camera,
                  float x,float y,float z);

#endif


#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include <stdio.h>
#include <SDL2/SDL.h>
//Devuelve la velocidad en el proximo instante

double computar_velocidad(double vi, double a, double dt); 
//Devulve la posicion en el proximo instante

double computar_posicion(double pi, double vi, double dt); 

//Computa el rebote contra un objeto con esa normal modificando tanto el punto(cx,cy) como la velocidad(vx,vy)
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy); //

#endif

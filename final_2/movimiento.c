#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "movimiento.h"
#include "poligono.h"

double computar_velocidad(double vi, double a, double dt){
	return (dt*a)+vi;
}

double computar_posicion(double pi, double vi, double dt){
	return (dt*vi)+pi;
}

void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy) {
    float proy = producto_interno(norm_x, norm_y, *vx, *vy);

    if(proy >= 0)
        return;

    *vx -= 2 * norm_x * proy;
    *vy -= 2 * norm_y * proy;

    // Además empujamos a la bola hacia afuera para que no se pegue
    *cx += norm_x * 0.1;
    *cy += norm_y * 0.1;
}

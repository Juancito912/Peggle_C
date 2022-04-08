#ifndef POLIGONO_H
#define POLIGONO_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

/*
 * Definición de los tipos de datos
 */
struct poligono;

typedef struct poligono poligono_t;

/*
 * Primitivas de poligono_t
*/

//Crea un poligono
//Post: Devulve un poligono creado en memoria nueva con n cantidad de vertices y sus cordenadas
//Si no hay memoria, devulve NULL
poligono_t *poligono_crear(float vertices[][2], size_t n);

//Crea un poligono en forma de circulo
//Post: Devuelve un circulo creado en memoria nueva a partir del radio
//si no hay memoria devuelve NULL
poligono_t *poligono_crear_circular(float radio); 

//Destruye el poligono
//Post: Libera la memoria asociada al poligono
void poligono_destruir(poligono_t *poligono);

//Dibuja el poligono en pantalla con la biblioteca SDL2
//Pre: poligono fue creado
void poligono_dibujar(poligono_t *poligono, SDL_Renderer *renderer);

//Realiza el producto interno entre a y b
float producto_interno(float ax, float ay, float bx, float by); //PRODUCTO INTERNO

//Devuelve la distancia entre un poligono y el punto por salida
//Devuelve por interfaz la norma(norm_x,norm_y)
//Pre: poligono fue creado
double poligono_distancia(const poligono_t *p, float xp, float yp, float *norm_x, float *norm_y);

//Translada los puntos del poligono mediante la adicion del par (dx,dy)
//Pre: poligono fue creado
void poligono_trasladar(poligono_t *poligono, float dx, float dy);

//Rota los puntos del poligono rad radianes
//Pre: poligono fue creado
void poligono_rotar(poligono_t *poligono, double rad);

//Realiza una rotacion centrada entorno a un punto(cx,cy) con velocidad vel
//Pre: el poligono fue creado
void poligono_rotar_centrado (poligono_t *poligono, float cx, float cy, float vel);

//Devulve la cantidad de vertices del poligono
//Pre: poligono fue creado
size_t poligono_cantidad_vertices(const poligono_t *poligono); 

#endif

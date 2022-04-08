#ifndef OBSTACULOS_H
#define OBSTACULOS_H

#include <stdio.h>
#include <SDL2/SDL.h>

/*
 * Definición de los tipos de datos
 */

struct obstaculos;

typedef struct obstaculos obstaculos_t;

//Crea un obstaculo
//Pre:El archivo.bin fue creado y abierto en modo lectura
//Post: Devuelve un obstaculo creado en memoria nueva a partir de la lectura del archivo
//Si no hay memoria, devulve NULL
obstaculos_t *obstaculo_crear(FILE *f);

//Realiza el rebote entre un obstaculo y un punto(cx,cy)
//Post: Marca todos los obstaculos en el q rebota el punto(menos COLOR_GRIS)
void rebote(obstaculos_t *obstaculo, float *cx, float *cy, float *vx, float *vy);

//Destruye el obstaculo
//Pre: Obstaculo fue creado
//Libera la memoria asociada al obstaculo
void obstaculo_destruir(obstaculos_t *obstaculo);

//Devuelve TRUE si el obstaculo esta tocado, FALSE en caso contrario
//Pre: Obstaculo fue creado
bool esta_marcado(obstaculos_t *obstaculo);

//Contabiliza el puntaje del obstaculo dependiendo su color y la cantidad de naranjas tocadas
//Pre: El obstaculo esta marcado
//Pre: Obstaculo fue creado
//Post: Devuelve el puntaje
int contador_puntos(obstaculos_t *obstaculo, int naranjas);

//Devuelve TRUE si el obstaculo es de COLOR_NARANJA, FALSE en caso contrario
//Pre: Obstaculo fue creado
bool es_naranja(obstaculos_t *obstaculo);

//Dibuja el obstaculo con su correspondiente color y si esta marcado se colorea en AMARTILLO
//Pre: Obstaculo fue creado
void obstaculo_dibujar(obstaculos_t *obstaculo, SDL_Renderer *renderer);

//Desplaza el obstaculo en el eje x dependiendo sus parametros
//Pre: Obstaculo fue creado
void obstaculo_mov_horizontal(obstaculos_t *obstaculo);

//Llama a la funcion del movimiento correspondiente al obstaculo 
//Pre: Obstaculo fue creado
void obstaculo_aplicar_mov(obstaculos_t *obstaculo);

#endif
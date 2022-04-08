#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "poligono.h"

/*
 * Definición de los tipos de datos
 */

typedef enum {COLOR_AZUL, COLOR_NARANJA, COLOR_VERDE, COLOR_GRIS,COLOR_AMARILLO} color_t;
typedef enum {MOV_INMOVIL, MOV_CIRCULAR, MOV_HORIZONTAL} movimiento_t;
typedef enum {GEO_CIRCULO, GEO_RECTANGULO, GEO_POLIGONO} geometria_t;

typedef bool (*MOV_funciones_t)(FILE *f, int16_t parametros[], size_t *n_parametros);
typedef poligono_t*(*GEO_funciones_t)(FILE *f);

//Lee un byte del archivo y devuelve por interfaz su color, movimiento y geometria
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_encabezado(FILE *f,color_t *color, movimiento_t *movimiento, geometria_t *geometria);

//Devuelve por interfaz 0 en n_parametros 
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_movimiento_inmovil (FILE *f, int16_t parametros[], size_t *n_parametros);

//Lee del archivo los parametros del movimiento circular y los almacena en parametros
//Devuelve la cantidad de parametros leidos en n_parametros
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros);

//Lee del archivo los parametros del movimiento horizontal y los almacena en parametros
//Devuelve la cantidad de parametros leidos en n_parametos
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros);

//Lee del archivo el movimiento correspondiente al movimiento dado
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_movimiento (FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros);

//Lee del archivo y devulve por interfaz la cantidad de obstaculos
//Pre: El archivo.bin tiene q estar abierto y en modo lectura
//Devuelve TRUE si todo sale correctamente, FALSE en caso contrario
bool leer_cant_obstaculos(FILE *f,int16_t *n);

//Lee del archivo la informacion de la geometria circular
//Pre:el archivo.bin fue creado y abierto en modo lectura
//Devuelve NULL si la operacion no salio correctamente
//Post: Devuelve el circulo ya creado en memoria nueva y en su posicion
poligono_t *leer_geometria_circulo(FILE *f);

//Lee del archivo la informacion de la geometria rectangular
//Pre:el archivo.bin fue creado y abierto en modo lectura
//Devuelve NULL si la operacion no salio correctamente
//Post: Devuelve el rectangulo ya creado en memoria nueva y en su posicion
poligono_t *leer_geometria_rectangulo(FILE *f);

//Lee del archivo la informacion de la geometria poligonal
//Pre:el archivo.bin fue creado y abierto en modo lectura
//Devuelve NULL si la operacion no salio correctamente
//Post: Devuelve el poligono ya creado en memoria nueva y en su posicion
poligono_t *leer_geometria_poligono(FILE *f);

//Lee del archivo la informacion de la geometria correspondiente a la geometria dada
//Pre:el archivo.bin fue creado y abierto en modo lectura
//Devuelve NULL si la operacion no salio correctamente
poligono_t *leer_geometria(FILE*f, geometria_t geometria);

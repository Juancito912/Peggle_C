#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "lectura.h"
#include "poligono.h"
#include "movimiento.h"
#include "config.h"

#define MASK_COLOR 0xc0
#define MASK_MOV 0x30
#define MASK_GEO 0x0f

#define SHIFT_COLOR 6
#define SHIFT_MOV 4
#define SHIFT_GEO 0

MOV_funciones_t movimiento_fun[] = {
    [MOV_INMOVIL] = leer_movimiento_inmovil,
    [MOV_CIRCULAR] = leer_movimiento_circular,
    [MOV_HORIZONTAL] = leer_movimiento_horizontal,
};

GEO_funciones_t geometria_fun[]={
    [GEO_CIRCULO]= leer_geometria_circulo,
    [GEO_RECTANGULO]= leer_geometria_rectangulo,
    [GEO_POLIGONO]= leer_geometria_poligono,
};

bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria){
    uint8_t byte;
    if(fread (&byte,sizeof (uint8_t), 1, f) != 1) 
        return false;
    *color = (byte & MASK_COLOR) >> SHIFT_COLOR;
    *movimiento = (byte & MASK_MOV) >> SHIFT_MOV;
    *geometria = (byte & MASK_GEO) >> SHIFT_GEO;

    return true;
}

bool leer_movimiento_inmovil (FILE *f, int16_t parametros[], size_t *n_parametros){
    *n_parametros = 0;
    return true;
}
bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros){
    if(fread(parametros,sizeof(int16_t),3,f) != 3) 
        return false;
    *n_parametros = 3;
    return true;
}

bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros){
    
    if (fread(parametros,sizeof(int16_t),3,f) != 3)
        return false;
    *n_parametros = 3;
    
    return true;
}

bool leer_movimiento (FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros){
    return movimiento_fun [movimiento] (f, parametros, n_parametros);
}

poligono_t *leer_geometria_circulo(FILE *f){
    int16_t aux[3];
    if(fread(aux,sizeof(int16_t),3,f)!= 3)
        return NULL;
    float parametros_f[3];
    for(size_t i = 0; i<3; i++){
        parametros_f[i] = aux[i];
    }
    poligono_t *circulo = poligono_crear_circular(parametros_f[2]);
    if(circulo == NULL) 
        return NULL;
    poligono_trasladar(circulo,parametros_f[0],parametros_f[1]);
    
    return circulo;
}

poligono_t *leer_geometria_rectangulo(FILE *f){
    int16_t aux[5];
    if(fread(aux,sizeof(int16_t),5,f) != 5)
        return NULL;
    float x = aux[0];
    float y = aux[1];
    float desx = (aux[2]/2);
    float desy = (aux[3]/2);
    float rad = (aux[4]*PI)/180;

    float vertices[4][2] = {
        {aux[0]+desx,aux[1]-desy},
        {aux[0]+desx,aux[1]+desy},
        {aux[0]-desx,aux[1]+desy},
        {aux[0]-desx,aux[1]-desy}
    };
    poligono_t *rectangulo = poligono_crear(vertices, 4);
    if(rectangulo == NULL)
        return NULL;
    poligono_trasladar(rectangulo,-x,-y);
    poligono_rotar(rectangulo,rad);
    poligono_trasladar(rectangulo,x,y);

    return rectangulo;
}

poligono_t *leer_geometria_poligono(FILE *f){
    int16_t n;
    if (fread(&n,sizeof(int16_t),1,f) != 1)
        return NULL;
    
    int16_t aux[n][2];
    if (fread(&aux,sizeof(int16_t),(n*2),f) != (n*2))
        return NULL;

    float (*vertices)[2] = malloc(sizeof(float)*n*2);
    if(vertices == NULL)
        return NULL;   
    for(size_t i = 0; i < n; i++){
        vertices[i][0] = aux[i][0];
        vertices[i][1] = aux[i][1];
    }
    poligono_t *poligono = poligono_crear(vertices, n);
    if(poligono == NULL)
        return NULL;

    free(vertices);

    return poligono;
}

poligono_t *leer_geometria(FILE *f, geometria_t geometria){
    return geometria_fun [geometria] (f);
}

bool leer_cant_obstaculos(FILE *f, int16_t *n){
    if(!fread(n, sizeof(int16_t), 1, f))
        return false;

    return true;
}

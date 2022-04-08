#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "obstaculos.h"
#include "poligono.h"
#include "lectura.h"
#include "movimiento.h"
#include "config.h"

struct obstaculos {
    geometria_t geometria;
    movimiento_t movimiento;
    color_t color;
    bool tocado; //TRUE = TOCADO ; FALSE = NO TOCADO
    poligono_t *poligono;
    float parametros[3];
};

int puntuacion_color[] = {
    [COLOR_AZUL] = 10,
    [COLOR_NARANJA] = 100,
    [COLOR_VERDE] = 10,
};

typedef uint8_t RGB_t [3];
RGB_t colores[]= {
    [COLOR_AZUL] = {0x00, 0x00, 0xFF},
    [COLOR_NARANJA] = {0xEB, 0x73, 0x1E},
    [COLOR_VERDE] = {0x00, 0xFF, 0x00},
    [COLOR_GRIS] = {0x80, 0x80, 0x80},
    [COLOR_AMARILLO] = {0xFF, 0xF0, 0x00},
};

obstaculos_t *obstaculo_crear(FILE *f){
    obstaculos_t *obstaculo = malloc(sizeof(obstaculos_t));

    obstaculo->tocado = false;

    if(!leer_encabezado(f,&obstaculo->color, &obstaculo->movimiento, &obstaculo->geometria)){
        free(obstaculo);
        return NULL;
    }
    size_t n_parametros;
    int16_t parametros[3];
    if(!leer_movimiento(f,obstaculo->movimiento, parametros, &n_parametros)){
        free(obstaculo);
        return NULL;
    }
    for(size_t i= 0; i < n_parametros; i++){
        obstaculo->parametros[i] = parametros[i];
    }
    obstaculo->poligono = leer_geometria(f,obstaculo->geometria);
    
    return obstaculo;
}

void rebote(obstaculos_t *obstaculo, float *cx, float *cy, float *vx, float *vy){
    float norm_x, norm_y;
    double distancia = poligono_distancia(obstaculo->poligono, *cx, *cy, &norm_x, &norm_y);
    if((obstaculo->color == COLOR_GRIS) && (distancia < BOLA_RADIO)){
        reflejar(norm_x, norm_y, cx, cy, vx, vy);
        *vx *= PLASTICIDAD;
        *vy *= PLASTICIDAD;
    }
    else if((obstaculo->color != COLOR_GRIS) && (distancia < BOLA_RADIO)){
        reflejar(norm_x, norm_y, cx, cy, vx, vy);
        *vx *= PLASTICIDAD;
        *vy *= PLASTICIDAD;
        obstaculo->tocado = true;
    }
}

void obstaculo_destruir(obstaculos_t *obstaculo){
    poligono_destruir(obstaculo->poligono);
    free(obstaculo);
}

bool esta_marcado(obstaculos_t *obstaculo){
    return obstaculo->tocado;
}

bool es_naranja(obstaculos_t *obstaculo){
    return obstaculo->color == COLOR_NARANJA;
}

int contador_puntos(obstaculos_t *obstaculo, int i){
    if(i < 10)
        return puntuacion_color[obstaculo->color];
    if(i >= 10 && i < 15)
        return (puntuacion_color[obstaculo->color]*2); 
    if(i >= 15 && i < 19)
        return (puntuacion_color[obstaculo->color]*3);
    if(i >= 19 && i < 21)
        return (puntuacion_color[obstaculo->color]*5);
    else
        return (puntuacion_color[obstaculo->color]*10);
    
}

void obstaculo_dibujar(obstaculos_t *obstaculo, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, colores[obstaculo->color][0],colores[obstaculo->color][1],colores[obstaculo->color][2],0x00);

    if(obstaculo->tocado == true)
        SDL_SetRenderDrawColor(renderer, colores[COLOR_AMARILLO][0],colores[COLOR_AMARILLO][1],colores[COLOR_AMARILLO][2], 0x00);
    poligono_dibujar(obstaculo->poligono, renderer);
}

void obstaculo_mov_horizontal(obstaculos_t *obstaculo){

    if(obstaculo->parametros[1] < 0|| obstaculo->parametros[1] > obstaculo->parametros[0])
	    obstaculo->parametros[2] *= -1;
	
    poligono_trasladar(obstaculo->poligono, obstaculo->parametros[2] / JUEGO_FPS, 0);
	obstaculo->parametros[1] += obstaculo->parametros[2] / JUEGO_FPS;
}

void obstaculo_aplicar_mov(obstaculos_t *obstaculo){
	if(obstaculo->movimiento == MOV_CIRCULAR){
		poligono_rotar_centrado(obstaculo->poligono, obstaculo->parametros[0], obstaculo->parametros[1], obstaculo->parametros[2]);
	}
	else if(obstaculo->movimiento == MOV_HORIZONTAL){
		obstaculo_mov_horizontal(obstaculo);
	}
}
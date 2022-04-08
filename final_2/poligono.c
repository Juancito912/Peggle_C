#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "config.h"
#include "poligono.h"

struct poligono {
    float (*vertices)[2];
    size_t n;
};

poligono_t *poligono_crear(float vertices[][2], size_t n){
	poligono_t *pm = malloc(sizeof(poligono_t));
	if(pm == NULL)
		return NULL;
	pm->vertices = malloc((2*n)*sizeof(float));
	if(pm->vertices == NULL){
		free(pm);
		return NULL;
	}
	for(int i = 0; i<n; i++){
		pm->vertices[i][0] = vertices[i][0];
		pm->vertices[i][1] = vertices[i][1];
	}
	pm->n = n;

	return pm;
}

void poligono_destruir(poligono_t *poligono){
	free(poligono->vertices);
	free(poligono);
}

size_t poligono_cantidad_vertices(const poligono_t *poligono){
	return poligono->n;
}

bool poligono_obtener_vertice(const poligono_t *poligono, size_t pos, float *x, float *y){
	if(pos >= poligono->n)
		return false;

	*x = poligono->vertices[pos][0];
	*y = poligono->vertices[pos][1];

	return true;
}

poligono_t *poligono_clonar(const poligono_t *poligono){
	return poligono_crear(poligono->vertices, poligono->n);
}

bool poligono_agregar_vertice(poligono_t *poligono, float x, float y){
	float (*aux)[2];
	aux = realloc(poligono->vertices,2*(poligono->n+1)*sizeof(float));
	if(aux == NULL)
		return false;

	poligono->vertices = aux;
	poligono->n +=1;
	poligono->vertices[poligono->n-1][0] = x;
	poligono->vertices[poligono->n-1][1] = y;

	return true;
}

static float prod_vec(float px, float py, float ax, float ay, float bx, float by){
		return ((bx-ax)*(py-ay))-((by-ay)*(px-ax));
}

bool punto_en_triangulo(float px, float py, float ax, float ay, float bx, float by, float cx, float cy){
	float abp = prod_vec(px,py,ax,ay,bx,by);//(B-A)X(P-A)
	float bcp = prod_vec(px,py,bx,by,cx,cy);//(C-B)x(P-B)
	float acp = prod_vec(px,py,cx,cy,ax,ay);//(A-C)X(P-C)

	return (abp>=0 && bcp>=0 && acp>=0) || (abp<=0 && bcp<=0 && acp<=0);
}
bool punto_en_poligono(float poligono[][2], size_t n, float px, float py){
	for (int i = 0; i <= n/2; i++){
		if (punto_en_triangulo(px,py,poligono[0][0],poligono[0][1],poligono[i+1][0],poligono[i+1][1],poligono[i+2][0],poligono[i+2][1]))
			return true;
	}
	return false;
	}

float producto_interno(float ax, float ay, float bx, float by){
    return ((ax*bx)+(ay*by));
}

float distancia (float px,float py,float ix,float iy){
    return sqrt(pow((px-ix),2)+pow((py-iy),2));
}

void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa = producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if(alfa <= 0) {
        *x = x0;
        *y = y0;
    }
    else if(alfa >= 1) {
        *x = x1;
        *y = y1;
    }
    else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}

double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y) {
    double d = 1 / 0.0;
    size_t idx = 0;

    for(size_t i = 0; i < p->n; i++) {
        float xi, yi;
        punto_mas_cercano(p->vertices[i][0], p->vertices[i][1], p->vertices[(i + 1) % p->n][0], p->vertices[(i + 1) % p->n][1], xp, yp, &xi, &yi);
        double di = distancia(xp, yp, xi, yi);

        if(di < d) {
            d = di;
            idx = i;
        }
    }

    float nx = p->vertices[(idx + 1) % p->n][1] - p->vertices[idx][1];
    float ny = p->vertices[idx][0] - p->vertices[(idx + 1) % p->n][0];
    float dn = distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}

poligono_t *poligono_crear_circular(float radio){
    float radianes = (2*PI)/NUM_CIRCULO_VERTICES; //Calculo el angulo entre 2 puntos
    poligono_t *circulo = malloc(sizeof(poligono_t));
        if(circulo == NULL)
            return NULL;

	circulo->vertices = malloc((NUM_CIRCULO_VERTICES*2)*sizeof(float));
		if(circulo->vertices == NULL){
			free(circulo);
			return NULL;
		}

    for(int i = 0; i<NUM_CIRCULO_VERTICES; i++){
            circulo->vertices[i][0] = radio*cos(radianes*(i+1));
            circulo->vertices[i][1] = radio*sin(radianes*(i+1));
    }
    circulo->n = NUM_CIRCULO_VERTICES;

    return circulo;
}

void poligono_dibujar(poligono_t *poligono, SDL_Renderer *renderer){
    for(int i = 0; i<(poligono->n)-1; i++){
        SDL_RenderDrawLine(renderer, poligono->vertices[i][0], poligono->vertices[i][1], poligono->vertices[i+1][0], poligono->vertices[i+1][1]);
    }
    SDL_RenderDrawLine(renderer, poligono->vertices[poligono->n-1][0],poligono->vertices[poligono->n-1][1], poligono->vertices[0][0], poligono->vertices[0][1]);
}

void poligono_trasladar(poligono_t *poligono, float dx, float dy){
	for (size_t i = 0; i < poligono->n; i++){
				poligono->vertices[i][0] += dx;
				poligono->vertices[i][1] += dy;
	}
}

void poligono_rotar(poligono_t *poligono, double rad){
	float coseno = cos(rad);
	float seno = sin(rad);
	for (int i = 0; i < poligono->n; i++){
		float rotar1 = (poligono->vertices[i][0]*coseno) - (poligono->vertices[i][1]*seno);
		float rotar2 = (poligono->vertices[i][0]*seno) + (poligono->vertices[i][1]*coseno);
				poligono->vertices[i][0] = rotar1;
				poligono->vertices[i][1] = rotar2;
	}
}

void poligono_rotar_centrado (poligono_t *poligono, float cx, float cy, float vel){
	poligono_trasladar(poligono,-cx,-cy);
	poligono_rotar(poligono, vel/JUEGO_FPS);
	poligono_trasladar(poligono,cx,cy);
}

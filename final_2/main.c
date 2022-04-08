#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "poligono.h"
#include "config.h"
#include "movimiento.h"
#include "lectura.h"
#include "obstaculos.h"
#include "lista.h"

#define DT (1.0 / JUEGO_FPS)

#ifdef TTF
#include <SDL2/SDL_ttf.h>

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255,255,255};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif

void destruir (void *a) {
        obstaculos_t *dato = a;
        obstaculo_destruir(dato);
}
int main(int argc, char *argv[]) {
    if(argc != 2){
        fprintf(stderr,"Uso %s [entrada.bin]\n",argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Peggle");
    long int puntaje_total = 0;
    size_t nivel = 1;
    while(1){
        
        int dormir = 0;

        // BEGIN código del alumno
        float canon_angulo = 0; // Ángulo del cañón
        bool cayendo = false;   // ¿Hay bola disparada?
    
        float cx, cy;   // Centro de la bola
        float vx, vy;   // Velocidad de la bola
        long int puntaje_nivel = 0;
        
        size_t naranjas_tocadas = 0;
        
        int16_t cant_obs;
        if(!leer_cant_obstaculos(f,&cant_obs)){
            fprintf(stderr, "No se pudo leer la cant_obstaculos\n");
            return 1;
        }

        lista_t *lista_obstaculos = lista_crear();

        for(int i = 0; i < cant_obs; i++){
            obstaculos_t *obstaculo = obstaculo_crear(f);
            if(lista_insertar_ultimo(lista_obstaculos, obstaculo) == false)
                return 1;
        }
        // END código del alumno
        
        unsigned int ticks = SDL_GetTicks();
        while(1) {
            if(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    break;

                // BEGIN código del alumno
                if(event.type == SDL_MOUSEBUTTONDOWN) {
                    if(! cayendo)
                        cayendo = true;
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                    if(canon_angulo > CANON_MAX)
                        canon_angulo = CANON_MAX;
                    if(canon_angulo < -CANON_MAX)
                        canon_angulo = -CANON_MAX;
                }
                // END código del alumno

                continue;
            }
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);


            // BEGIN código del alumno
        #ifdef TTF

            escribir_texto(renderer, font, "Nivel", 310, 20);
            char buff3[5];
            sprintf(buff3, "%ld", nivel);
            escribir_texto(renderer, font ,buff3, 375,20);

            escribir_texto(renderer, font, "Puntuacion:", MIN_X, 20);
            char buff[20];
            sprintf(buff, "%ld", puntaje_nivel);
            escribir_texto(renderer, font, buff, 225, 20);

            escribir_texto(renderer, font,"Puntacion Total:", 430, 20);
            char buff2[30];
            sprintf(buff2, "%ld",puntaje_total);
            escribir_texto(renderer, font, buff2,630,20);

        #endif

            if(cayendo) {
                // Si la bola está cayendo actualizamos su posición
                vy = computar_velocidad(vy, G, DT);
                vx *= ROZAMIENTO;
                vy *= ROZAMIENTO;
                cx = computar_posicion(cx, vx, DT);
                cy = computar_posicion(cy, vy, DT);
            }
            else {
                // Si la bola no se disparó establecemos condiciones iniciales
                cx = CANON_X + CANON_LARGO * sin(canon_angulo);
                cy = CANON_Y + CANON_LARGO * cos(canon_angulo);
                vx = BOLA_VI * sin(canon_angulo);
                vy = BOLA_VI * cos(canon_angulo);
            }

            // Rebote contra las paredes:
            if(cx < MIN_X + BOLA_RADIO || cx > MAX_X - BOLA_RADIO) vx = - vx;
            if(cy < MIN_Y + BOLA_RADIO) vy = -vy;

            // Rebote contra los obstaculos:
            lista_iter_t *iter_rebote = lista_iter_crear(lista_obstaculos);

            while(lista_iter_al_final(iter_rebote) != true){
                obstaculos_t *obstaculos = lista_iter_ver_actual(iter_rebote);
                rebote(obstaculos,&cx,&cy,&vx,&vy);
                lista_iter_avanzar(iter_rebote);
            }
            lista_iter_destruir(iter_rebote);

            // Se salió de la pantalla:
            if(cy > MAX_Y - BOLA_RADIO)
                cayendo = false;


            // Dibujamos el cañón:
            SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

            // Dibujamos la bola:
            poligono_t *bola = poligono_crear_circular(BOLA_RADIO);
            poligono_trasladar(bola,cx,cy);
            poligono_dibujar(bola,renderer);
            poligono_destruir(bola);

            // Dibujamos las paredes:
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
            SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
            SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
            SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
            SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

            // Dibujamos el vector de velocidad:
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
            SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);

            //Dibujamos los obstaculos:
            lista_iter_t *iter_lista_obstaculos = lista_iter_crear(lista_obstaculos);

            while(lista_iter_al_final(iter_lista_obstaculos) != true){

                obstaculos_t *obstaculos = lista_iter_ver_actual(iter_lista_obstaculos);
                obstaculo_dibujar(obstaculos, renderer);
                lista_iter_avanzar(iter_lista_obstaculos);
            }
            lista_iter_destruir(iter_lista_obstaculos);

            // Asignamos el movimiento a los obstaculos
            lista_iter_t *movimiento_obstaculos = lista_iter_crear(lista_obstaculos);
            while(lista_iter_al_final(movimiento_obstaculos) != true){
                obstaculos_t *obstaculos = lista_iter_ver_actual(movimiento_obstaculos);
                obstaculo_aplicar_mov(obstaculos);
                lista_iter_avanzar(movimiento_obstaculos);
            }
            lista_iter_destruir(movimiento_obstaculos);

            //Destruimos los obstaculos tocados
            //contabilizamos los puntos en el nivel actual
            //contabiliza los naranjas tocados
            if(!cayendo || ((vx <= 0.4 && vx >= -0.4) && (vy <= 1 && vy >= -1))){
                lista_iter_t *destruccion_puntaje_naranjas = lista_iter_crear(lista_obstaculos);

                while(lista_iter_al_final(destruccion_puntaje_naranjas) != true){
                    obstaculos_t *obstaculos = lista_iter_ver_actual(destruccion_puntaje_naranjas);
                    if(esta_marcado(obstaculos) == true){
                        if(es_naranja(obstaculos)){
                            naranjas_tocadas++;       
                        }
                        puntaje_nivel += contador_puntos(obstaculos, naranjas_tocadas);
                        obstaculo_destruir(obstaculos);
                        lista_iter_borrar(destruccion_puntaje_naranjas);
                    }
                    lista_iter_avanzar(destruccion_puntaje_naranjas);
                }
                lista_iter_destruir(destruccion_puntaje_naranjas);
            }
            

            // END código del alumno

            SDL_RenderPresent(renderer);
            ticks = SDL_GetTicks() - ticks;
            if(dormir) {
                SDL_Delay(dormir);
                dormir = 0;
            }
            else if(ticks < 1000 / JUEGO_FPS)
                SDL_Delay(1000 / JUEGO_FPS - ticks);
            ticks = SDL_GetTicks();
            if(naranjas_tocadas == 25)
            break;
        }

    // BEGIN código del alumno

    //Suma el puntaje obtenido en el nivel al puntaje total
    puntaje_total += puntaje_nivel;

    //Aumenta el nivel 
    nivel++;

    //Destruimos la lista de obstaculos creadas en cada nivel
    lista_destruir(lista_obstaculos,destruir);
    
    if (event.type == SDL_QUIT)
        break;
    }
    
    //cerramos el archivo
    fclose(f);
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}


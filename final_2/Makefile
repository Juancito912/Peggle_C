CC=gcc
CFLAGS=-pedantic -Wall -Werror -std=c99 -g
LFLAGS=-lm
SDL2FLAGS=-lSDL2 -lSDL2_ttf
TTFFLAGS=-DTTF

all: peggle

peggle: poligono.o movimiento.o obstaculos.o lista.o lectura.o main.o config.h
	$(CC) main.o poligono.o obstaculos.o movimiento.o lista.o lectura.o -o peggle $(SDL2FLAGS) $(LFLAGS)

main.o: poligono.o movimiento.o lectura.o config.h main.c 
	$(CC) $(CFLAGS) -c main.c $(TTFFLAGS) $(SDL2FLAGS)

poligono.o: poligono.h poligono.c config.h
	$(CC) $(CFLAGS) -c poligono.c

movimiento.o: movimiento.h movimiento.c poligono.h
	$(CC) $(CFLAGS) -c movimiento.c

lectura.o: lectura.h lectura.c config.h
	$(CC) $(CFLAGS) -c lectura.c

lista.o: lista.c lista.h 
	$(CC) $(CFLAGS) -c lista.c

obstaculos.o: obstaculos.c obstaculos.h config.h
	$(CC) $(CFLAGS) -c obstaculos.c

clean:
	rm *.o
	rm *peggle
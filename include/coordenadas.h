/* O par de coordenadas (X,Y) do mapa e os limites dele. */

#ifndef COORDENADAS_H
#define COORDENADAS_H

/* Limites do mapa. Com eles, a soma dos quadrados no calculo da distancia
   chega no maximo a 8 * COORD_MAX^2 = 8 x 10^12: nao cabe em int, mas cabe em
   long long. */
#define COORD_MAX 1000000
#define COORD_MIN (-COORD_MAX)

/* Localizacao do Pokemon, do Treinador e do Centro. */
typedef struct {
    int cordX;
    int cordY;
} cord;

#endif

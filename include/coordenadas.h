/* O par de coordenadas (X,Y) do mapa e os limites dele. */

#ifndef COORDENADAS_H
#define COORDENADAS_H

/* Limites do mapa. A leitura recusa coordenadas fora deste intervalo, e e isso
   que impede o calculo da distancia de estourar: a soma dos quadrados das
   diferencas chega a 8 * COORD_MAX^2, ou 8 x 10^12, que nao cabe em int (dai o
   long long) mas cabe com folga em long long. Sem o limite, coordenadas nos
   extremos de int dariam 3,2 x 10^19, que estoura ate o long long, e a
   distancia sairia negativa. A conta esta em descricoes/coordenadas.md. */
#define COORD_MAX 1000000
#define COORD_MIN (-COORD_MAX)

/* Usado pela localizacao do Pokemon, do Treinador e do Centro. Nao tem
   operacoes: e copiado por valor. */
typedef struct {
    int cordX;
    int cordY;
} cord;

#endif

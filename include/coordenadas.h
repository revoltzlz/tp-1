#ifndef COORDENADAS_H
#define COORDENADAS_H

/* Par de coordenadas inteiras (X,Y) do mapa da missao. Usado pela localizacao
   do Pokemon, do Treinador e do Centro de Pesquisa. Nao tem operacoes: e um
   agregado de dois inteiros, copiado por valor. */
typedef struct {
    int cordX;
    int cordY;
} cord;

#endif

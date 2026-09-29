#ifndef COORDENADAS_H
#define COORDENADAS_H

/* Limites do mapa da missao. Coordenadas fora deste intervalo sao recusadas
   na leitura.

   O limite nao e enfeite: ele e o que garante que o calculo da distancia nao
   estoure. A maior diferenca possivel entre duas coordenadas e 2 * COORD_MAX,
   e o que se calcula e a soma dos quadrados das duas diferencas, que vale no
   maximo 8 * COORD_MAX^2. Com COORD_MAX = 1000000 isso da 8 x 10^12:

     - nao cabe em int, cujo maior valor e cerca de 2,1 x 10^9, e por isso a
       conta e feita em long long;
     - cabe com folga enorme em long long, cujo maior valor e cerca de
       9,2 x 10^18.

   Sem esse limite, duas coordenadas nos extremos de int (cerca de 2 bilhoes)
   dariam uma soma de 3,2 x 10^19, que estoura ate o long long: a soma fica
   negativa, a raiz quadrada devolve nan e a comparacao escolhe o treinador
   errado. */
#define COORD_MAX 1000000
#define COORD_MIN (-COORD_MAX)

/* Par de coordenadas inteiras (X,Y) do mapa da missao. Usado pela localizacao
   do Pokemon, do Treinador e do Centro de Pesquisa. Nao tem operacoes: e um
   agregado de dois inteiros, copiado por valor. */
typedef struct {
    int cordX;
    int cordY;
} cord;

#endif

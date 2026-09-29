# `main.c` — o programa principal

O módulo do programa principal, separado dos TADs como a especificação exige.
Ele é curto de propósito: prepara o sorteio das Pokébolas e passa o controle
para o menu. Toda a lógica da missão está em `src/missao.c`, descrita em
[missao.md](missao.md).

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "missao.h"

int main(void)
{
    srand(time(NULL));

    missaoMenu();

    return 0;
}
```

## O que ele inclui, e por quê

| Cabeçalho | Para quê |
|---|---|
| `<stdlib.h>` | `srand` |
| `<time.h>` | `time` |
| `"missao.h"` | `missaoMenu` |
| `<stdio.h>` | vem por hábito de arquivo de programa principal; a única saída deste arquivo é o que o menu imprime |

O `missao.h` é o único cabeçalho do projeto incluído aqui. Ele já traz toda a
hierarquia de TADs abaixo dele, mas o `main.c` não usa nenhum deles
diretamente: ele não conhece `Pokemon`, `Pokelista`, `Treinador` nem
`PokeCenter` por dentro. É essa separação que a especificação pede quando diz
que "o módulo do programa principal deve estar separado dos módulos que compõem
os TADs".

## `srand`, uma única vez

```c
srand(time(NULL));
```

`srand` define a semente do gerador de números aleatórios, e `rand()` produz a
sequência a partir dela. `time(NULL)` devolve o número de segundos desde 1970,
então a semente muda a cada execução e o sorteio da recarga sai diferente cada
vez que o programa roda.

**Por que a chamada está aqui e não dentro da recarga:** chamar
`srand(time(NULL))` de novo no mesmo segundo reinicia o gerador com a mesma
semente, e `rand()` volta a devolver o mesmo primeiro número. Se o `srand`
estivesse dentro de `pokecenterRecarregarPokebolas`, duas recargas no mesmo
segundo — que é o que acontece, porque o programa roda em milissegundos —
sorteariam a mesma quantidade de Pokébolas. Como a recarga é o único sorteio do
programa, a semente é definida uma vez só, antes de tudo.

`srand` recebe um `unsigned int` e `time` devolve um `time_t`. A conversão é
implícita e não gera aviso nem com `-Wall -Wextra -pedantic`, então não há cast.

## `return 0`

O `main` devolve 0, que é o código de "terminou bem" para o sistema
operacional. O programa não propaga o resultado da missão para o código de
saída: quando um arquivo de entrada é inválido, o menu imprime a mensagem de
erro e volta a perguntar, e quem decide encerrar é o usuário, pela opção 0.

## O que não está aqui

Duas coisas que estiveram em versões anteriores e foram removidas na revisão
final:

**`SetConsoleOutputCP(CP_UTF8)` e o `#include <windows.h>`.** As mensagens do
programa têm acento e os arquivos estão em UTF-8; o terminal do Windows não usa
UTF-8 por padrão, e sem nenhum ajuste aparece `DragÃ£o` no lugar de `Dragão`.
Aquela chamada avisava o terminal, mas exigia `<windows.h>` e um `#ifdef _WIN32`
para não quebrar no Linux — três coisas fora do que a disciplina cobre, para
resolver um problema que é do terminal e não do programa. A solução passou a ser
rodar `chcp 65001` antes de executar, o que está no [README.md](README.md).

**Os dois `setvbuf`.** Eles desligavam o buffer de `stdout` e de `stderr`,
porque as mensagens de erro saíam por `stderr` e apareciam fora de ordem quando
a saída era redirecionada para um arquivo. Com as mensagens de erro passando a
sair por `printf`, como o resto, o problema deixou de existir.

**`argc` e `argv`.** Houve uma versão que aceitava `./tp1 arquivo.txt` como
atalho para rodar sem passar pelo menu. A especificação pede os dois modos de
uso, e o menu já atende os dois; o atalho era conveniência para os testes. Os
testes passaram a alimentar o menu pela entrada padrão, que é como um usuário
de verdade o usaria.

## Fluxo completo do programa

```
main
 └── srand(time(NULL))
 └── missaoMenu                                    [src/missao.c]
      │
      ├── imprime a moldura e as três opções
      ├── lê a opção com scanf
      │
      ├── opção 1 ──► missaoExecutarPorArquivo
      │                └── fopen, testa NULL
      │                └── pulaMarcaUtf8
      │                └── executa(arquivo, 0)
      │                └── fclose
      │
      ├── opção 2 ──► missaoExecutarInterativo
      │                └── executa(stdin, 1)
      │
      └── opção 0 ──► "Até a próxima!" e encerra


executa(entrada, interativo)
 │
 ├── pokecenterInicializar            cria as duas listas do Centro
 ├── leTreinador  (x2)                nome, Pokébolas, id, posição (0,0)
 ├── leFugitivos                      n Pokémon, Id = ordem de leitura
 │                                    → pokecenterRegistrarFugitivo
 │   (se algo falhou: libera o que existe e volta ao menu)
 │
 ├── (modo interativo) imprime os fugitivos registrados
 │
 ├── executaMissao
 │    ├── moldura INÍCIO DA MISSÃO
 │    ├── treinadorImprimir (x2)
 │    ├── recarregaSeComecouSemPokebola (x2)
 │    │
 │    └── para cada Id de 1 a n:
 │         ├── pokecenterBuscarFugitivo      ainda está fugido?
 │         ├── resgataPokemon
 │         │    ├── as duas distâncias, com sqrt só na impressão
 │         │    ├── escolheTreinador          menor distância, empate → menor id
 │         │    ├── treinadorMovimentar
 │         │    ├── treinadorCapturar         gasta uma Pokébola
 │         │    └── pokecenterRemoverFugitivo atualiza a lista de fugas
 │         │
 │         ├── acabaram os fugitivos?  → sai do laço      [PRIMEIRO teste]
 │         └── ficou sem Pokébolas?    → retornaAoCentro  [SEGUNDO teste]
 │              ├── moldura SEM POKÉBOLAS
 │              ├── treinadorMovimentar até o Centro
 │              ├── pokecenterReceberPokemon
 │              └── pokecenterRecarregarPokebolas
 │
 │    └── encerraMissao
 │         ├── moldura Todos Pokemons foram resgatados
 │         ├── treinadorMovimentar (x2) até o Centro
 │         ├── pokecenterReceberPokemon (t1, depois t2)
 │         └── moldura MISSÃO CONCLUÍDA
 │
 ├── pokecenterGerarRelatorio          grava relatorio.txt
 │
 └── treinadorLiberar (x2) e pokecenterLiberar
      → as quatro células cabeça e tudo o que sobrou nas listas
```

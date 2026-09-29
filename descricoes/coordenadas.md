# `include/coordenadas.h`

Define o tipo `cord`, que é um par de coordenadas inteiras (X,Y) do mapa da
missão, e os limites desse mapa. É o arquivo mais baixo da hierarquia: não
inclui nenhum outro e não tem `.c`, porque só declara um tipo e duas
constantes, sem nenhuma operação.

Três coisas usam `cord`: a localização do Pokémon, a localização do Treinador e
a localização do Centro de Pesquisa.

## A struct

```c
typedef struct {
    int cordX;
    int cordY;
} cord;
```

| Campo | Tipo | O que é |
|---|---|---|
| `cordX` | `int` | coordenada no eixo horizontal |
| `cordY` | `int` | coordenada no eixo vertical |

Não tem operações. É um agregado de dois inteiros, copiado por valor: quando
uma função devolve um `cord`, quem recebe fica com uma cópia e pode mexer nela
sem alterar o original. É por isso que `pokemonGetLocalizacao`,
`treinadorGetLocalizacao` e `pokecenterGetLocalizacao` devolvem `cord` e não um
apontador.

## As constantes

```c
#define COORD_MAX 1000000
#define COORD_MIN (-COORD_MAX)
```

O mapa vai de −1.000.000 a 1.000.000 nos dois eixos. A leitura recusa qualquer
Pokémon fora disso, com mensagem de erro.

### Por que esse limite existe

**Não é enfeite: é o que garante que o cálculo da distância não estoure.** Esta
é a parte mais importante deste arquivo, e vale entender a conta.

A missão compara as distâncias dos dois treinadores até o alvo. A comparação
usa o **quadrado** da distância, `dx² + dy²`, porque é um número inteiro e
exato (ver [missao.md](missao.md)). A maior diferença possível entre duas
coordenadas é `2 × COORD_MAX`, então o maior valor que essa soma pode atingir é:

```
(2 × COORD_MAX)² + (2 × COORD_MAX)²  =  8 × COORD_MAX²
```

Com `COORD_MAX = 1.000.000`:

```
8 × (10⁶)²  =  8 × 10¹²
```

Comparando com o que cada tipo guarda:

| Tipo | Maior valor | 8 × 10¹² cabe? |
|---|---|---|
| `int` | ≈ 2,1 × 10⁹ | **não** |
| `long long` | ≈ 9,2 × 10¹⁸ | **sim**, com folga de seis ordens de grandeza |

É daí que vem a decisão de fazer a conta em `long long`: com o limite do mapa,
o resultado não cabe em `int`, mas sobra muito espaço num `long long`.

### O que aconteceria sem o limite

Um `int` vai até cerca de 2,1 bilhões. Se o programa aceitasse coordenadas até
esse valor, dois pontos em extremos opostos do mapa dariam:

```
(4 × 10⁹)² + (4 × 10⁹)²  =  3,2 × 10¹⁹
```

Isso **estoura até o `long long`**, cujo teto é 9,2 × 10¹⁸. Em C, estourar um
inteiro com sinal não dá erro nem aviso: a soma dá a volta e fica **negativa**.
A consequência em cadeia é:

1. `dx*dx + dy*dy` devolve um número negativo;
2. `sqrt` de um número negativo devolve `nan` ("not a number");
3. a comparação `distancia1 < distancia2` com um `nan` é sempre falsa;
4. a missão é atribuída ao treinador **mais distante**.

Esse bug existiu durante o desenvolvimento e foi encontrado na revisão final.
Naquele momento o programa imprimia `Distância Treinador(a) Rosa: nan` e
mandava a Rosa, que estava a 5,66 × 10⁹ do alvo, no lugar do Nate, que estava a
2,83 × 10⁹. Está registrado em [revisao.md](revisao.md).

O teste `testes/erro_coordenada_fora_do_mapa.txt` é o que guarda essa
correção: ele tem coordenadas de 2 bilhões e o programa tem de **recusar** o
arquivo, não calcular nada. E o
`testes/coordenadas_no_limite.txt`, com coordenadas exatamente em
±1.000.000, é o que exercita o `long long` de verdade.

### Por que 1.000.000 e não outro número

Qualquer valor até cerca de 1,07 × 10⁹ seria seguro para o `long long`
(`8 × L² < 9,2 × 10¹⁸`). Um milhão foi escolhido por ser folgado, redondo e
muito maior do que qualquer mapa que a especificação sugere — os exemplos usam
coordenadas de um dígito ou dois.

### Por que `COORD_MIN` tem parênteses

```c
#define COORD_MIN (-COORD_MAX)
```

`#define` é substituição de texto. Sem os parênteses, uma expressão como
`a - COORD_MIN` viraria `a - -1000000`, que funciona, mas em outros contextos a
falta de parênteses em macro com sinal causa erro de precedência. Pôr os
parênteses é o hábito seguro.

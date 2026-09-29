# TAD PokeLista

Arquivos: `include/conexao.h`, `include/pokelista.h` e `src/pokelista.c`.

É a lista encadeada de Pokémon que a especificação obriga a usar. Este é o
coração do trabalho: **toda a memória dinâmica do projeto está neste TAD**, nos
dois únicos `malloc` e nos dois únicos `free` do código.

A célula fica num arquivo separado, `conexao.h`, porque é um tipo próprio: ela
é a unidade de encadeamento, e a lista é feita de células. Os dois são descritos
aqui porque só fazem sentido juntos.

---

## `include/conexao.h`

```c
typedef struct conec {
    Pokemon pokemon;
    struct conec *prox;
} conec;
```

| Campo | Tipo | O que é |
|---|---|---|
| `pokemon` | `Pokemon` | o Pokémon guardado **por valor**, não por apontador |
| `prox` | `struct conec *` | o endereço da célula seguinte, ou `NULL` no fim |

### Por que `struct conec` aparece duas vezes

O campo `prox` aponta para uma célula, ou seja, para o próprio tipo que está
sendo declarado. Dentro do `typedef`, o nome `conec` **ainda não existe** — ele
só passa a existir depois do `}`. Por isso a struct precisa de um nome próprio
(`struct conec`), que pode ser usado antes de o `typedef` terminar.

Esse foi um erro do primeiro rascunho: ele escrevia `conec *prox;`, e o
compilador não conseguia compilar o arquivo.

### Por que o Pokémon é guardado por valor

Durante a missão, um Pokémon passa de uma lista para outra várias vezes:

```
lista de fugitivos  →  lista do treinador  →  lista de recuperados
```

Guardando **por valor**, cada lista fica com a sua própria cópia. Transferir é
remover de uma (o que copia o Pokémon para fora, numa variável local) e inserir
na outra (o que copia de dentro dessa variável para a célula nova). Nenhuma
memória é compartilhada entre as duas listas.

Se a célula guardasse um apontador `Pokemon *`, duas listas apontariam para o
mesmo Pokémon. Liberar uma delas deixaria a outra com um **apontador
pendurado**, apontando para memória devolvida ao sistema, e liberar as duas
daria **`free` duplo**. Os dois são erros difíceis de achar, porque o programa
muitas vezes continua funcionando por acidente.

O custo dessa escolha é que cada cópia move a struct inteira — 68 bytes neste
compilador, medidos com `sizeof(Pokemon)`, dos quais 50 são os dois vetores de
caracteres. Para o tamanho deste problema isso é
irrelevante, e a segurança compensa.

---

## `include/pokelista.h`

```c
typedef struct {
    conec *primeiro;
    conec *ultimo;
    int tamanho;
} Pokelista;
```

| Campo | Tipo | O que é |
|---|---|---|
| `primeiro` | `conec *` | aponta para a **célula cabeça**, que não guarda Pokémon |
| `ultimo` | `conec *` | aponta para a última célula da lista |
| `tamanho` | `int` | quantos Pokémon a lista tem, sem contar a cabeça |

É a lista com célula cabeça do Ziviani (capítulo 2). Inclui `<stdio.h>` além do
`conexao.h`, porque uma das operações recebe um `FILE *`.

---

## Como a lista é, por dentro

```
Pokelista
 primeiro ──┐                                          ┌── ultimo
            ▼                                          ▼
        ┌────────┐    ┌────────┐    ┌──────────┐    ┌─────────┐
        │ (vazia)│    │  Axew  │    │Frogadier │    │ Turtwig │
        │  prox ─┼───►│  prox ─┼───►│  prox ───┼───►│  prox   │──► NULL
        └────────┘    └────────┘    └──────────┘    └─────────┘
            ▲
     CÉLULA CABEÇA: existe, ocupa memória, mas o campo
     pokemon dela nunca é lido nem escrito
```

O primeiro Pokémon de verdade está em `primeiro->prox`, não em `primeiro`.

**Lista vazia:**

```
 primeiro ──┐
            ▼
        ┌────────┐
        │ (vazia)│
        │  prox  │──► NULL
        └────────┘
            ▲
 ultimo ────┘        primeiro == ultimo  →  a lista está vazia
```

### Para que serve a célula cabeça

Para que o primeiro Pokémon da lista **também tenha um antecessor**.

Numa lista encadeada simples não se anda para trás. Para desligar uma célula é
preciso ter em mãos quem aponta para ela, e alterar o `prox` desse antecessor.
Sem a célula cabeça, o primeiro elemento não tem antecessor, e a remoção
precisaria de dois códigos diferentes: um para o primeiro (mexendo no campo
`primeiro` da lista) e outro para os demais. Com a cabeça, **existe um só
código**, e é isso que faz a função `pokelistaRemover` ter um único caminho.

De brinde, o teste de lista vazia fica trivial: `primeiro == ultimo`.

### Para que serve o apontador `ultimo`

Para inserir no fim em **O(1)**. Sem ele, cada inserção precisaria percorrer a
lista inteira até achar a última célula, o que seria O(n) — e a missão insere um
Pokémon por captura e outro por entrega, então isso pesaria.

O preço do `ultimo` é ter de mantê-lo correto em toda operação que mexe no fim
da lista. É exatamente onde mora o bug clássico desse tipo de lista, descrito
na remoção do último, mais abaixo.

---

## `src/pokelista.c`, função por função

### `pokelistaInicializar`

```c
int pokelistaInicializar(Pokelista *pl);
```

Aloca a célula cabeça e deixa a lista vazia. Devolve 1 em caso de sucesso e 0
se o `malloc` falhar. **Custo O(1).**

```c
pl->primeiro = (conec *) malloc(sizeof(conec));
if (pl->primeiro == NULL) {
    pl->ultimo = NULL;
    pl->tamanho = 0;
    return 0;
}
pl->primeiro->prox = NULL;
pl->ultimo = pl->primeiro;
pl->tamanho = 0;
```

Este é o **primeiro dos dois `malloc` do projeto**, e é testado contra `NULL`.
No caminho de falha os outros campos também são zerados, para a struct não
ficar com lixo.

Repare em `pl->ultimo = pl->primeiro`: é isso que estabelece a invariante da
lista vazia.

Quem chama: `pokecenterInicializar` (duas vezes, para as duas listas do Centro)
e `treinadorInicializar` (uma vez, para a lista do treinador). São quatro
células cabeça numa execução.

### `pokelistaInserir`

```c
int pokelistaInserir(Pokelista *pl, const Pokemon *p);
```

Insere uma cópia do Pokémon **no fim** da lista. Devolve 1 em caso de sucesso e
0 se o `malloc` falhar. **Custo O(1)**, por causa do `ultimo`.

```
ANTES                                        ultimo
                                               ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL


1) aloca a célula nova e copia o Pokémon para dentro dela
   (nova->pokemon = *p  copia a struct inteira)

                                    nova ──► [Umbreon] ──► NULL

2) ultimo->prox = nova

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► [Umbreon] ──► NULL
                                               ▲
                                            ultimo   (ainda no Turtwig)

3) ultimo = nova

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► [Umbreon] ──► NULL
                                                              ▲
                                                           ultimo
```

Este é o **segundo `malloc` do projeto**, também testado contra `NULL`.

Como a inserção é sempre no fim, a lista guarda os Pokémon **na ordem em que
foram inseridos**. É disso que dependem duas coisas: a lista de fugitivos fica
na ordem do arquivo de entrada, e a lista de recuperados fica na ordem em que
os Pokémon foram entregues.

Quem chama: `pokecenterRegistrarFugitivo`, `pokecenterReceberPokemon` e
`treinadorCapturar`.

### `pokelistaRemover`

```c
int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido);
```

Remove da lista o Pokémon de identificação `id`. Se `removido` não for `NULL`,
copia o Pokémon para lá **antes** de liberar a célula. Devolve 1 se encontrou e
removeu, 0 se não existe Pokémon com esse id. **Custo O(n)**, porque precisa
procurar.

O laço anda com um apontador para a célula **anterior** ao alvo, e começa na
célula cabeça:

```c
anterior = pl->primeiro;
while (anterior->prox != NULL && pokemonGetId(&anterior->prox->pokemon) != id) {
    anterior = anterior->prox;
}
if (anterior->prox == NULL) {
    return 0;              /* chegou ao fim sem achar */
}
alvo = anterior->prox;
```

#### Remoção do MEIO

```
ANTES — anterior parou na célula ANTERIOR ao alvo

           anterior          alvo
              ▼               ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL
                                                ▲
                                             ultimo

1) copia alvo->pokemon para *removido    (ANTES do free!)
2) anterior->prox = alvo->prox           (desliga o alvo da corrente)

              ┌──────────────────────────────┐
              │                              ▼
   [cabeça] ──► [ Axew ] ─┐  [Frogadier]  [Turtwig] ──► NULL
                          └─────►(solto)

3) alvo == ultimo?  NÃO  →  ultimo não muda
4) free(alvo)

   [cabeça] ──► [ Axew ] ──► [Turtwig] ──► NULL
                                ▲
                             ultimo
```

#### Remoção do PRIMEIRO

```
ANTES — anterior começa na cabeça, que é a anterior ao primeiro Pokémon

  anterior     alvo
     ▼          ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL

DEPOIS

   [cabeça] ──► [Frogadier] ──► NULL
```

**É exatamente o mesmo código da remoção do meio.** Nenhum `if` a mais, nenhum
caso especial. É para isso que a célula cabeça existe.

#### Remoção do ÚLTIMO — o caso perigoso

```
ANTES
                         anterior      alvo == ultimo
                            ▼             ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL
                                                ▲
                                             ultimo

1) copia o Pokémon para fora
2) anterior->prox = alvo->prox   (= NULL)

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL     [Turtwig] (solto)
                                                          ▲
                                                       ultimo

3) alvo == ultimo?  SIM  →  ultimo = anterior      ← A LINHA QUE IMPORTA

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL     [Turtwig] (solto)
                                 ▲
                              ultimo

4) free(alvo)

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL
                                 ▲
                              ultimo
```

No código:

```c
if (alvo == pl->ultimo) {
    pl->ultimo = anterior;
}
free(alvo);
```

**O que acontece se essa correção faltar:** `ultimo` continua apontando para a
célula do Turtwig, que acabou de ser liberada. A próxima inserção executa
`ultimo->prox = nova`, ou seja, **escreve em memória já devolvida ao sistema**.
Às vezes funciona, porque o bloco ainda não foi reaproveitado; às vezes o
programa quebra; e a lista fica com o fim perdido, porque a célula nova é
pendurada num bloco que não faz mais parte da lista. É o bug clássico da lista
com apontador para o fim, e é difícil de achar justamente porque não falha
sempre.

#### Remoção do ÚNICO elemento

É o caso do último e do primeiro ao mesmo tempo. `anterior` é a cabeça, `alvo`
é a única célula com Pokémon, e `alvo == ultimo`, então `ultimo` volta a
apontar para a cabeça — que é a invariante da lista vazia. Depois disso a lista
está pronta para receber novas inserções.

```
ANTES                          DEPOIS
 primeiro ─► [cabeça] ─► [Axew] ─► NULL      primeiro ─► [cabeça] ─► NULL
                            ▲                              ▲
 ultimo ────────────────────┘                ultimo ───────┘
```

Este caso foi testado diretamente na revisão final, e é o que a linha
"`ultimo` voltou para a cabeça? 1" confirma. Ver [testes.md](testes.md).

#### Por que a cópia vem antes do `free`

```c
if (removido != NULL) {
    *removido = alvo->pokemon;
}
...
free(alvo);
```

Depois do `free`, a memória não pertence mais ao programa e ler dela é erro. A
cópia acontece enquanto a célula ainda é válida. O `if` permite chamar a função
sem querer o Pokémon de volta, passando `NULL` — é o que
`pokecenterRemoverFugitivo` faz quando só precisa tirar o Pokémon da lista de
fugas.

Quem chama: `pokelistaRemoverPrimeiro` e `pokecenterRemoverFugitivo`.

### `pokelistaRemoverPrimeiro`

```c
int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido);
```

Remove o primeiro Pokémon da lista e o copia para `*removido`. Devolve 1 em
caso de sucesso e 0 se a lista estava vazia. **Custo O(1)**: o alvo é a
primeira célula, então o laço da remoção para na primeira comparação.

```c
if (pokelistaVazia(pl)) {
    return 0;
}
return pokelistaRemover(pl, pokemonGetId(&pl->primeiro->prox->pokemon), removido);
```

Ela pega o Id do primeiro Pokémon e reaproveita a remoção por Id. Assim existe
**um único algoritmo de remoção** no TAD, em vez de dois códigos parecidos que
poderiam divergir.

O `pl->primeiro->prox->pokemon` é o primeiro Pokémon de verdade: `primeiro` é a
cabeça, `primeiro->prox` é a primeira célula com conteúdo. A checagem de lista
vazia antes é obrigatória — sem ela, `primeiro->prox` seria `NULL` e o acesso
quebraria.

Quem chama: `treinadorRetirarPokemon`. É o que faz o treinador entregar os
Pokémon **na ordem em que os capturou**.

### `pokelistaBuscar`

```c
int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado);
```

Procura o Pokémon de identificação `id`. Se achar e `encontrado` não for
`NULL`, copia o Pokémon para lá. Devolve 1 se encontrou, 0 se não.
**Custo O(n).**

```c
atual = pl->primeiro->prox;        /* começa no primeiro Pokémon de verdade */
while (atual != NULL) {
    if (pokemonGetId(&atual->pokemon) == id) { ... return 1; }
    atual = atual->prox;
}
return 0;
```

O custo é O(n) porque numa lista encadeada a única forma de chegar a um
elemento é seguir os apontadores `prox`, um por um, desde o começo. Não existe
cálculo de endereço como num arranjo. No pior caso — quando o elemento não está
lá — todas as n células são visitadas.

Quem chama: `pokecenterBuscarFugitivo`, que é usada pelo laço da missão para
confirmar que um Pokémon ainda está fugido antes de montar o resgate.

### `pokelistaImprimir`

```c
void pokelistaImprimir(const Pokelista *pl);
```

Imprime todos os Pokémon, um por linha, na ordem em que estão encadeados. Se a
lista estiver vazia, imprime `(nenhum Pokémon na lista)`. **Custo O(n).**

Chama `pokemonImprimir` para cada célula: cada TAD cuida do seu formato.

Quem chama: `pokecenterImprimirFugitivos`.

### `pokelistaEscreverRelatorio`

```c
void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida);
```

Escreve no arquivo já aberto uma linha `<numeroPokedex> <nome>` para cada
Pokémon, na ordem em que estão encadeados. **Custo O(n).**

Ela recebe um `FILE *` já aberto e não abre nem fecha nada. A divisão é: o
Centro de Pesquisa abre o arquivo, escreve o cabeçalho, chama esta função e
fecha. **Quem sabe percorrer a lista é a lista.**

A alternativa seria expor uma função tipo "me dê o Pokémon da posição i" e o
Centro chamá-la em laço. Isso seria O(n²), porque cada acesso por posição numa
lista encadeada é O(n), e ainda exporia a ideia de posição, que a lista
encadeada não tem.

Quem chama: `pokecenterGerarRelatorio`.

### `pokelistaVazia` e `pokelistaGetTamanho`

```c
int pokelistaVazia(const Pokelista *pl);      /* return pl->primeiro == pl->ultimo; */
int pokelistaGetTamanho(const Pokelista *pl); /* return pl->tamanho; */
```

Ambas **O(1)**. A primeira usa a invariante da célula cabeça. A segunda devolve
o contador, que é mantido pela inserção e pela remoção — contar percorrendo
seria O(n).

Quem chama a primeira: `pokelistaRemoverPrimeiro`, `pokelistaImprimir` (para
decidir se imprime a mensagem de lista vazia) e `pokecenterTemFugitivos`. Quem
chama a segunda: `pokecenterGetQtdFugitivos`.

### `pokelistaLiberar`

```c
void pokelistaLiberar(Pokelista *pl);
```

Libera **todas** as células, inclusive a cabeça, e anula os apontadores.
**Custo O(n).**

```c
atual = pl->primeiro;              /* começa NA CABEÇA, não em primeiro->prox */
while (atual != NULL) {
    seguinte = atual->prox;        /* guarda o prox ANTES do free */
    free(atual);
    atual = seguinte;
}
pl->primeiro = NULL;
pl->ultimo = NULL;
pl->tamanho = 0;
```

Três detalhes, todos importantes:

1. **Começa em `primeiro`, não em `primeiro->prox`.** A célula cabeça foi
   alocada com `malloc` na inicialização, então também precisa de `free`.
   Começar em `primeiro->prox` deixaria uma célula vazando por lista — quatro
   por execução.
2. **Guarda `atual->prox` antes do `free`.** Depois de liberar a célula, ler o
   campo `prox` dela seria ler memória devolvida ao sistema. A variável
   `seguinte` existe só para isso.
3. **Anula os apontadores no fim.** Se a lista for usada por acidente depois de
   liberada, ela falha de imediato em vez de mexer em memória liberada. E torna
   uma segunda chamada de `pokelistaLiberar` inofensiva, porque o laço não
   executa nenhuma vez.

É um laço, e não recursão: cada célula é visitada uma vez, e um laço não gasta
pilha.

Quem chama: `treinadorLiberar`, `pokecenterLiberar` e tambem
`pokecenterInicializar`, no caminho de erro em que a primeira lista do Centro
foi criada e a segunda falhou.

---

## A conta da memória

Cada Pokémon de uma execução passa por três células ao longo da missão:

| Onde | Quando a célula é criada | Quando é liberada |
|---|---|---|
| lista de fugitivos do Centro | ao registrar a fuga | quando o treinador informa a captura |
| lista do treinador | na captura | na entrega ao Centro |
| lista de recuperados do Centro | na entrega | no `pokecenterLiberar`, no fim |

Somando as quatro células cabeça (as duas listas do Centro e a lista de cada um
dos dois treinadores), uma execução com **n** Pokémon faz exatamente:

```
4 + 3n  alocações, e o mesmo número de liberações
```

Isso foi medido, não estimado. Com o `teste2.txt`, de 20 Pokémon: 64 alocações
e 64 liberações, e 4 + 3×20 = 64. Com 500 Pokémon: 1504 e 1504, e
4 + 3×500 = 1504. O método está em [testes.md](testes.md).

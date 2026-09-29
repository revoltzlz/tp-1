# TAD Pokémon

Arquivos: `include/pokemon.h` e `src/pokemon.c`.

Representa cada um dos Pokémon envolvidos na missão. É o TAD mais baixo da
hierarquia: inclui só o `coordenadas.h`, para ter o tipo `cord` da localização.
É usado pela célula da lista (`conexao.h`), que guarda um `Pokemon` dentro de
si.

---

## `include/pokemon.h`

### A struct

```c
typedef struct {
    int identificacao;
    int numPokedex;
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    cord localizacao;
} Pokemon;
```

| Campo | Tipo | O que é |
|---|---|---|
| `identificacao` | `int` | o **Id único**, atribuído pelo programa na ordem de leitura: 1, 2, 3... |
| `numPokedex` | `int` | o número da espécie na Pokédex, que **pode repetir** |
| `nome` | `char[30]` | o nome, como "Axew" |
| `tipo` | `char[20]` | o tipo, como "Dragão" |
| `localizacao` | `cord` | onde ele está no mapa |

### As constantes

```c
#define TAM_NOME 30
#define TAM_TIPO 20
#define FMT_NOME "%29s"
#define FMT_TIPO "%19s"
```

Os dois primeiros são o tamanho dos vetores, contando o `'\0'`. Os dois últimos
são a largura máxima de leitura para o `fscanf`, sempre **um a menos** que o
tamanho do vetor, para o `'\0'` caber. Ficam declarados lado a lado de
propósito: se alguém mudar `TAM_NOME` sem mudar `FMT_NOME`, o erro fica
visível na mesma tela.

Por que 30 e não 12, que era o tamanho do primeiro rascunho: `Charmander` e
`Jigglypuff` têm 10 caracteres e já ocupariam 11 dos 12 bytes. E os tipos
acentuados gastam mais de um byte por letra em UTF-8 — `Elétrico` ocupa 9
bytes, não 8. A entrevista usa arquivos novos, então a folga é barata.

---

## `src/pokemon.c`

### Inicialização

```c
void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex,
                        const char *nome, const char *tipo, int cordX, int cordY);
```

Preenche os cinco atributos. **Custo O(1).**

Ela não copia os campos diretamente: chama os próprios `set`, um para cada
atributo. A vantagem é que existe um único lugar no programa que sabe como cada
campo é copiado — em particular, um único lugar que faz a cópia segura das
strings. E isso dá uso real aos `set` que a especificação exige, sem precisar
chamá-los de forma artificial em outro lugar.

Chamada por `leFugitivos`, em `src/missao.c`, uma vez por Pokémon lido.

### Os `set`

```c
void pokemonSetId(Pokemon *p, int identificacao);
void pokemonSetNumPokedex(Pokemon *p, int numPokedex);
void pokemonSetNome(Pokemon *p, const char *nome);
void pokemonSetTipo(Pokemon *p, const char *tipo);
void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY);
```

Todos **O(1)**. Todos chamados por `pokemonInicializar`.

Os dois que copiam string merecem atenção:

```c
void pokemonSetNome(Pokemon *p, const char *nome)
{
    strncpy(p->nome, nome, TAM_NOME - 1);
    p->nome[TAM_NOME - 1] = '\0';
}
```

São duas linhas e as duas importam:

- `strncpy` com `TAM_NOME - 1` copia no máximo 29 caracteres, deixando a última
  posição do vetor livre. É o que impede um nome comprido de escrever fora do
  vetor, que é o erro clássico do `strcpy`.
- a atribuição do `'\0'` fecha a string. O `strncpy` só escreve o `'\0'`
  sozinho se o texto de origem for mais curto que o limite; se for mais
  comprido, ele copia 29 caracteres e **não** termina a string. Sem essa linha,
  um nome de 30 caracteres ou mais deixaria o vetor sem terminador, e qualquer
  `printf("%s")` depois disso leria memória além do campo.

### Os `get`

```c
int pokemonGetId(const Pokemon *p);
int pokemonGetNumPokedex(const Pokemon *p);
const char *pokemonGetNome(const Pokemon *p);
const char *pokemonGetTipo(const Pokemon *p);
cord pokemonGetLocalizacao(const Pokemon *p);
```

Todos **O(1)**. O `const` no parâmetro avisa que a função só lê o Pokémon.

Os dois que devolvem string devolvem `const char *`, que é o endereço do vetor
que está dentro da struct. O `const` no retorno diz que quem recebe só pode
ler: para alterar o nome existe o `pokemonSetNome`. Devolver o endereço é mais
simples do que copiar para um vetor do chamador, e funciona porque o uso é
sempre imediato — passar para um `printf`, por exemplo.

`pokemonGetLocalizacao` devolve o `cord` **por valor**, ou seja, uma cópia.

Quem chama: `src/missao.c` usa `pokemonGetId`, `pokemonGetNome` e
`pokemonGetLocalizacao` no laço da missão; `src/pokelista.c` usa
`pokemonGetId` na busca e na remoção, e `pokemonGetNumPokedex` e
`pokemonGetNome` para escrever o relatório; `src/pokecenter.c` usa
`pokemonGetNome` na mensagem de erro do recebimento.

### Impressão

```c
void pokemonImprimir(const Pokemon *p);
```

Imprime os cinco atributos em uma linha. **Custo O(1).** Chamada por
`pokelistaImprimir`, uma vez por Pokémon da lista.

```
Id 1 | Pokédex 610 | Axew | Tipo: Dragão | Localização: (7,7)
```

Ela lê os atributos pelos próprios `get`, e não direto da struct. Duas razões:
fica coerente com o resto do TAD, e se algum dia a forma de guardar um campo
mudar, só o `get` precisa acompanhar.

O número da Pokédex sai com `%03d`, com pelo menos três dígitos. Os arquivos de
teste oficiais trazem `025`, `004`, `007` e `001`; lidos como `int`, `025` vira
o número 25, e o `%03d` o devolve escrito como estava na entrada. Para os
números do exemplo do PDF (610, 657, 387, 715, 197) o `%03d` e o `%d` dão o
mesmo resultado.

---

## Decisões de projeto

### O Id é a ordem de leitura, não o número da Pokédex

A especificação exige que o Id seja **único**, e o arquivo de entrada **não
traz Id**: traz só o número da Pokédex. Então o programa atribui o Id na ordem
em que lê, começando em 1.

Usar o número da Pokédex como Id seria o caminho óbvio, e está errado: **o
número da Pokédex não é único**. O `testes/oficiais/teste2.txt`, que a
professora disponibilizou, tem **quatro Pikachus, todos com o número 025**. Com
a Pokédex no lugar do Id, a busca acharia sempre o primeiro Pikachu e a remoção
tiraria o Pokémon errado da lista de fugitivos — o programa entraria em laço ou
terminaria com Pokémon não resgatados.

Com o Id por ordem de leitura, os quatro Pikachus têm Ids diferentes, são
capturados individualmente e aparecem quatro vezes no relatório. O teste
`testes/pokedex_repetida.txt` cobre exatamente isso.

### O tipo é um atributo que o primeiro rascunho não tinha

A especificação lista cinco atributos para o Pokémon, e o tipo é um deles. O
rascunho inicial tinha a constante do tamanho (com o nome trocado, `TAM_IPO`)
mas não tinha o campo na struct. Foi acrescentado.

### Por que `Pokemon` é guardado por valor na lista

Isso é decisão do TAD PokeLista, e está explicada em
[pokelista.md](pokelista.md), mas afeta este arquivo: é o que faz a atribuição
`nova->pokemon = *p` funcionar como cópia completa, inclusive dos dois vetores
de caracteres.

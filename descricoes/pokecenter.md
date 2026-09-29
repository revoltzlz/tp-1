# TAD Centro de Pesquisa

Arquivos: `include/pokecenter.h` e `src/pokecenter.c`.

Representa o Centro de Pesquisa Pokémon. Guarda as informações de todos os
Pokémon fugitivos, recebe os recuperados e recarrega as Pokébolas dos
treinadores. Inclui o `treinador.h`, porque duas das operações dele recebem um
treinador.

O nome dos arquivos é `pokecenter`, e não `centro`, porque foi assim que o
projeto começou e o nome se manteve. O struct chama `PokeCenter` e as funções
usam o prefixo `pokecenter`.

---

## `include/pokecenter.h`

### A struct

```c
typedef struct {
    Pokelista fugitivos;
    Pokelista recuperados;
    cord locPokeCenter;
} PokeCenter;
```

| Campo | Tipo | O que é |
|---|---|---|
| `fugitivos` | `Pokelista` | os Pokémon cuja fuga foi registrada e que ainda não voltaram |
| `recuperados` | `Pokelista` | os Pokémon que os treinadores já entregaram |
| `locPokeCenter` | `cord` | a posição do Centro, que é (0,0) |

As **duas** instâncias de PokeLista são exigência literal da especificação. As
duas são por valor, como o campo `lista` do treinador: o Centro é dono delas.

Ao longo de uma missão bem-sucedida, os n Pokémon migram inteiramente da
primeira lista para a segunda: a de fugitivos termina vazia e a de recuperados
termina com n.

### As constantes

```c
#define CENTRO_X 0
#define CENTRO_Y 0
#define MIN_RECARGA 1
#define MAX_RECARGA 20
```

As duas primeiras são a posição que a especificação fixa ("Considere que a
localização do Centro de Pesquisa está nas coordenadas (0,0)"). As duas últimas
são o intervalo da recarga, também literal: "uma quantidade aleatória de
Pokébolas, no intervalo de 1 a 20".

---

## `src/pokecenter.c`, função por função

### `pokecenterInicializar`

```c
int pokecenterInicializar(PokeCenter *cp);
```

Põe o Centro em (0,0) e cria as duas PokeLista vazias. Devolve 1 em caso de
sucesso e 0 se alguma das listas não puder ser criada. **Custo O(1).**

```c
cp->locPokeCenter.cordX = CENTRO_X;
cp->locPokeCenter.cordY = CENTRO_Y;

if (!pokelistaInicializar(&cp->fugitivos)) {
    return 0;
}
if (!pokelistaInicializar(&cp->recuperados)) {
    pokelistaLiberar(&cp->fugitivos);
    return 0;
}
return 1;
```

O detalhe que importa é o **caminho de erro do meio**: se a primeira lista foi
criada e a segunda falhou, a primeira é liberada antes de retornar. Sem essa
linha, a célula cabeça da lista de fugitivos ficaria alocada para sempre, e o
Centro voltaria pela metade — com uma lista válida e outra com apontadores de
lixo.

Quem chama: `executa`, em `src/missao.c`, uma vez por execução.

### `pokecenterRegistrarFugitivo`

```c
int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p);
```

Registra a fuga de um Pokémon, inserindo uma cópia dele **no fim** da lista de
fugitivos. Devolve 1 em caso de sucesso e 0 se a inserção falhar.
**Custo O(1).**

Inserir no fim é o que mantém a lista de fugitivos na mesma ordem do arquivo de
entrada. Isso não é detalhe: o laço da missão percorre os Ids de 1 a n, e os
Ids são atribuídos na ordem de leitura, então a ordem da lista e a ordem do
laço coincidem.

O primeiro rascunho tinha `InsercaoPokemonFugido(PokeCenter *cp)`, sem receber
o Pokémon a inserir. Foi corrigido.

Quem chama: `leFugitivos`, uma vez por Pokémon lido.

### `pokecenterRemoverFugitivo`

```c
int pokecenterRemoverFugitivo(PokeCenter *cp, int id, Pokemon *removido);
```

Remove da lista de fugitivos o Pokémon de identificação `id`. Devolve 1 se
encontrou e removeu, 0 caso contrário. **Custo O(n).**

É a "atualização da listagem de fugas" que a especificação pede: o treinador
informa a captura ao Centro imediatamente, e o Centro tira o Pokémon da lista.

Quem chama: `resgataPokemon`, logo depois de cada captura, passando `NULL` no
terceiro parâmetro — nesse momento o Pokémon já está na lista do treinador, e o
Centro não precisa de outra cópia.

### `pokecenterBuscarFugitivo`

```c
int pokecenterBuscarFugitivo(const PokeCenter *cp, int id, Pokemon *encontrado);
```

Procura na lista de fugitivos o Pokémon de identificação `id` e, se achar,
copia-o para `*encontrado`. Devolve 1 se encontrou, 0 se não. **Custo O(n).**

É a operação de busca por Id que a especificação pede no TAD PokeLista, usada
de verdade: o laço da missão chama esta função antes de cada resgate, para
confirmar que aquele Pokémon ainda está fugido.

Quem chama: `executaMissao`, uma vez por Id.

### `pokecenterImprimirFugitivos`

```c
void pokecenterImprimirFugitivos(const PokeCenter *cp);
```

Imprime os Pokémon que ainda não foram recuperados. **Custo O(n).**

É a "impressão dos pokémon que ainda não foram recuperados" da especificação.
Delega para `pokelistaImprimir`.

Quem chama, em dois momentos:

1. no **modo interativo**, depois da leitura, para o usuário conferir o que
   digitou antes de a missão começar;
2. em `encerraMissao`, **se sobrou algum Pokémon** na lista de fugas, para
   mostrar quem ficou para trás em vez de anunciar que todos foram resgatados.

O segundo caso não acontece no fluxo normal. Fica como rede de segurança: se um
dia uma captura falhar, a saída diz exatamente quem não voltou.

### `pokecenterTemFugitivos`, `pokecenterGetQtdFugitivos`, `pokecenterGetQtdRecuperados`

```c
int pokecenterTemFugitivos(const PokeCenter *cp);       /* !pokelistaVazia(&cp->fugitivos) */
int pokecenterGetQtdFugitivos(const PokeCenter *cp);    /* pokelistaGetTamanho(&cp->fugitivos) */
int pokecenterGetQtdRecuperados(const PokeCenter *cp);  /* pokelistaGetTamanho(&cp->recuperados) */
```

Todas **O(1)**.

`pokecenterTemFugitivos` é a função que decide o fim da missão: é ela que
responde "acabaram os fugitivos?" no primeiro dos dois testes que vêm depois de
cada captura — ver [missao.md](missao.md).

Quem chama: `executaMissao` e `encerraMissao` usam as duas primeiras;
`pokecenterGetQtdFugitivos` também aparece na linha "Pokémons fugitivos a serem
resgatados: 5".

### `pokecenterGetLocalizacao`

```c
cord pokecenterGetLocalizacao(const PokeCenter *cp);
```

Devolve uma cópia da posição do Centro. **Custo O(1).**

Quem chama: `retornaAoCentro` e `encerraMissao`, para saber para onde mover os
treinadores na volta, e `pokecenterRecarregarPokebolas`. Ter essa função, em vez
de usar `CENTRO_X` e `CENTRO_Y` direto no módulo da missão, mantém a posição do
Centro como assunto do Centro.

### `pokecenterReceberPokemon`

```c
int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t);
```

Recebe todos os Pokémon que o treinador está carregando e devolve quantos foram
recebidos. **Custo O(n).**

```c
while (treinadorRetirarPokemon(t, &entregue)) {
    if (!pokelistaInserir(&cp->recuperados, &entregue)) {
        printf("Erro: memoria insuficiente ao receber %s no Centro de Pesquisa.\n",
               pokemonGetNome(&entregue));
        return recebidos;
    }
    recebidos++;
}
```

É a operação "recebimento dos Pokémon recuperados" da especificação, que ela
define como "removê-los da sua PokeLista e adicioná-los à lista de Pokémon
recuperados do Centro de Pesquisa" — exatamente o que o laço faz.

**Como o Pokémon viaja:** ele sai copiado da célula do treinador para a
variável local `entregue`, a célula do treinador é liberada, e depois ele é
copiado de `entregue` para uma célula nova do Centro. Em nenhum instante as
duas listas apontam para a mesma memória. É a decisão de guardar o Pokémon por
valor funcionando na prática — ver [pokelista.md](pokelista.md).

**A ordem é preservada:** o treinador entrega sempre o primeiro da lista dele (o
mais antigo) e o Centro insere no fim da lista de recuperados. O resultado é
que a lista de recuperados fica na ordem de captura.

**O caso de falta de memória:** se a inserção falhar, o Pokémon já saiu da lista
do treinador e não entrou na do Centro — ele seria perdido em silêncio. A
função avisa e para a entrega. Sem memória, insistir não ajudaria. Esse caminho
só é alcançável se um `malloc` falhar.

Quem chama: `retornaAoCentro` (quando o treinador fica sem Pokébolas) e
`encerraMissao` (duas vezes, uma por treinador, no fim da missão).

### `pokecenterRecarregarPokebolas`

```c
int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t);
```

Entrega ao treinador uma quantidade aleatória de Pokébolas no intervalo
[1, 20] e devolve essa quantidade. Devolve 0, sem recarregar, se o treinador
não estiver na posição do Centro. **Custo O(1).**

```c
posicaoCentro = pokecenterGetLocalizacao(cp);
posicaoTreinador = treinadorGetLocalizacao(t);
if (posicaoTreinador.cordX != posicaoCentro.cordX ||
    posicaoTreinador.cordY != posicaoCentro.cordY) {
    return 0;
}

quantidade = MIN_RECARGA + rand() % (MAX_RECARGA - MIN_RECARGA + 1);
treinadorSetPokebolas(t, quantidade);
return quantidade;
```

**O sorteio.** `rand() % (MAX - MIN + 1)` dá um número de 0 até `MAX - MIN`, ou
seja, de 0 a 19. Somando `MIN`, o intervalo vira 1 a 20, fechado nas duas
pontas — que é o que a especificação pede. Escrever `MIN + rand() % MAX` daria
1 a 20 por coincidência aqui, mas estaria errado para qualquer outro par de
limites; a forma com `(MAX - MIN + 1)` é a que vale sempre.

**O `srand` não está aqui.** Ele é chamado uma única vez, no `main`. Se
estivesse dentro desta função, duas recargas no mesmo segundo receberiam a
mesma semente e sorteariam a mesma quantidade — ver [main.md](main.md).

**A checagem de posição.** As Pokébolas ficam no Centro, então só quem está no
Centro pode ser recarregado. No fluxo normal da missão o treinador sempre se
movimenta para cá antes de pedir a recarga, então essa checagem nunca dispara;
ela existe para que uma chamada fora de hora não crie Pokébolas do nada. É
também o que dá uso ao parâmetro `cp`: sem ela, a operação de recarga não
usaria o Centro para nada, e um TAD cuja operação não recebe a própria
instância fica estranho.

A recarga **atribui**, não soma, porque é o verbo que a especificação usa.

O primeiro rascunho não tinha essa operação, e a que existia estava no TAD
Treinador. Foi acrescentada aqui.

Quem chama: `recarregaNoCentro`, em `src/missao.c`.

### `pokecenterGerarRelatorio`

```c
int pokecenterGerarRelatorio(const PokeCenter *cp, const char *nomeArquivo);
```

Emite o relatório final em arquivo `.txt`, com os Pokémon recuperados. Devolve
1 em caso de sucesso e 0 se o arquivo não puder ser aberto para escrita.
**Custo O(n).**

```c
saida = fopen(nomeArquivo, "w");
if (saida == NULL) {
    return 0;
}
fprintf(saida, "Pokemon recuperados:\n");
pokelistaEscreverRelatorio(&cp->recuperados, saida);
fclose(saida);
return 1;
```

O `fopen` é testado contra `NULL` e o `fclose` é sempre chamado no caminho de
sucesso. A travessia da lista é delegada ao TAD PokeLista: o Centro abre o
arquivo, escreve o cabeçalho e fecha.

O conteúdo sai assim:

```
Pokemon recuperados:
610 Axew
657 Frogadier
387 Turtwig
197 Umbreon
715 Noivern
```

**Uma errata da especificação aparece aqui.** O texto dela pede "o ID e o Nome"
dos Pokémon recuperados, mas o exemplo mostra `610 Axew`, e 610 é o número da
Pokédex, não o Id — com o Id por ordem de leitura, o Axew tem Id 1. O programa
segue o **exemplo**, que é o artefato concreto de conferência.

A consequência disso precisa ser dita na entrevista: no `teste2.txt`, que tem
quatro Pikachus, o relatório sai com quatro linhas idênticas `025 Pikachu`,
indistinguíveis entre si — que é justamente o problema que o Id único resolve.
Imprimir o Id junto resolveria, mas afastaria o relatório do exemplo do PDF.
Está registrado em [requisitos.md](requisitos.md) e [revisao.md](revisao.md).

Quem chama: `executa`, uma vez, no fim da missão. O nome do arquivo vem da
constante `ARQ_RELATORIO`, que é `"relatorio.txt"`.

### `pokecenterLiberar`

```c
void pokecenterLiberar(PokeCenter *cp);
```

Libera as duas PokeLista do Centro. **Custo O(n).**

```c
pokelistaLiberar(&cp->fugitivos);
pokelistaLiberar(&cp->recuperados);
```

No fim de uma missão bem-sucedida, a lista de fugitivos está vazia (mas com a
célula cabeça alocada) e a de recuperados tem os n Pokémon. As duas chamadas
recuperam tudo.

Quem chama: `executa`, no caminho normal e em todos os caminhos de erro da
leitura.

# TAD Treinador

Arquivos: `include/treinador.h` e `src/treinador.c`.

Representa um dos dois treinadores do esquadrão de recuperação. Inclui o
`pokelista.h`, porque cada treinador **tem uma PokeLista** dentro de si, onde
ficam os Pokémon que ele capturou e ainda não entregou.

---

## `include/treinador.h`

### A struct

```c
typedef struct {
    int identificador;
    char nome[TAM_NOME_COACH];
    cord loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;
```

| Campo | Tipo | O que é |
|---|---|---|
| `identificador` | `int` | 1 ou 2, atribuído na ordem de leitura |
| `nome` | `char[30]` | o nome, como "Rosa" |
| `loccoach` | `cord` | onde o treinador está no mapa |
| `lista` | `Pokelista` | os Pokémon que ele está carregando |
| `qntdpokebolas` | `int` | quantas Pokébolas ele ainda tem |

São os cinco atributos que a especificação pede. O campo `lista` é uma
`Pokelista` **por valor**, e não um apontador: o treinador é dono da lista dele,
e a vida dela é a vida do treinador.

Para que serve o identificador: é o critério de desempate quando os dois
treinadores estão à mesma distância do alvo. A especificação manda a missão
para o de **menor** identificador, então o primeiro treinador do arquivo
precisa receber o menor número.

### As constantes

```c
#define TAM_NOME_COACH 30
#define FMT_NOME_COACH "%29s"
#define TREINADOR_X_INICIAL 0
#define TREINADOR_Y_INICIAL 0
```

As duas primeiras seguem o mesmo par de tamanho e largura de leitura do TAD
Pokémon. As duas últimas são a posição inicial que a especificação fixa: todo
treinador começa em (0,0), que é a posição do Centro de Pesquisa.

---

## `src/treinador.c`, função por função

### `treinadorInicializar`

```c
int treinadorInicializar(Treinador *t, int identificador, const char *nome,
                         int qntdpokebolas);
```

Prepara o treinador: identificador, nome, quantidade inicial de Pokébolas,
posição (0,0) e a PokeLista dele. Devolve 1 em caso de sucesso e 0 se a célula
cabeça da lista não puder ser alocada. **Custo O(1).**

```c
treinadorSetId(t, identificador);
treinadorSetNome(t, nome);
treinadorSetPokebolas(t, qntdpokebolas);
treinadorSetLocalizacao(t, TREINADOR_X_INICIAL, TREINADOR_Y_INICIAL);
return pokelistaInicializar(&t->lista);
```

Duas coisas importam aqui:

**Ela não recebe coordenadas.** A especificação diz que a inicialização define
"localização inicial (0,0)", então a posição é fixa e vem das constantes. O
primeiro rascunho recebia `cordX` e `cordY` como parâmetros; foi corrigido,
porque deixar isso configurável contradiz o enunciado.

**O retorno é o da inicialização da lista.** Se o `malloc` da célula cabeça
falhar, o treinador não está utilizável e a falha sobe para quem chamou. É por
isso que a função devolve `int` e não `void`: sem esse retorno, o programa
seguiria com um treinador cuja lista tem apontadores inválidos.

Ela chama os próprios `set`, pelo mesmo motivo do TAD Pokémon: um único lugar
que copia cada campo, e os `set` ganham uso real.

Quem chama: `leTreinador`, em `src/missao.c`, uma vez por treinador.

### Os `set`

```c
void treinadorSetId(Treinador *t, int identificador);
void treinadorSetNome(Treinador *t, const char *nome);
void treinadorSetLocalizacao(Treinador *t, int cordX, int cordY);
void treinadorSetPokebolas(Treinador *t, int qntdpokebolas);
```

Todos **O(1)**. O `treinadorSetNome` usa `strncpy` com `'\0'` explícito, igual
ao do Pokémon — a explicação das duas linhas está em [pokemon.md](pokemon.md).

Quem chama: `treinadorInicializar` usa os quatro; `treinadorMovimentar` usa o
`treinadorSetLocalizacao`; e `pokecenterRecarregarPokebolas` usa o
`treinadorSetPokebolas`. Nenhum é chamado de forma artificial só para "estar
usado".

O verbo do `treinadorSetPokebolas` é **atribuir**, não somar. Isso é de
propósito: a especificação diz que a recarga "deverá **atribuir** ao treinador
uma quantidade aleatória de Pokébolas", e não somar ao que ele já tinha. No
fluxo da missão isso nem se distingue, porque a recarga só acontece quando o
treinador está com zero.

### Os `get`

```c
int treinadorGetId(const Treinador *t);
const char *treinadorGetNome(const Treinador *t);
cord treinadorGetLocalizacao(const Treinador *t);
int treinadorGetPokebolas(const Treinador *t);
```

Todos **O(1)**.

A especificação não pede `get` para o Treinador — ela pede para o TAD Pokémon.
Estes existem por necessidade: o módulo da missão precisa ler o nome para
imprimir, a posição para calcular a distância, as Pokébolas para decidir se o
treinador volta ao Centro, e o identificador para desempatar.

Quem chama: `src/missao.c` usa os quatro, e `treinadorGetNome` é a função mais
chamada do projeto, porque quase toda linha da saída traz o nome de um
treinador. `pokecenterRecarregarPokebolas` usa o `treinadorGetLocalizacao`.

Não existe um `get` para a PokeLista do treinador. Ela é acessada pelas
operações de domínio (capturar e retirar), que é o que a especificação pede —
um `get` que devolvesse a lista inteira permitiria mexer nela por fora do TAD.

### `treinadorMovimentar`

```c
void treinadorMovimentar(Treinador *t, int cordX, int cordY);
```

Move o treinador para a coordenada indicada. **Custo O(1).**

```c
treinadorSetLocalizacao(t, cordX, cordY);
```

**Ela não imprime nada.** Isso é decisão de projeto: o TAD não decide o que
aparece no terminal, exceto na operação de impressão. Quem narra a missão é o
módulo da missão — e ele usa esta mesma operação em dois momentos com mensagens
diferentes:

| Momento | O que o módulo da missão imprime |
|---|---|
| ir até o Pokémon | `Treinador(a) Rosa se movimentou para (7,7).` |
| voltar ao Centro | `Treinador(a) Rosa retorna ao Centro de Pesquisa.` |

Se a movimentação imprimisse por conta própria, o segundo caso sairia com a
mensagem errada, ou apareceria uma linha a mais que o exemplo do PDF não tem.

O primeiro rascunho tinha a assinatura `MovimentacaoCoach(Treinador *t)`, sem
as coordenadas — não era possível dizer para onde mover. Foi corrigido.

Quem chama: `resgataPokemon`, `retornaAoCentro` e `encerraMissao`.

### `treinadorCapturar`

```c
int treinadorCapturar(Treinador *t, const Pokemon *p);
```

Captura o Pokémon: gasta uma Pokébola e guarda uma cópia do Pokémon na
PokeLista do treinador. Devolve 1 em caso de sucesso, 0 se não havia Pokébola
ou se a inserção na lista falhou. **Custo O(1).**

```c
if (t->qntdpokebolas <= 0) {
    return 0;
}
if (!pokelistaInserir(&t->lista, p)) {
    return 0;
}
t->qntdpokebolas--;
return 1;
```

A **ordem das três operações importa**. A Pokébola só é descontada depois de a
inserção dar certo. Se fosse o contrário, uma falha de memória deixaria o
treinador com uma Pokébola a menos e sem o Pokémon — o estado ficaria
inconsistente.

A checagem de `qntdpokebolas <= 0` fica **dentro do TAD**, e não no módulo da
missão, para que a quantidade de Pokébolas nunca possa ficar negativa, mesmo
que alguém chame a captura fora de hora. No fluxo normal ela nunca dispara,
porque a missão garante que o treinador escolhido tem Pokébola. É uma checagem
defensiva de propósito: uma operação de TAD não deve confiar em quem a chama.

Quem chama: `resgataPokemon`.

### `treinadorRetirarPokemon`

```c
int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado);
```

Retira o **primeiro** Pokémon da PokeLista do treinador e o copia para
`*retirado`. Devolve 1 em caso de sucesso e 0 se ele não está carregando nada.
**Custo O(1).**

```c
return pokelistaRemoverPrimeiro(&t->lista, retirado);
```

Retirar sempre o primeiro é o que faz a entrega sair **na ordem em que os
Pokémon foram capturados**: a lista insere no fim, então o primeiro da lista é
o mais antigo. Combinado com o Centro inserindo no fim da lista de recuperados,
é isso que reproduz a ordem `610, 657, 387, 197, 715` do relatório do exemplo
do PDF.

O primeiro rascunho tinha `RemoveListacoach(Treinador *t)`, que não devolvia o
Pokémon retirado — o Centro não teria o que receber. Foi corrigido.

Quem chama: `pokecenterReceberPokemon`, em laço, até a lista do treinador
esvaziar.

### `treinadorImprimir`

```c
void treinadorImprimir(const Treinador *t);
```

Imprime os dados do treinador em uma linha. **Custo O(1).**

```
Treinador(a) Rosa: posição (0,0) | Pokébolas: 2
```

Esse formato é exatamente o do exemplo da especificação, e é por isso que a
linha traz nome, posição e Pokébolas, e não o identificador nem a lista:
acrescentar campos afastaria a saída do exemplo. Os Pokémon que o treinador
carregou aparecem no relatório final, depois da entrega, e a PokeLista tem a
sua própria operação de impressão, usada pelo Centro para listar os fugitivos.

Ela lê os atributos pelos próprios `get`, como faz a impressão do Pokémon.

Quem chama: `executaMissao`, duas vezes, no começo da missão.

### `treinadorLiberar`

```c
void treinadorLiberar(Treinador *t);
```

Libera a PokeLista do treinador. **Custo O(n)**, onde n é a quantidade de
Pokémon que ele ainda estava carregando.

```c
pokelistaLiberar(&t->lista);
```

No fim de uma missão bem-sucedida a lista está vazia, porque o treinador
entregou tudo — mas a **célula cabeça** dela continua alocada, e é essa que
este `free` recupera. Nos caminhos de erro, em que a missão não chega ao fim, a
lista pode ter Pokémon dentro, e todas as células são liberadas.

Quem chama: `executa`, em `src/missao.c`, tanto no caminho normal como nos
caminhos de erro da leitura.

---

## Decisões de projeto

### Os dois treinadores são duas variáveis, não um vetor

Em `executa` existem `Treinador treinador1` e `Treinador treinador2`, e não
`Treinador treinadores[2]`.

A especificação obriga o uso de lista encadeada para as listas lineares e fixa
a dupla em dois treinadores. Um vetor de dois elementos não seria
tecnicamente errado, porque não é uma coleção de Pokémon — mas evitá-lo tira
qualquer dúvida na correção, e com dois elementos fixos o vetor não simplifica
nada: o código que compara os dois treinadores ficaria igual, só com `[0]` e
`[1]` no lugar dos nomes.

### A recarga é operação do Centro, não do Treinador

O primeiro rascunho tinha `RecargaPokebola(Treinador *t)` no TAD Treinador. A
especificação lista a recarga entre as operações do **Centro de Pesquisa**
("Recarga de Pokébolas de um treinador"), que é quem tem as Pokébolas. Foi
movida. O que ficou no Treinador é o `set`, que o Centro usa para entregar a
quantidade sorteada.

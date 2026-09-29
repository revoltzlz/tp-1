# Guia da entrevista — TP1 AEDS I (CCF211)

Este arquivo é para você estudar. **Não vai no `.zip`.**

---

## 1. Mapa dos arquivos

```
Makefile              comentado no topo: como compilar, executar e limpar
main.c                programa principal: srand, terminal UTF-8, chama o menu
include/              os 7 cabeçalhos (struct + protótipos comentados)
src/                  as 5 implementações
testes/               25 casos de teste + o script que roda tudo
testes/oficiais/      os 2 arquivos que a professora disponibilizou
```

A dependência entre os módulos sobe numa linha só, nunca volta:

```
coordenadas.h → pokemon.h → conexao.h → pokelista.h → treinador.h → pokecenter.h → missao.h
                                                                                      ↑
                                                                                   main.c
```

### O que cada função faz, em uma linha

**`coordenadas.h`** — só o tipo `cord` (X e Y inteiros). Não tem `.c`.

**`conexao.h`** — só o tipo `conec`, a célula da lista. Não tem `.c`.

**`pokemon.c`** (TAD Pokémon)

| Função | O que faz |
|---|---|
| `pokemonInicializar` | preenche os 5 atributos chamando os próprios `Set` |
| `pokemonSetId` / `GetId` | a identificação única |
| `pokemonSetNumPokedex` / `Get` | o número da espécie |
| `pokemonSetNome` / `GetNome` | `strncpy` com `'\0'` explícito / devolve o endereço, `const` |
| `pokemonSetTipo` / `GetTipo` | idem para o tipo |
| `pokemonSetLocalizacao` / `Get` | X e Y; o `Get` devolve uma cópia da struct |
| `pokemonImprimir` | imprime os 5 atributos, lendo pelos próprios `Get` |

**`pokelista.c`** (TAD PokeLista — **onde está toda a memória dinâmica do projeto**)

| Função | O que faz | Custo |
|---|---|---|
| `pokelistaInicializar` | aloca a célula cabeça; lista vazia | O(1) |
| `pokelistaInserir` | insere no fim, pelo apontador `ultimo` | O(1) |
| `pokelistaRemover` | remove por Id, andando com o apontador para o **anterior** | O(n) |
| `pokelistaRemoverPrimeiro` | pega o Id do primeiro e chama a de cima | O(1) |
| `pokelistaBuscar` | procura por Id e copia para fora | O(n) |
| `pokelistaImprimir` | percorre chamando `pokemonImprimir` | O(n) |
| `pokelistaEscreverRelatorio` | percorre escrevendo num `FILE *` já aberto | O(n) |
| `pokelistaVazia` | `primeiro == ultimo` | O(1) |
| `pokelistaGetTamanho` | devolve o contador | O(1) |
| `pokelistaLiberar` | libera todas as células **e a cabeça**, e anula os apontadores | O(n) |

**`treinador.c`** (TAD Treinador)

| Função | O que faz |
|---|---|
| `treinadorInicializar` | id, nome, Pokébolas, posição (0,0) e a PokeLista dele |
| `treinadorSet*` / `Get*` | os 4 atributos simples |
| `treinadorMovimentar` | muda a posição; **não imprime nada** |
| `treinadorCapturar` | insere na lista e só então gasta a Pokébola |
| `treinadorRetirarPokemon` | tira o primeiro da lista e devolve para quem chamou |
| `treinadorImprimir` | nome, posição e Pokébolas, no formato do enunciado |
| `treinadorLiberar` | libera a PokeLista dele |

**`pokecenter.c`** (TAD Centro de Pesquisa)

| Função | O que faz |
|---|---|
| `pokecenterInicializar` | posição (0,0) e as duas PokeLista |
| `pokecenterRegistrarFugitivo` | insere no fim dos fugitivos |
| `pokecenterRemoverFugitivo` | remove por Id, quando um treinador avisa uma captura |
| `pokecenterBuscarFugitivo` | procura por Id na lista de fugas |
| `pokecenterImprimirFugitivos` | imprime quem ainda não foi recuperado |
| `pokecenterTemFugitivos` / `GetQtd*` | consultas às duas listas |
| `pokecenterGetLocalizacao` | devolve (0,0) |
| `pokecenterReceberPokemon` | esvazia a lista do treinador para a de recuperados |
| `pokecenterRecarregarPokebolas` | sorteia de 1 a 20; recusa quem não está no Centro |
| `pokecenterGerarRelatorio` | abre o `.txt`, escreve o cabeçalho e delega a travessia |
| `pokecenterLiberar` | libera as duas listas |

**`missao.c`** (sistema de controle — não é um TAD, é o módulo que conduz a missão)

| Função | O que faz |
|---|---|
| `imprimeRepetido` | imprime um caractere N vezes (usada para `=`, `-` e espaços) |
| `imprimeSeparador` / `imprimeMoldura` | as linhas de destaque da saída |
| `distanciaQuadrado` | `dx*dx + dy*dy` em `long long` — **sem raiz quadrada** |
| `escolheTreinador` | o mais próximo; no empate, o de menor id |
| `removeFimDeLinha` | tira `\n` e `\r` do caminho digitado no menu |
| `leTreinador` / `leFugitivos` | leitura, iguais nos dois modos, recebendo `FILE *` |
| `imprimeMolduraSemPokebolas` / `recarregaNoCentro` | trechos comuns aos dois retornos |
| `retornaAoCentro` | volta, entrega tudo e recarrega |
| `recarregaSeComecouSemPokebola` | trata quem começou com 0 |
| `resgataPokemon` | o bloco de um resgate: distâncias, escolha, movimento, captura |
| `encerraMissao` | os dois voltam e entregam, na ordem do id |
| `executaMissao` | o laço principal |
| `executa` | lê, roda, gera o relatório e **libera tudo** |
| `missaoExecutarPorArquivo` / `Interativo` / `missaoMenu` | a interface pública |

---

## 2. A lista encadeada, em desenhos

### Como ela é

```
Pokelista
 primeiro ──┐                                           ┌── ultimo
            ▼                                           ▼
        ┌────────┐     ┌────────┐     ┌────────┐     ┌────────┐
        │ (vazia)│     │  Axew  │     │Frogadier│    │Turtwig │
        │  prox ─┼────►│  prox ─┼────►│  prox ─┼────►│  prox  │──► NULL
        └────────┘     └────────┘     └────────┘     └────────┘
         ↑
       CÉLULA CABEÇA: não guarda Pokémon nenhum
```

**Lista vazia:**

```
 primeiro ──┐
            ▼
        ┌────────┐
        │ (vazia)│
        │  prox  │──► NULL
        └────────┘
            ▲
 ultimo ────┘          primeiro == ultimo  →  a lista está vazia
```

### Inserção no fim (O(1))

```
ANTES                                        ultimo
                                               ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL


1) aloca a célula nova e copia o Pokémon para dentro dela

                                    nova ──► [Umbreon] ──► NULL

2) ultimo->prox = nova

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► [Umbreon] ──► NULL
                                               ▲
                                            ultimo   (ainda aponta para Turtwig)

3) ultimo = nova

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► [Umbreon] ──► NULL
                                                              ▲
                                                           ultimo
```

Sem o apontador `ultimo`, o passo 2 exigiria percorrer a lista toda: O(n).

### Remoção do MEIO

```
ANTES — anterior parou na célula ANTERIOR ao alvo

           anterior           alvo
              ▼                ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL
                                                ▲
                                             ultimo

1) copia alvo->pokemon para fora (o parâmetro `removido`)
2) anterior->prox = alvo->prox            (desliga o alvo)

              ┌──────────────────────────────┐
              │                              ▼
   [cabeça] ──► [ Axew ] ─┐   [Frogadier]  [Turtwig] ──► NULL
                          └──────►(solto)

3) alvo == ultimo?  NÃO  →  ultimo não muda
4) free(alvo)

   [cabeça] ──► [ Axew ] ──► [Turtwig] ──► NULL
                                ▲
                             ultimo
```

### Remoção do PRIMEIRO

```
ANTES — anterior começa na célula cabeça, que é a anterior ao primeiro Pokémon

  anterior      alvo
     ▼           ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL

DEPOIS

   [cabeça] ──► [Frogadier] ──► NULL
```

**É exatamente o mesmo código da remoção do meio.** É para isso que a célula
cabeça existe: sem ela, remover o primeiro seria um caso especial, porque o
primeiro Pokémon não teria antecessor e seria preciso mexer no campo `primeiro`
da lista.

### Remoção do ÚLTIMO — **o caso perigoso**

```
ANTES
                         anterior         alvo == ultimo
                            ▼                ▼
   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► [Turtwig] ──► NULL
                                                ▲
                                             ultimo

1) anterior->prox = alvo->prox   (= NULL)

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL      [Turtwig] (solto)
                                                          ▲
                                                       ultimo

2) alvo == ultimo?  SIM  →  ultimo = anterior     ← ESTA LINHA É A CORREÇÃO

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL      [Turtwig] (solto)
                                 ▲
                              ultimo

3) free(alvo)

   [cabeça] ──► [ Axew ] ──► [Frogadier] ──► NULL
                                 ▲
                              ultimo
```

**O que acontece se o passo 2 for esquecido:** `ultimo` continua apontando
para a célula do Turtwig, que acabou de ser liberada. A próxima inserção faz
`ultimo->prox = nova`, ou seja, **escreve em memória já devolvida ao sistema**.
Às vezes funciona (o bloco ainda não foi reaproveitado), às vezes o programa
quebra, e a lista fica com o fim perdido. É o bug clássico da lista com
apontador para o fim.

No código: `src/pokelista.c`, dentro de `pokelistaRemover`.
Teste que pega esse bug: remover o último e inserir em seguida.

---

## 3. As decisões de projeto, e por quê

| Decisão | Por quê |
|---|---|
| Lista com **célula cabeça** e apontador **`ultimo`** | Célula cabeça: remover o primeiro deixa de ser caso especial. `ultimo`: inserir no fim em O(1). |
| **Id do Pokémon = ordem de leitura** (1, 2, 3...) | O enunciado exige Id único, e a entrada não traz Id. O número da Pokédex **não serve**: o `teste2.txt` da professora tem **quatro Pikachus com Pokédex 025**. Com a Pokédex como Id, a busca acharia sempre o mesmo Pikachu e a remoção tiraria o errado. |
| **Identificador do treinador = ordem de leitura** (1 e 2) | A entrada não traz identificador, e o desempate precisa de um. |
| **Pokémon guardado por valor** na célula, não por ponteiro | Passar de uma lista para outra é *remover* (copia para fora) e *inserir* (copia para dentro). Nenhuma memória é compartilhada entre as listas: nunca há ponteiro pendurado nem `free` duplo. |
| **Distância comparada pelo quadrado**, em `long long` | `dx*dx + dy*dy` é inteiro e exato, então o empate é detectado com segurança — comparar `double` com `==` é arriscado. A raiz é crescente, então a ordem não muda. `long long` porque o quadrado de uma diferença grande não cabe em `int`. `sqrt` só na impressão. |
| **Ordem dos testes após a captura**: 1º "acabaram os fugitivos?", 2º "ficou sem Pokébolas?" | É o que o exemplo do PDF mostra: o último resgate (Umbreon) zera as Pokébolas da Rosa e **não** aparece recarga. Trocando a ordem, apareceria uma recarga a mais no fim. |
| **Entrega na ordem de captura**; o treinador tira sempre o primeiro, o Centro insere no fim | É o que reproduz a ordem `610, 657, 387, 197, 715` do relatório do PDF. |
| **Rosa (id 1) entrega antes de Nate (id 2)** no retorno final | Também vem do relatório: `387, 197` (Rosa) antes de `715` (Nate). |
| Ao retornar, o treinador **volta para (0,0)** | Confirmado pelo exemplo: depois da recarga, a distância da Rosa até o Turtwig (1,1) é 1.41, ou seja, ela está na origem. |
| **`srand(time(NULL))` uma única vez**, no `main` | Se estivesse dentro da recarga, duas recargas no mesmo segundo receberiam a mesma semente e sorteariam o mesmo número. |
| `Inicializar` chama os próprios `Set` | Os `Set` que o enunciado pede ganham uso real, sem duplicar o código de cópia e sem deixar função morta. |
| Quem começa com **0 Pokébolas recarrega antes** do primeiro resgate | Ninguém sai para capturar sem Pokébola, e os dois já começam no Centro. A captura mantém a checagem: devolve 0 se não havia Pokébola. |
| **Uma só função de leitura**, recebendo `FILE *` | `fopen` no modo arquivo, `stdin` no interativo. As mensagens que pedem cada dado só saem no interativo. |
| **Dois treinadores em duas variáveis**, não em vetor | O enunciado obriga lista encadeada nas coleções e fixa a dupla em dois. Sem vetor, não há dúvida na correção. |
| **Prefixo do TAD** em toda função (`pokemonGetId`, `treinadorGetId`) | C não tem sobrecarga: dois `getId` em arquivos diferentes dariam `multiple definition` no ligador. |
| Escrita do relatório **no TAD PokeLista**, recebendo um `FILE *` | Quem sabe percorrer a lista é a lista. Assim o Centro nunca toca em `primeiro` nem em `prox`, e o custo fica O(n) — um `getPorPosicao` chamado de fora seria O(n²). |
| Número da Pokédex impresso com **`%03d`** | Os arquivos da professora têm `025`, `004`, `007`, `001`. Lidos como `int`, `025` vira 25; o `%03d` devolve `025`, igual à entrada. Para os números do exemplo (610, 657...) dá no mesmo. |
| **Mensagens com acento, comentários sem** | As mensagens precisam bater com o enunciado, que usa acento. Os comentários ficam sem acento para o arquivo abrir igual em qualquer editor. |
| `treinadorMovimentar` **não imprime** | O TAD não decide o que aparece no terminal. A mesma operação serve para ir até o Pokémon e para voltar ao Centro, que no enunciado têm mensagens diferentes. |

---

## 4. Perguntas prováveis, com resposta curta

**Por que a célula cabeça existe?**
Para que o primeiro Pokémon da lista também tenha um antecessor. Assim a
remoção é um código só: eu ando com um apontador para a célula anterior ao
alvo e faço `anterior->prox = alvo->prox`. Sem a cabeça, remover o primeiro
seria um caso especial, porque eu teria que mexer no campo `primeiro` da
própria lista. Além disso, a lista vazia fica fácil de testar:
`primeiro == ultimo`.

**O que acontece com o `ultimo` numa remoção?**
Se o Pokémon removido era o último, `ultimo` passa a apontar para a célula
anterior. Sem isso, `ultimo` ficaria apontando para memória que acabou de ser
liberada, e a próxima inserção escreveria nessa memória. É o bug clássico —
às vezes funciona, às vezes quebra.

**Por que a busca custa O(n)?**
Porque numa lista encadeada a única forma de chegar a um elemento é seguir os
apontadores `prox`, um por um, desde o começo. Não existe cálculo de endereço
como num arranjo. No pior caso — o elemento não está lá — eu visito todas as
n células.

**Arranjo ou lista encadeada?**

| | Arranjo | Lista encadeada |
|---|---|---|
| Acesso à posição i | **O(1)** | O(n) |
| Inserir/remover no meio | O(n): desloca os outros | **O(1)**, dado o anterior |
| Busca por valor | O(n) | O(n) |
| Tamanho | fixo na declaração | **cresce sob demanda** |
| Memória extra | nenhuma | **um apontador por célula** |

Aqui a lista encadeada é obrigatória pelo enunciado, e faz sentido: a lista de
fugitivos perde um elemento a cada captura e a de recuperados ganha um, sem
que se saiba de antemão quantos Pokémon o arquivo traz.

**Por que Pokémon por valor e não por ponteiro?**
Porque um Pokémon sai de uma lista e entra em outra várias vezes: fugitivos →
lista do treinador → recuperados. Guardando por valor, cada lista tem a sua
cópia, e a transferência é copiar para fora e copiar para dentro. Se fosse por
ponteiro, duas listas apontariam para o mesmo Pokémon, e liberar uma delas
deixaria a outra com ponteiro pendurado — ou daria `free` duplo.

**Por que comparar a distância ao quadrado?**
Duas razões. A primeira é exatidão: `dx*dx + dy*dy` é inteiro, então o empate
que o enunciado pede é detectado com certeza; comparar dois `double` com `==`
depende de arredondamento. A segunda é que a raiz quadrada é crescente, então
comparar os quadrados dá a mesma resposta que comparar as distâncias. Eu só
chamo `sqrt` na hora de imprimir com `%.2f`.

**Por que a ordem daqueles dois testes importa?**
Depois de cada captura eu pergunto primeiro se acabaram os fugitivos e só
depois se o treinador ficou sem Pokébolas. No exemplo do enunciado, o último
Pokémon (Umbreon) zera as Pokébolas da Rosa e **não** aparece recarga nenhuma
— a missão vai direto para o retorno final. Se eu testasse as Pokébolas
primeiro, sairia uma recarga a mais.

**Por que `srand` só uma vez?**
`srand` define a semente do gerador. Chamar `srand(time(NULL))` de novo no
mesmo segundo reinicia o gerador com a mesma semente, e `rand()` devolve o
mesmo número. Se o `srand` estivesse dentro da recarga, duas recargas no mesmo
segundo dariam a mesma quantidade de Pokébolas. Por isso ele está no `main`,
antes de tudo.

**Para que servem os include guards?**
Para um `.h` não ser processado duas vezes na mesma compilação. `pokecenter.h`
inclui `treinador.h`, que inclui `pokelista.h`; se `missao.c` incluir os dois,
`pokelista.h` chegaria duas vezes e o `typedef struct` apareceria repetido —
erro de `conflicting types`. O `#ifndef POKELISTA_H` / `#define POKELISTA_H` /
`#endif` faz a segunda passagem ficar vazia.

**Como o Makefile funciona?**
Em duas etapas. Primeiro cada `.c` vira um `.o` separado, com
`-Wall -Wextra -std=c99 -Iinclude`; o `-Iinclude` diz ao gcc onde achar os
cabeçalhos. Depois os `.o` são ligados num executável, com `-lm` **depois** dos
objetos, porque o ligador resolve os símbolos da esquerda para a direita e é o
meu código que chama `sqrt`. As dependências de cabeçalho estão declaradas, então
mudar um `.h` recompila só os `.o` que o incluem. O `clean` escolhe entre `del`
e `rm` pela variável `OS`, que só existe no Windows.

**Como você garante que toda a memória é liberada?**
Todo o `malloc` do projeto está em `src/pokelista.c`, em dois lugares: a célula
cabeça, na inicialização, e a célula nova, na inserção. `pokelistaLiberar`
percorre desde a **célula cabeça** — por isso ela também é liberada — guardando
o `prox` antes de cada `free`, e no fim anula os apontadores. São quatro listas
no total: as duas do Centro e a de cada treinador. Medi com um contador de
`malloc`/`free`: no `teste2.txt`, 64 alocações e 64 liberações, que é 4 células
cabeça + 20 registros de fuga + 20 capturas + 20 entregas.

**Por que o `long long` na distância?**
Porque `dx*dx` com `dx` grande não cabe em `int`. Com coordenadas de 2 bilhões,
o quadrado passa de 4×10¹⁸, que precisa de 64 bits. Tem um teste para isso:
`testes/coordenadas_grandes.txt`.

**Por que a busca por Id tem uso, se a lista já está na ordem do arquivo?**
Porque a lista de fugitivos vai perdendo elementos. Meu laço percorre os Ids de
1 até n; a busca no Centro é o que confirma que aquele Pokémon ainda está
fugido antes de montar o resgate. É também o que garantiria a ordem correta se
alguma captura falhasse.

**Onde está a memória compartilhada entre as listas?**
Em nenhum lugar — e isso é de propósito. Cada célula tem a sua própria cópia
do Pokémon. Quando ele passa da lista do treinador para a do Centro, ele viaja
dentro de uma variável local (`Pokemon entregue`, em
`pokecenterReceberPokemon`): sai copiado da célula do treinador, que é
liberada, e entra copiado numa célula nova do Centro.

---

## 5. O que mudou em relação ao que eu fiz à mão

Use `git diff estado-inicial HEAD` para ver tudo. O resumo:

### Erros que impediam a compilação

| Onde | O que estava | Por que mudou |
|---|---|---|
| `conexao.h` | `conec *prox;` | `conec` ainda não existia nesse ponto. Em C o campo que aponta para o próprio tipo precisa da struct nomeada: `struct conec *prox;` |
| 6 dos 7 `.h` | sem include guard | Causava `conflicting types` — o gcc apontou 6 erros só no primeiro arquivo |
| `pokemon.h` | `#ifndef poTkemon_h` **sem o `#define`** | O guard não protegia nada |
| `Pokemon.c`, `treinador.c` | `strcpy` sem `<string.h>` | Faltava o include |

### Erro que travava em execução

| Onde | O que estava | Por que mudou |
|---|---|---|
| `Pokemon.c:23` | `printf("Numero na Pokedex: %s", p->Numpokedex)` | `%s` com um `int`: o `printf` tratava 610 como endereço de memória e o programa quebrava na primeira impressão |

### Segurança de memória

| Onde | O que estava | Por que mudou |
|---|---|---|
| `Pokemon.c`, `treinador.c` | `strcpy(p->Nome, nome)` | Sem limite: um nome maior que o vetor escreve fora dele. Virou `strncpy` com `'\0'` explícito na última posição |
| `pokelista.h` | struct só com `inicio` | Sem `ultimo`, a inserção no fim é O(n) e a remoção do último deixa o fim da lista corrompido |

### Assinaturas que não davam para implementar

| Onde | O que estava | Por que mudou |
|---|---|---|
| `pokelista.h` | as 5 funções recebiam `int tamanho` e devolviam `void` | Inserir precisa de um `Pokemon`; remover e buscar precisam de um **Id**; a busca precisa **devolver** o que achou |
| `treinador.h` | `MovimentacaoCoach(Treinador *t)` | Movimentar para onde? Passou a receber X e Y |
| `treinador.h` | `CapturadePokemon(Treinador *t)` | Capturar qual Pokémon? Passou a receber o `Pokemon` |
| `treinador.h` | `RemoveListacoach(Treinador *t)` | Não devolvia o Pokémon retirado, então o Centro não tinha o que receber |
| `pokecenter.h` | `InsercaoPokemonFugido(PokeCenter *cp)` | Inserir qual Pokémon? Passou a receber o `Pokemon` |
| `treinador.h` | `RecargaPokebola` estava no Treinador | O enunciado lista a recarga como operação **do Centro de Pesquisa** |

### Requisitos do enunciado que faltavam

- **campo `tipo` no Pokémon** — o `TAM_IPO` estava definido (com typo) mas não havia campo na struct
- **todos os get/set** — nenhum TAD tinha
- **operação de recarga de Pokébolas no Centro** — não existia
- `pokelista.c`, `pokecenter.c`, `missao.c`, `main.c` e o `Makefile` — não existiam

### Organização

- `tp/` e o `pokelista.c` da raiz viraram `include/` (os `.h`) e `src/` (os `.c`), com `git mv`, preservando o histórico de cada arquivo
- `tp/Pokemon.c` → `src/pokemon.c`: o **P maiúsculo** não casava com `pokemon.h` e quebraria a compilação no Linux, que é o ambiente provável de correção
- cada `.c` passou a incluir **só o seu `.h`** — antes todos incluíam os cinco, o que invertia a hierarquia de dependências (`Pokemon.c` incluindo `pokecenter.h`)
- `TAM_NOME` de 12 para 30 e `TAM_IPO` → `TAM_TIPO`: `Jigglypuff` e `Charmander` têm 10 caracteres e já ocupavam 11 dos 12 bytes
- `indentificacao` → `identificacao` (typo), e os campos padronizados em minúsculas

### O que eu mantive do seu rascunho

- `coordenadas.h` com a struct `cord` — não está no enunciado, mas é uma boa decisão de projeto para o atributo "Localização"
- `conexao.h` como arquivo separado para a célula
- `missao.h`/`missao.c` como módulo do sistema de controle, separado dos TADs e do `main` — isso atende exatamente o requisito de modularização
- os nomes `PokeCenter`, `Pokelista`, `qntdpokebolas`, `loccoach`, `locPokeCenter`, `TAM_NOME_COACH 30`
- indentação de 4 espaços e o estilo dos `printf`

---

## 6. Como compilar e rodar

### Windows — Git Bash (recomendado)

```bash
mingw32-make              # compila
./tp1.exe                 # abre o menu
./tp1.exe testes/oficiais/teste1.txt    # roda direto num arquivo
bash testes/rodar_testes.sh             # a bateria de testes
mingw32-make clean        # limpa
```

### Windows — PowerShell

```powershell
mingw32-make
.\tp1.exe
.\tp1.exe testes\oficiais\teste1.txt
mingw32-make clean
```

**No PowerShell não existe `<` para redirecionar entrada.** Para alimentar o
modo interativo por um arquivo:

```powershell
Get-Content testes\oficiais\teste1.txt | .\tp1.exe
```

Se os acentos saírem errados (`DragÃ£o`), o problema é o terminal, não o
programa: rode `chcp 65001` antes. O programa já chama
`SetConsoleOutputCP(CP_UTF8)`, protegido por `#ifdef _WIN32`.

### Linux / WSL

```bash
make
./tp1
./tp1 testes/oficiais/teste1.txt
valgrind --leak-check=full ./tp1 testes/oficiais/teste2.txt
make clean
```

### ⚠ Se o programa não rodar no seu Windows

O **Smart App Control** do Windows 11 está ligado nesta máquina, em modo de
imposição, e bloqueia executáveis sem assinatura digital — inclusive os que o
gcc acabou de gerar. O sintoma é `Permission denied` mesmo com o arquivo
existindo, ou o programa não imprimir nada.

Para confirmar, no PowerShell:

```powershell
Get-WinEvent -LogName Microsoft-Windows-CodeIntegrity/Operational -MaxEvents 5
```

Se aparecer "Smart App Control Block", as saídas são:

1. **Desligar o Smart App Control** — Configurações → Privacidade e segurança →
   Segurança do Windows → Controle de aplicativo e navegador. **É irreversível:
   uma vez desligado, só volta reinstalando o Windows.** É o que resolve de vez
   para a disciplina toda.
2. **Usar o WSL** — o Ubuntu já está instalado, mas a virtualização está
   desligada no BIOS. Precisa ativar Intel VT-x / AMD-V no firmware e rodar
   `wsl --install --no-distribution`. Reversível, e de quebra permite testar no
   Linux e usar o valgrind.
3. **Recompilar com outra flag de otimização** (`-O1`, `-O2`, `-O3`, `-Og`) até
   um binário passar. É o que o `testes/rodar_testes.sh` faz automaticamente.
   Funciona, mas é sorte — não conte com isso no dia da entrevista.

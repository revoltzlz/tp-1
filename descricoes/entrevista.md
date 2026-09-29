# Guia da entrevista

Perguntas prováveis com resposta curta, e o que mudou em relação ao rascunho
feito à mão. O que já está nas outras descrições não se repete aqui:

- o mapa dos arquivos e como compilar estão no [README.md](README.md);
- o que cada função faz, o custo e quem a chama estão na descrição do TAD
  correspondente;
- os desenhos das operações da lista estão em [pokelista.md](pokelista.md);
- a conta do exemplo está em [exemplo.md](exemplo.md);
- as erratas da especificação e a evidência de cada requisito estão em
  [requisitos.md](requisitos.md).

---

## Sobre a lista encadeada

**Por que a célula cabeça existe?**
Para que o primeiro Pokémon da lista também tenha um antecessor. Numa lista
encadeada simples não se anda para trás: para desligar uma célula é preciso ter
em mãos quem aponta para ela. Sem a cabeça, remover o primeiro elemento seria
um caso especial, com um código diferente que mexe no campo `primeiro` da
lista. Com a cabeça, existe **um só** código de remoção. De brinde, o teste de
lista vazia fica `primeiro == ultimo`.

**O que acontece com o `ultimo` numa remoção?**
Se o Pokémon removido era o último, `ultimo` passa a apontar para a célula
anterior. Sem essa linha, `ultimo` ficaria apontando para memória que acabou de
ser liberada, e a próxima inserção faria `ultimo->prox = nova`, escrevendo em
memória devolvida ao sistema. Às vezes funciona, porque o bloco ainda não foi
reaproveitado; às vezes quebra. É o bug clássico da lista com apontador para o
fim, e é difícil de achar justamente porque não falha sempre.

**E se a lista tiver só um elemento?**
É o caso do primeiro e do último ao mesmo tempo. O anterior é a própria cabeça,
e como o alvo é o `ultimo`, ele volta a apontar para a cabeça — que é exatamente
a condição de lista vazia. Depois disso a lista aceita novas inserções
normalmente. Testei esse caso separado, porque o fluxo da missão nunca o
exercita.

**Por que a busca custa O(n)?**
Porque numa lista encadeada a única forma de chegar a um elemento é seguir os
apontadores `prox`, um por um, desde o começo. Não existe cálculo de endereço
como num arranjo. No pior caso — o elemento não está lá — visito todas as n
células.

**Por que inserir custa O(1), se a busca custa O(n)?**
Porque a inserção é sempre no fim e a lista guarda um apontador `ultimo`. Sem
ele, cada inserção precisaria percorrer a lista até achar a última célula, e
seria O(n). O preço é ter de manter o `ultimo` correto em toda operação que mexe
no fim — que é de onde vem a pergunta anterior.

**Arranjo ou lista encadeada?**

| | Arranjo | Lista encadeada |
|---|---|---|
| acesso à posição i | **O(1)** | O(n) |
| inserir ou remover no meio | O(n): desloca os outros | **O(1)**, dado o anterior |
| busca por valor | O(n) | O(n) |
| tamanho | fixo na declaração | **cresce sob demanda** |
| memória extra | nenhuma | **um apontador por célula** |

Aqui a lista encadeada é obrigatória pela especificação, e faz sentido: a lista
de fugitivos perde um elemento a cada captura e a de recuperados ganha um, sem
que se saiba de antemão quantos Pokémon o arquivo vai trazer.

**Por que o Pokémon é guardado por valor e não por ponteiro?**
Porque um Pokémon passa por três listas ao longo da missão: fugitivos, lista do
treinador, recuperados. Guardando por valor, cada lista tem a sua cópia, e
transferir é copiar para fora e copiar para dentro. Se fosse por ponteiro, duas
listas apontariam para o mesmo Pokémon: liberar uma deixaria a outra com
apontador pendurado, e liberar as duas daria `free` duplo.

**Onde está a memória compartilhada entre as listas?**
Em nenhum lugar, e isso é de propósito. Quando o Pokémon passa da lista do
treinador para a do Centro, ele viaja dentro de uma variável local
(`Pokemon entregue`, em `pokecenterReceberPokemon`): sai copiado da célula do
treinador, que é liberada, e entra copiado numa célula nova do Centro.

**Como você garante que toda a memória é liberada?**
Todo o `malloc` do projeto está em `src/pokelista.c`, em dois lugares: a célula
cabeça, na inicialização, e a célula nova, na inserção. O `pokelistaLiberar`
percorre desde a **célula cabeça** — por isso ela também é liberada — guardando
o `prox` antes de cada `free`, e anula os apontadores no fim. São quatro listas
numa execução: as duas do Centro e a de cada treinador. Medi com um contador: no
`teste2.txt`, 64 alocações e 64 liberações, e a conta é 4 células cabeça mais 3
por Pokémon, ou seja 4 + 3×20 = 64. Com 500 Pokémon, 1504.

**Por que liberar a lista com um laço e não com recursão?**
Porque um laço resolve e não gasta pilha. Uma versão recursiva com muitos
elementos poderia estourar a pilha, e não ganharia nada em clareza.

---

## Sobre a missão

**Por que comparar a distância ao quadrado?**
Duas razões. A primeira é exatidão: `dx*dx + dy*dy` é inteiro, e dois inteiros
se comparam sem ambiguidade — a especificação manda detectar o empate e mandar
o de menor identificador, e com números de ponto flutuante dois valores que
deveriam ser iguais podem diferir na última casa. A segunda é que a raiz
quadrada é crescente, então comparar os quadrados dá a mesma resposta que
comparar as distâncias. Só chamo `sqrt` na hora de imprimir, com `%.2f`.

**E se você usasse `pow`?**
Seria pior. A `pow` trabalha em ponto flutuante mesmo com expoente inteiro,
então reintroduziria o arredondamento que eu estava justamente evitando. Para
elevar ao quadrado, `dx*dx` é exato e mais rápido.

**Por que `long long` na distância?**
Porque o resultado não cabe em `int`. As coordenadas vão até ±1.000.000, então a
maior diferença é 2.000.000, o quadrado dela é 4×10¹² e a soma dos dois
quadrados chega a 8×10¹². Um `int` guarda até cerca de 2,1×10⁹. O `long long`
guarda até 9,2×10¹⁸, então sobra folga.

**Por que as coordenadas são limitadas, então?**
Porque nem o `long long` basta se as coordenadas forem arbitrárias. Dois pontos
nos extremos de um `int` (±2 bilhões) dariam uma soma de 3,2×10¹⁹, acima do
limite do `long long`: a soma daria a volta e ficaria **negativa**, `sqrt` de
negativo devolve `nan`, e a comparação mandaria o treinador **mais distante**.
Esse bug existiu no meio do desenvolvimento e o limite `COORD_MAX` é a correção.
O teste `erro_coordenada_fora_do_mapa.txt` é o que o guarda.

**Por que a ordem daqueles dois testes importa?**
Depois de cada captura eu pergunto primeiro se acabaram os fugitivos e só depois
se o treinador ficou sem Pokébolas. No exemplo da especificação, o último
resgate é o do Umbreon e ele zera as Pokébolas da Rosa: o exemplo mostra
`Pokébolas restantes: 0` e em seguida o bloco de fim, **sem nenhuma recarga**.
Se eu testasse as Pokébolas primeiro, a Rosa voltaria ao Centro e receberia
Pokébolas que não vai usar, e sairia um bloco a mais.

**Por que o relatório sai naquela ordem esquisita?**
`610, 657, 387, 197, 715`. O 715 fica no fim porque foi o Nate que capturou o
Noivern, e no retorno final a Rosa entrega antes dele, por ter o identificador
menor. Os três primeiros saem nessa ordem porque a Rosa entregou 610 e 657 no
meio da missão, quando ficou sem Pokébolas, e 387 e 197 só no fim. A conta
completa está em [exemplo.md](exemplo.md).

**Por que `srand` só uma vez?**
`srand` define a semente do gerador. Chamá-lo de novo no mesmo segundo reinicia
o gerador com a mesma semente, e `rand()` devolve o mesmo número. Se estivesse
dentro da recarga, duas recargas no mesmo segundo — que é o que acontece, porque
o programa roda em milissegundos — dariam a mesma quantidade de Pokébolas. Por
isso ele está no `main`, antes de tudo.

**Por que `MIN + rand() % (MAX - MIN + 1)` e não `MIN + rand() % MAX`?**
Porque `rand() % (MAX - MIN + 1)` dá um número de 0 até `MAX - MIN`, e somar
`MIN` desloca o intervalo para [MIN, MAX], fechado nas duas pontas. Com
`MIN = 1` e `MAX = 20`, a segunda forma também daria 1 a 20 por coincidência,
mas estaria errada para qualquer outro par de limites.

**Por que a busca por Id tem uso, se a lista já está na ordem do arquivo?**
Porque a lista de fugitivos vai perdendo elementos. Meu laço percorre os Ids de
1 até n, e a busca no Centro é o que confirma que aquele Pokémon ainda está
fugido antes de montar o resgate. É também o que garantiria a ordem correta se
alguma captura falhasse.

**Por que o Id é a ordem de leitura e não o número da Pokédex?**
Porque a especificação exige que o Id seja único, e o número da Pokédex **não
é**. O `teste2.txt`, que a professora disponibilizou, tem quatro Pikachus, todos
com o número 025. Com a Pokédex no lugar do Id, a busca acharia sempre o
primeiro Pikachu e a remoção tiraria o Pokémon errado da lista de fugitivos.
Além disso, o arquivo de entrada não traz Id nenhum — só o número da Pokédex.

---

## Sobre a organização do código

**Para que servem os include guards?**
Para um `.h` não ser processado duas vezes na mesma compilação. O `pokecenter.h`
inclui `treinador.h`, que inclui `pokelista.h`; se um `.c` incluir dois deles, o
`pokelista.h` chegaria duas vezes e o `typedef struct` apareceria repetido, o
que dá erro de `conflicting types`. O `#ifndef POKELISTA_H` / `#define
POKELISTA_H` / `#endif` faz a segunda passagem ficar vazia.

**Por que cada função tem o nome do TAD na frente?**
Porque C não tem sobrecarga: dois `getId` em arquivos diferentes dariam
`multiple definition` no ligador. O prefixo (`pokemonGetId`, `treinadorGetId`)
resolve e ainda deixa claro, na chamada, de qual TAD é a operação.

**Por que cada `.c` inclui só o próprio `.h`?**
Porque o `.h` já traz os de baixo. Incluir mais do que isso permitiria que um
módulo de baixo passasse a depender de um de cima por acidente — o rascunho
tinha `pokemon.c` incluindo `pokecenter.h`, que é o contrário da hierarquia.

**Por que `missao.c` não é um TAD?**
Porque ele não representa uma entidade com dados próprios: ele conduz a missão
usando os quatro TADs. A especificação pede o "Sistema de Controle da Missão"
como um módulo separado dos TADs e do programa principal, e é isso que ele é.

**Como o Makefile funciona?**
Em duas etapas. Primeiro cada `.c` vira um `.o` separado, com `-Iinclude` para
o gcc achar os cabeçalhos. Depois os seis `.o` são ligados num executável, com
`-lm` **depois** dos objetos, porque o ligador resolve os símbolos da esquerda
para a direita e é o meu código que chama `sqrt`. Cada regra lista os
cabeçalhos de que o objeto depende, então mudar um `.h` recompila só o que
precisa. Mais detalhes em [makefile.md](makefile.md).

**Por que o `make clean` tem aquele `ifeq` aninhado?**
Porque o comando de apagar depende do shell, não só do sistema. No Windows o
make pode rodar sob o `cmd`, onde existe `del`, ou sob o `sh` do Git Bash, onde
existe `rm`. A variável `OS` diz se é Windows e a `MSYSTEM` diz se é o Git Bash.
Escolher errado é pior que falhar: o comando não existe, o `-` da regra faz o
make seguir em frente, e os arquivos ficam onde estavam.

**Tem trechos do seu código que nunca executam. Por quê?**
Dois, e são de propósito: a captura que falha por falta de Pokébola, e o aviso
de que sobrou Pokémon na lista de fugitivos. O fluxo da missão garante que
nenhum dos dois acontece. Eles ficam porque uma operação de TAD não deve
confiar em quem a chama: se alguém chamar `treinadorCapturar` fora de hora, a
quantidade de Pokébolas não pode ficar negativa. Para alcançá-los de verdade
seria preciso um `malloc` falhando.

**Por que a impressão do treinador não mostra o identificador nem a lista dele?**
Porque o formato dessa linha vem do exemplo da especificação:
`Treinador(a) Rosa: posição (0,0) | Pokébolas: 2`. Acrescentar campos afastaria
a saída do exemplo. Os Pokémon que ele carregou aparecem no relatório final, e o
TAD PokeLista tem a própria operação de impressão, usada pelo Centro para
listar os fugitivos.

**Por que os treinadores são duas variáveis e não um vetor de dois?**
Porque a especificação obriga lista encadeada nas coleções e fixa a dupla em
dois. Um vetor de dois não seria errado, mas também não simplificaria nada: o
código ficaria igual, só com `[0]` e `[1]` no lugar dos nomes. Sem vetor, não
há dúvida na correção.

---

## O que mudou em relação ao rascunho feito à mão

```bash
git diff estado-inicial HEAD
```

### Erros que impediam a compilação

| Onde | O que estava | Por que mudou |
|---|---|---|
| `conexao.h` | `conec *prox;` | o nome `conec` ainda não existe dentro do próprio `typedef`. Em C o campo que aponta para o próprio tipo precisa da struct nomeada: `struct conec *prox;` |
| 6 dos 7 `.h` | sem include guard | causava `conflicting types` — o gcc apontou 6 erros só no primeiro arquivo |
| `pokemon.h` | `#ifndef poTkemon_h` **sem o `#define`** | o guard não protegia nada |
| `Pokemon.c`, `treinador.c` | `strcpy` sem `#include <string.h>` | faltava o include |

### Erro que travava em execução

| Onde | O que estava | Por que mudou |
|---|---|---|
| `Pokemon.c` | `printf("Numero na Pokedex: %s", p->Numpokedex)` | `%s` com um `int`: o `printf` tratava 610 como endereço de memória e o programa quebrava na primeira impressão |

### Segurança de memória

| Onde | O que estava | Por que mudou |
|---|---|---|
| `Pokemon.c`, `treinador.c` | `strcpy(p->Nome, nome)` | sem limite: um nome maior que o vetor escreve fora dele. Virou `strncpy` com `'\0'` explícito |
| `pokelista.h` | struct só com `inicio` | sem `ultimo`, a inserção no fim é O(n) e a remoção do último deixa o fim da lista corrompido |

### Assinaturas que não davam para implementar

| Onde | O que estava | Por que mudou |
|---|---|---|
| `pokelista.h` | as 5 funções recebiam `int tamanho` e devolviam `void` | inserir precisa de um `Pokemon`; remover e buscar precisam de um **Id**; a busca precisa **devolver** o que achou |
| `treinador.h` | `MovimentacaoCoach(Treinador *t)` | movimentar para onde? Passou a receber X e Y |
| `treinador.h` | `CapturadePokemon(Treinador *t)` | capturar qual Pokémon? Passou a receber o `Pokemon` |
| `treinador.h` | `RemoveListacoach(Treinador *t)` | não devolvia o Pokémon retirado, então o Centro não tinha o que receber |
| `pokecenter.h` | `InsercaoPokemonFugido(PokeCenter *cp)` | inserir qual Pokémon? Passou a receber o `Pokemon` |
| `treinador.h` | `RecargaPokebola` estava no Treinador | a especificação lista a recarga como operação **do Centro de Pesquisa** |

### Requisitos da especificação que faltavam

- o campo **`tipo`** no Pokémon — a constante do tamanho existia, com o nome
  trocado (`TAM_IPO`), mas não havia campo na struct;
- **todos os get e set** — nenhum TAD tinha;
- a operação de **recarga de Pokébolas** no Centro — não existia;
- `pokelista.c`, `pokecenter.c`, `missao.c`, `main.c` e o `Makefile` — não
  existiam.

### Organização

- `tp/` e o `pokelista.c` da raiz viraram `include/` (os `.h`) e `src/` (os
  `.c`), com `git mv`, preservando o histórico de cada arquivo;
- `tp/Pokemon.c` → `src/pokemon.c`: o **P maiúsculo** não casava com
  `pokemon.h` e quebraria a compilação no Linux;
- cada `.c` passou a incluir **só o seu `.h`** — antes todos incluíam os cinco;
- `TAM_NOME` de 12 para 30 e `TAM_IPO` → `TAM_TIPO`: `Charmander` tem 10
  caracteres e já ocupava 11 dos 12 bytes;
- `indentificacao` → `identificacao`, e os campos padronizados em minúsculas.

### O que ficou do rascunho

- `coordenadas.h` com a struct `cord` — não está na especificação, mas é uma boa
  decisão de projeto para o atributo "Localização";
- `conexao.h` como arquivo separado para a célula;
- `missao.h`/`missao.c` como módulo do sistema de controle, separado dos TADs e
  do `main` — que é exatamente o que a especificação pede;
- os nomes `PokeCenter`, `Pokelista`, `qntdpokebolas`, `loccoach`,
  `locPokeCenter` e a constante `TAM_NOME_COACH`;
- a indentação de 4 espaços e o estilo dos `printf`.

### O que a revisão final mudou depois disso

Está em [revisao.md](revisao.md). Em resumo: nada de errado no comportamento do
programa, e várias simplificações — saíram `windows.h`, `SetConsoleOutputCP`,
`setvbuf`, `argc`/`argv`, os `#ifdef` de teste, `snprintf`, `fgets` com
`sscanf`, o `static` das funções auxiliares e a regra de padrão do Makefile.

# A revisão final

O que esta revisão verificou, encontrou e mudou. A marca `antes-da-revisao`, no
git, aponta para o estado em que o projeto estava quando ela começou:

```bash
git diff antes-da-revisao --stat        # o que mudou no total
git log antes-da-revisao..HEAD --oneline # um commit por correção
```

## Andamento

| Etapa | Situação |
|---|---|
| 0. Preparação: git, build do zero, releitura do PDF | feita |
| 1. Correção contra o PDF | feita |
| 2. Simplicidade | feita |
| 3. Pasta `descricoes/` | feita |
| 4. Enxugar os comentários | feita |
| 5. Revisão independente | feita: três revisores, os três aplicados |
| 6. Slides, zip e fechamento | feita: slides em `apresentacao/` |
| 7. Apresentação, Makefile do modelo e revisão geral | feita: ver a última seção |

---

## O que foi verificado

### O PDF foi relido inteiro

7 páginas, 4 imagens. As quatro imagens são pequenas (296×670 e 196×219,
duplicadas) e são as ilustrações do Professor Oak e do Poké Radar — **nenhuma
contém texto do enunciado**. O texto foi extraído de quatro formas diferentes
(`-layout`, padrão, `-raw` e `-table`) e comparado.

### A dúvida do 17.72 estava errada, e não havia bug

A anotação do `CLAUDE.md` dizia que o PDF mostrava 17.72 como distância do Nate
até o Umbreon. **Não mostra.** As dez distâncias que aparecem no PDF são
exatamente 9.90, 9.90, 3.00, 12.21, 1.41, 1.41, 15.56, 14.14, 7.21 e **22.67**,
conferidas nas quatro extrações. A sequência `17.72` não existe em nenhum lugar
do texto do PDF.

O traço à mão do exemplo bate 100% com o PDF: as dez distâncias, os cinco
treinadores escolhidos e a ordem do relatório. **O PDF não tem erro de conta e
não houve nada a corrigir por causa disso.** A conta completa, com a posição do
Nate passo a passo, está em [exemplo.md](exemplo.md).

### A leitura do arquivo de entrada foi confirmada por três caminhos

O bloco de entrada do PDF está em três colunas, e copiar e colar embaralha as
colunas (junta `7 7` em `77`). A extração `-raw`, que segue a ordem do conteúdo,
mostra o bloco alinhado; removendo os comentários explicativos, o resultado é
**idêntico** ao `testes/oficiais/teste1.txt`, o arquivo que a professora
disponibilizou. E as dez distâncias calculadas a partir dele reproduzem as do
PDF.

### As transcrições dos oráculos são fiéis

`testes/saida_exemplo_pdf.txt` e `testes/relatorio_exemplo_pdf.txt` foram
comparados com a região correspondente da extração do PDF: **idênticos**.

### A saída do programa é idêntica à do exemplo

Comparada com `diff`, numa cópia temporária com o sorteio da recarga trocado
pelo valor do exemplo. A única diferença é a errata do bloco final (linha em
branco a mais e caractere invisível no PDF), explicada em
[exemplo.md](exemplo.md). O relatório também é idêntico, a menos do recuo de um
espaço por linha.

### O TAD PokeLista foi testado caso a caso

Dez casos, incluindo os que o fluxo da missão não exercita: lista vazia, remover
o único elemento, remover Id inexistente, remover do meio, do primeiro e do
último, e inserir depois de remover o último. A tabela está em
[testes.md](testes.md). A invariante `ultimo->prox == NULL` foi conferida em
todos os estados.

### Verificações automatizadas

| O que | Resultado |
|---|---|
| build do zero com as opções do Makefile | zero avisos |
| o mesmo acrescentando `-pedantic` | zero avisos |
| o mesmo acrescentando mais 20 flags de análise | 17 avisos, os dois de baixo |
| `fflush(stdin)`, `gets`, `void main`, `system`, `conio.h`, `pow` | nenhum |
| variáveis globais | nenhuma |
| recursão | nenhuma |
| `malloc` sem teste de `NULL` | nenhum (são dois `malloc`, os dois testados) |
| retorno de `fscanf` ignorado | nenhum (são quatro, os quatro testados) |
| `fopen` sem teste, `fclose` sem par | nenhum |
| `strcpy` sem limite | nenhum (só `strncpy`, sempre com `'\0'` explícito) |
| largura do `%s` diferente do tamanho do vetor menos 1 | nenhuma |
| número literal solto no código | nenhum, fora dos casos listados em [requisitos.md](requisitos.md) |
| dependências do Makefile | tocando um `.h`, os seis objetos recompilam |
| `make clean` no Git Bash e no PowerShell | apaga em ambos |
| bateria de testes | 72 de 72 |
| `malloc` igual a `free` | em sete arquivos, incluindo os de erro |

### O custo das duas simplificações, medido

Com as opções do Makefile, e também acrescentando `-pedantic`, o projeto
compila com **zero avisos**. Ligando mais vinte flags de análise, aparecem 17,
todos consequência direta de duas simplificações desta revisão. Nenhum é erro,
mas vale saber que existem:

**16 avisos de `-Wmissing-prototypes` em `src/missao.c`.** Vêm da remoção do
`static` das funções auxiliares. Uma função sem `static` e sem protótipo num
`.h` tem ligação externa mas nenhuma declaração prévia, e essa flag cobra a
declaração. O `static` era tecnicamente melhor: ele diz "esta função só é usada
dentro deste arquivo", o que é verdade para as dezesseis. Foi removido porque a
instrução desta revisão lista `static` entre o que trocar por algo mais simples.

Consequência prática: se um dia outro arquivo do projeto definisse uma função
com um desses nomes — `imprimeMoldura`, por exemplo — o ligador acusaria
`multiple definition`. Hoje não há conflito, e as flags da disciplina não
incluem essa verificação.

**1 aviso de `-Wsign-conversion` em `main.c`**, na linha `srand(time(NULL))`.
`time` devolve `time_t` e `srand` recebe `unsigned int`; a conversão é
implícita e pode mudar o sinal. O cast `(unsigned int)` que existia antes
silenciava o aviso, e foi removido por ser mais uma coisa a explicar.
`srand(time(NULL))` é a forma que o material da disciplina mostra.

---

## O que foi corrigido

Nada de errado foi encontrado no **comportamento** do programa nesta revisão: a
saída, o relatório, as distâncias, a ordem das entregas e a memória já estavam
corretos. As mudanças foram de simplicidade, de documentação e de uma
imprecisão.

### Uma imprecisão: a mensagem do relatório

O programa imprimia, depois da moldura final:

```
Relatório gravado em relatorio.txt: 5 Pokémon.
```

Essas duas linhas não estão no exemplo da especificação. Não eram errata do
PDF: eram acréscimo nosso. Antes, o script de teste **cortava** a saída antes de
comparar, para o `diff` passar — o que escondia a diferença. Foram removidas. A
mensagem de erro, para quando o arquivo não pode ser gravado, continua.

Agora o `diff` contra o exemplo passa sem cortar nada além do menu, que o
exemplo naturalmente não mostra.

### Uma constante órfã

`TAM_TITULO`, em `missao.h`, deixou de ser usada quando o `snprintf` saiu. Foi
removida.

---

## O que foi simplificado

Cada item passou pelos três critérios: está no conteúdo das disciplinas, é
explicável em uma ou duas frases, e não existe forma mais simples e igualmente
correta.

### `main.c`

| Saiu | Por quê | O que ficou no lugar |
|---|---|---|
| `#include <windows.h>`, `SetConsoleOutputCP(CP_UTF8)` e `#ifdef _WIN32` | três coisas fora do conteúdo da disciplina para resolver um problema que é do terminal, não do programa | a instrução `chcp 65001` no [README.md](README.md) |
| `#ifdef SEMENTE_FIXA` | era um modo de teste dentro do programa entregue | o script de teste troca a linha do sorteio numa cópia temporária |
| dois `setvbuf` | existiam porque `stdout` e `stderr` têm buffers separados | as mensagens de erro passaram a sair por `printf` |
| `argc` e `argv` | a especificação pede menu, e o menu já atende os dois modos | os testes alimentam o menu pela entrada padrão |
| `EXIT_SUCCESS` e `EXIT_FAILURE` | — | `return 0` |
| o cast em `srand((unsigned int) time(NULL))` | não é necessário: não gera aviso nem com `-pedantic` | `srand(time(NULL))` |

O arquivo saiu de 56 linhas para 18.

### `src/missao.c`

| Saiu | Por quê | O que ficou no lugar |
|---|---|---|
| `static` nas 16 funções auxiliares | está na lista do que trocar | as funções sem `static` |
| `snprintf` e a constante `TAM_TITULO` | montar o título num vetor para depois imprimi-lo | a moldura do treinador sem Pokébolas imprime direto |
| `fgets` com `sscanf`, `strchr`, `strlen`, `size_t` e três funções auxiliares de leitura de linha | seis coisas para ler um número e um caminho | `scanf("%d")` e `scanf(FMT_CAMINHO)` |
| `fprintf(stderr, ...)` nas 13 mensagens de erro | duas streams com buffers separados | `printf` |
| o ternário da escolha do treinador | — | `if/else` |
| `#include <string.h>` | deixou de ser usado | — |
| a cadeia de `if` do menu | — | `switch`, que a especificação do trabalho permite |
| `fread` com `sizeof` no `pulaMarcaUtf8` | — | três `fgetc` |

A troca do `fgets`+`sscanf` por `scanf` resolveu de graça um problema que antes
exigia código: a quebra de linha que sobrava na entrada depois do modo
interativo, e que fazia o menu reclamar de uma opção inválida que ninguém
digitou. O `scanf` pula espaço em branco sozinho.

### `Makefile`

| Saiu | Por quê | O que ficou no lugar |
|---|---|---|
| a regra de padrão `%.o: src/%.c` com `$<` e `$@` | exige conhecer três construções a mais, e deixava as dependências dos cabeçalhos longe das regras | uma regra explícita por módulo, com os cabeçalhos listados nela |

---

## O que foi avaliado e **ficou**

Cada um destes foi considerado para simplificação e mantido, porque simplificar
quebraria a correção. É a aplicação da regra "simples, mas nunca errado".

### `long long` no cálculo da distância

A conta `dx*dx + dy*dy` com as coordenadas no limite do mapa chega a 8 × 10¹², e
um `int` guarda só até 2,1 × 10⁹. Fazer a conta em `int` daria resultado errado.
A própria instrução da revisão prevê esse caso. Ver
[coordenadas.md](coordenadas.md).

### `COORD_MAX`, o limite do mapa

É o que garante que a conta acima não estoure **nem no `long long`**. Sem o
limite, duas coordenadas nos extremos de `int` dariam 3,2 × 10¹⁹, a soma ficaria
negativa, `sqrt` devolveria `nan` e a missão iria para o treinador mais
distante. Esse bug existiu antes desta revisão e está descrito na seção
seguinte.

### `MAX_QUANTIDADE`, o teto das quantidades

O `%d` do `scanf`, diante de um número que não cabe num `int`, **não garante o
que grava** — o padrão da linguagem diz que o comportamento é indefinido. Neste
compilador, `99999999999999999999` entrou como 1.661.992.959, um valor positivo
que passaria pela checagem de "não pode ser negativo". Como não dá para confiar
no valor, o teto o recusa.

A primeira versão desta seção dizia que o `%d` "trunca". Está errado, e o
revisor independente pegou: 1.661.992.959 é específico deste compilador, e
outro poderia parar em `INT_MAX`. Dizer "trunca" numa entrevista seria um ponto
perdido de graça.

### `pulaMarcaUtf8`

O Bloco de Notas do Windows escreve três bytes invisíveis no começo dos arquivos
UTF-8 que salva. Sem pular, eles entram colados no nome do primeiro treinador.
Como a entrevista usa arquivos novos, possivelmente criados no Bloco de Notas, o
tratamento ficou. São oito linhas com `fgetc` e `rewind`, ambas de `stdio.h`.

### A checagem de posição na recarga

`pokecenterRecarregarPokebolas` recusa recarregar um treinador que não está no
Centro. No fluxo normal nunca dispara.

O motivo de ficar é **verificável**: é ela que usa o parâmetro `cp`. Compilando
uma versão sem a checagem, o gcc emite
`warning: unused parameter 'cp' [-Wunused-parameter]`, o que quebraria o
critério de zero avisos do projeto. A primeira versão desta seção dava um
argumento estético ("um TAD fica estranho"); o revisor independente apontou o
argumento concreto, que é este.

### A escolha entre `del` e `rm` no `clean`

**Esta ficou pela metade e a revisão independente corrigiu.** O `clean` tinha 13
linhas de `ifeq` aninhado escolhendo o comando por duas variáveis de ambiente.
O diagnóstico estava certo — olhar só `OS` fazia o `clean` não apagar nada em
silêncio no Git Bash — mas a conclusão, não: a correção para "escolheu o comando
errado" é **tirar** o condicional, não somar outro.

Agora são duas linhas que tentam os dois comandos, cada uma com `-` na frente.
Testado nos dois shells. Ver [makefile.md](makefile.md).

### Trechos que o fluxo normal nunca alcança

São quatro, e o motivo de cada um é diferente. A primeira versão desta seção
dava um argumento só, e errado — dizia que eram "um TAD não confiando em quem o
chama", mas isso descreve a checagem **dentro** de `treinadorCapturar`, que é
outra coisa. O revisor independente apontou a confusão. Os motivos certos:

| Onde | Por que nunca roda | Por que fica |
|---|---|---|
| `resgataPokemon`: a captura que falha | o fluxo garante que o escolhido tem Pokébola | é o **tratamento de falha do `malloc`**: `treinadorCapturar` devolve 0 também quando `pokelistaInserir` não consegue memória. Testar o retorno de `malloc` é exigência da disciplina |
| `resgataPokemon`: `if (escolhido == NULL) continue;` | idem | **não é defesa, é necessidade estrutural**: sem ele, a linha seguinte desreferenciaria `NULL` |
| `encerraMissao`: o aviso de Pokémon não recuperado | todos são resgatados | mesma categoria: só é alcançável se um `malloc` falhar |
| `pokecenterReceberPokemon`: o aviso de memória insuficiente | idem | idem |

Há ainda um quinto caso que merece nota, porque a resposta é diferente: o
`continue` de `if (!pokecenterBuscarFugitivo(cp, id, &alvo))`, em
`executaMissao`. O `continue` nunca executa — os ids 1..n foram todos
registrados e cada um é visitado uma vez. **Mas a chamada é obrigatória**,
porque é ela que preenche `alvo`. A resposta certa na entrevista é: "o
`continue` nunca roda, mas a busca sim, porque é ela que me dá o Pokémon".

E vale saber defender a escolha de percorrer os ids de 1 a n em vez de iterar a
lista de fugitivos enquanto se remove dela: **iterar uma lista encadeada
enquanto se remove dela é onde os bugs moram.**

### O relatório imprime o número da Pokédex, não o Id

O texto da especificação pede "o ID e o Nome", e o exemplo mostra `610 Axew`,
que é o número da Pokédex. O programa segue o exemplo. A consequência é que no
`teste2.txt` o relatório sai com quatro linhas idênticas `025 Pikachu`. Está na
lista de pendências, porque depende de confirmação com os monitores.

---

## A revisão independente

Três revisores sem o histórico desta sessão foram encarregados de conformidade
com o PDF, simplicidade e consistência da documentação. **O de simplicidade
entregou; os outros dois pararam por limite de uso da ferramenta e serão
refeitos.**

O revisor de simplicidade rodou primeiro um varredor bruto dos itens proibidos
sobre `main.c`, `include/` e `src/` — `static`, `inline`, `extern`, `goto`,
`enum`, `union`, ternário, `void *`, `size_t`, `unsigned`, `qsort`, `memcpy`,
`memset`, `assert`, `errno`, `perror`, `stdbool`, `strdup`, `strtok`, `strtol`,
`snprintf`, `fgets`, `sscanf`, `getline`, `#pragma`, `#ifdef`, `_WIN32` — e não
achou **nenhuma ocorrência**. Também confirmou que não há recursão, operação de
bits, aritmética de ponteiro nem `**`.

### O que foi aceito e corrigido

| Achado | O que foi feito |
|---|---|
| `pokecenterGetQtdRecuperados` era API morta | removida. Ela era chamada pela mensagem "Relatório gravado em ...: N Pokémon", que saiu por não estar no exemplo, e ficou órfã |
| o `ifeq` aninhado do `clean` | trocado por duas linhas de comando, sem condicional |
| o comentário do `MAX_QUANTIDADE` dizia "trunca" | corrigido para "não garante o que grava": é comportamento indefinido |
| `treinadoresProntos` e o `&&` de curto-circuito | trocados por três `if` explícitos. `NUM_TREINADORES` ficou sem uso e saiu |
| a variável `ok` com dois significados | a segunda virou `relatorioGravado` |
| a guarda de `pulaMarcaUtf8` estava invertida | reescrita na forma positiva |
| `for (;;)` | virou `while (1)` |
| cores ANSI no script de teste | removidas |
| o argumento da checagem de posição era estético | trocado pelo concreto: sem ela o `-Wextra` acusa `unused parameter` |
| o argumento dos trechos inalcançáveis não correspondia ao código citado | reescrito, com um motivo por trecho |

### O que foi analisado e recusado, com o motivo

**Baixar `COORD_MAX` de 10⁶ para 10⁴ e eliminar o `long long`.** Esta é a
proposta mais forte do revisor, e a conta dele está certa: com `COORD_MAX` de
10.000, a soma dos quadrados chega a 8×10⁸ e cabe num `int`, o que apagaria o
`long long`, dois casts e o parágrafo de aritmética de estouro.

Não foi adotada porque a instrução desta revisão é explícita no ponto: *"`dx*dx
+ dy*dy` cabe em `int` com coordenadas até cerca de ±16000. **Se o PDF não
limitar as coordenadas, faça essa conta em `long long`**"*. O PDF não limita as
coordenadas em lugar nenhum. O revisor não tinha essa instrução. Fica
registrado como alternativa válida: se em algum momento se preferir aceitar um
mapa menor em troca de `int`, a mudança é `COORD_MAX 10000`, trocar `long long`
por `int` em `distanciaQuadrado` e em `escolheTreinador`, e reescrever
`testes/coordenadas_no_limite.txt`.

**Simplificar `testes/rodar_testes.sh`.** O revisor tem razão que as 543 linhas
dele estão muito acima do nível: cores de terminal (removidas), macro com
parâmetro no cabeçalho de contagem de memória, `2>&1 > /dev/null`, substituição
de processo, `${par%%:*}`. Mas o script **é ferramenta de verificação, não
material de entrega**: ele não entra no `.zip`, e a versão de cinco linhas que
o revisor propõe perderia o `diff` contra o exemplo, as invariantes, a contagem
de memória e o desvio do Smart App Control — que é justamente o que esta
revisão precisava medir.

A recomendação dele para a entrevista é boa e vale seguir: **dizer antes de ser
perguntado** que o script é ferramenta de conferência, montada com ajuda, e que
o que foi escrito e se sabe explicar é o programa em C. O mesmo vale para o
`gerar_zip.sh`.

**Escrever `"%d %29s %19s %d %d"` direto, em vez de concatenar `FMT_NOME`.** O
revisor está certo que a macro não protege nada — o 29 é escrito à mão e não
acompanha o `TAM_NOME`. Mas a especificação exige que números fixos sejam
constantes, e escrever as larguras direto no formato as devolve ao código como
números soltos. Fica como está.

**Trocar `pokelistaRemoverPrimeiro` pela remoção direta do Ziviani.** As duas se
defendem. A versão atual reaproveita `pokelistaRemover` para que exista um único
algoritmo de remoção no TAD; a do livro duplica o código mas é literalmente a
figura da aula. Mantida a atual, com o comentário que explica a escolha.

### O revisor de conformidade

Conferiu o código, a saída e o relatório contra o PDF, sem o histórico desta
sessão. **Nenhum problema grave ou médio.** Verificou os quatro TADs atributo
por atributo e operação por operação, a lista encadeada obrigatória, cada
funcionalidade do sistema de controle da missão, recalculou as dez distâncias na
mão e comparou o relatório byte a byte com o exemplo.

Sobre a única divergência de saída, ele chegou por conta própria à mesma
conclusão: o bloco final é **erro do PDF**, porque o caractere invisível e a
linha em branco extra aparecem só naquele bloco e quebram o padrão que o próprio
PDF usa nos outros três. Reproduzir exigiria imprimir um caractere invisível de
propósito.

Dois achados menores, os dois aceitos: o literal `5` em `fscanf(...) != 5` não
estava na lista de exceções de números literais do `requisitos.md` (entrou), e
duas imprecisões numéricas na documentação (corrigidas, ver abaixo).

### O revisor de consistência

Conferiu as descrições contra o código: assinatura, comportamento, retorno,
custo e "quem chama" de cada função, todos os campos de todas as structs, todas
as constantes, os desenhos ASCII passo a passo contra a implementação, e todos
os números citados.

**Os desenhos, as structs e os custos passaram sem nenhuma correção.** Seguiu os
quatro desenhos de `pokelista.md` contra o código e confirmou que a ordem das
operações — copiar antes do `free`, desligar, checar `alvo == ultimo`, atualizar
`ultimo`, só então `free` — é exatamente a do código em todos os casos.

Quatro afirmações erradas, todas corrigidas:

| Onde | O que dizia | O que é |
|---|---|---|
| `requisitos.md` D20 | que existe uma função `removeFimDeLinha` e que o caminho do arquivo é lido com `fgets` | nenhuma das duas existe: saíram quando o menu passou a usar `scanf`. Era a pior das quatro, porque afirmava no presente |
| `requisitos.md` R39 | citava uma tabela no `GUIA_ENTREVISTA.md` | o arquivo virou `descricoes/entrevista.md` e não tem essa tabela |
| `pokelista.md` | `pokelistaLiberar` tem dois chamadores | tem **três**: faltava `pokecenterInicializar`, no caminho de erro |
| `pokelista.md` | `pokelistaVazia` tem dois chamadores | tem **três**: faltava `pokelistaImprimir` |

E seis números, cada um reconferido antes de ser aceito:

| Dizia | É |
|---|---|
| 31 constantes | **29**, sem contar os 7 include guards |
| 17 casos inválidos | **16** — meu `grep` contava a definição da função junto |
| 18 casos válidos | **19** |
| `static` em 19 funções auxiliares | **16**: `missao.c` define 19, três públicas |
| `main.c` de 56 para 20 linhas | **18** |
| `sizeof(Pokemon)` de "uns 60 bytes" | **68**, medidos |

`R55` também tinha um trecho **duplicado** dentro da própria célula, e a segunda
metade afirmava algo que deixou de valer quando a mensagem do relatório foi
removida.

### Terceira rodada, por conta própria

Depois de aplicar os três revisores, uma conferência por script comparou **todo
número citado nas descrições** com o valor medido no projeto. Achou as duas
últimas divergências da tabela acima (`static` em 19 e 18 casos válidos). Uma
quarta passagem não achou mais nada.

### Para a entrevista

O revisor destacou onde o trabalho está seguro, e vale saber conduzir a conversa
por aí: `main.c` inteiro (17 linhas), `conexao.h`, **`pokelista.c` inteiro** —
que é o Ziviani capítulo 2 bem feito, com a correção do `ultimo` na remoção do
último, que é o ponto que separa quem entendeu de quem copiou —, `pokemon.c`,
`treinador.c`, `pokecenter.c` exceto o sorteio, `escolheTreinador`, o `switch`
do menu, e o Makefile.

---

## Bugs que existiram antes desta revisão

Registrados aqui porque são o tipo de coisa que cai na entrevista, e o
histórico do git os preserva.

### O estouro no cálculo da distância

`distanciaQuadrado` estourava o `long long` com coordenadas grandes. A soma dos
quadrados ficava negativa, `sqrt` devolvia `nan`, e a comparação mandava a
missão para o treinador **mais distante**. O programa imprimia
`Distância Treinador(a) Rosa: nan` e escolhia a Rosa, a 5,66 × 10⁹ do alvo, no
lugar do Nate, a 2,83 × 10⁹.

Três agravantes: o comentário da função afirmava que o `long long` resolvia o
problema; o guia de estudo citava como prova justamente o arquivo de teste que
era o contra-exemplo; e o teste marcava o caso como **passou**, porque
verificava código de saída, lista de fugas e contagem do relatório, mas nunca as
distâncias.

Corrigido com o limite `COORD_MAX` e com a verificação de distância inválida na
bateria de testes.

### O `make clean` que não apagava nada

Descrito acima. Falha silenciosa por causa do `-` na regra.

### O script de teste que dava falso positivo

Procurava a palavra "erro" na saída, e a mensagem `Permission denied` do próprio
shell era confundida com a recusa do programa. Reportava 30 testes como "passou"
sem que nada tivesse rodado. Corrigido procurando `Erro:` exatamente como o
programa escreve, e com uma verificação que **interrompe** a bateria se o
binário não produzir a saída esperada.

### O Pokémon perdido em silêncio

`pokecenterReceberPokemon` não avisava se a inserção na lista de recuperados
falhasse por falta de memória: o Pokémon já tinha saído da lista do treinador e
não entrava na do Centro. Agora avisa e para a entrega.

---

## O ambiente

### O Smart App Control bloqueia os executáveis

O Smart App Control do Windows 11 está ligado em modo de imposição nesta
máquina e bloqueia executáveis sem assinatura digital, inclusive os que o gcc
acabou de gerar. A decisão depende do conteúdo do binário: alguns passam,
outros não. Confirmado no log de eventos do Windows
(`Microsoft-Windows-CodeIntegrity/Operational`, evento 3077, "Smart App Control
Block").

O `testes/rodar_testes.sh` contorna isso tentando até 14 combinações de opções
de otimização até um binário conseguir rodar. **Funciona, mas depende de sorte.**
As alternativas definitivas estão no [README.md](README.md).

### Não foi possível testar no Linux

A especificação não diz onde o trabalho será compilado — só exige o Makefile com
o comentário de compilação. O projeto foi compilado e testado no Windows, no Git
Bash e no PowerShell.

O Linux não pôde ser testado: o WSL está instalado (Ubuntu), mas não inicia
porque a virtualização está desligada no firmware da máquina. O Makefile usa
apenas `gcc` com opções padrão de C99, e a escolha do comando de limpeza trata o
Linux explicitamente, mas isso é análise, não teste. **Fica como pendência.**

---

## Resumo das mudanças

`git diff antes-da-revisao --stat` no fim da revisão:

```
 34 arquivos alterados, 4514 inserções, 1583 remoções
```

A maior parte das inserções é a pasta `descricoes/`, que não existia: 14
arquivos, cerca de 3.900 linhas. No código:

| Arquivo | Mudança |
|---|---|
| `main.c` | 56 linhas → 18 |
| `src/missao.c` | reescrito em boa parte: leitura do menu, liberação nos caminhos de erro, e os comentários |
| `include/*.h` | comentários enxutos, uma linha por protótipo |
| `Makefile` | regras explícitas no lugar da regra de padrão; `clean` sem condicional |
| `testes/rodar_testes.sh` | refeito: alimenta o menu pela entrada padrão e monta as cópias temporárias |
| `testes/conta_memoria.h` | removido: o script agora gera esse cabeçalho na cópia temporária |

**Nenhum bug de comportamento foi encontrado nesta revisão.** A saída, o
relatório, as distâncias, a ordem das entregas e a memória já estavam corretos
quando ela começou. O que mudou foi simplicidade, documentação, uma mensagem a
mais na saída e um punhado de afirmações erradas na documentação.

## A segunda rodada: apresentação, Makefile do modelo e revisão geral

Depois que os slides ficaram prontos, o projeto inteiro foi revisto de novo.
O que essa passada encontrou:

| # | O que estava errado | Onde | Correção |
|---|---|---|---|
| 1 | O `gerar_zip.sh` procurava os slides em `slides/slides.pdf`, caminho que nunca existiu. O pacote sairia **sem os slides**, com um aviso fácil de não ver | `gerar_zip.sh` | aponta para `apresentacao/slides.pdf`; o teste do pacote confirma que o PDF entra |
| 2 | O `README.md` da raiz tinha só o título `# tp-1` | `README.md` | passou a explicar o problema, como compilar, rodar, testar e gerar o zip |
| 3 | A lista de pendências tinha o item do `valgrind` **duplicado** e dois itens já resolvidos | `descricoes/requisitos.md` | reescrita; o R05 deixou de ser pendência |
| 4 | O `main.c` incluía `<stdio.h>` sem usar. A própria descrição dizia que ele vinha "por hábito" | `main.c` | removido: o arquivo não imprime nada |
| 5 | Duas afirmações erradas nos slides, achadas na conferência visual: a ordem dos campos do arquivo de entrada e a faixa da recarga, que é de 1 a 20 e não de 1 a 10 | `apresentacao/` | corrigidas antes de gerar o PDF |
| 6 | A descrição do Makefile falava de regras explícitas e argumentava **contra** a regra de padrão | `descricoes/makefile.md` | reescrita para o Makefile novo |
| 7 | O `CLAUDE.md` listava a regra de padrão do Makefile entre as coisas a não usar | `CLAUDE.md` | registra que ela voltou de propósito, por causa do modelo da disciplina |
| 8 | A tabela das seções de teste dizia "19 casos válidos" numa seção que conta 20 verificações | `descricoes/testes.md` | a tabela agora traz a contagem de cada seção e o total |

### O Makefile foi reescrito na forma do modelo da disciplina

O documento "Makefile" das instruções traz dois modelos. O segundo usa `CC`,
`CFLAGS`, `SRC_DIR`, `BIN`, `SRCS` e `OBJS`, os alvos `all`, `run` e `clean`, e
uma regra de padrão com `$<` e `$@`. O nosso tinha uma regra explícita por
módulo e não tinha o `run`.

Veio do modelo: a regra de padrão, o alvo `run`, as variáveis de diretório e o
`-g`, que o próprio documento chama de essencial para o `gdb` e o `valgrind`.
Ficou diferente o que tinha motivo: `-Wextra` e `-std=c99`, o `-lm` do `sqrt`,
o executável `tp1` e o `main.c` na raiz. A lista completa, com o porquê de
cada item, está em [makefile.md](makefile.md).

Note a reviravolta: a regra de padrão **tinha sido removida** na primeira
revisão, por ser sintaxe a mais para explicar. Ela voltou porque é a forma que
a disciplina pediu. As duas decisões estão certas para o que se sabia em cada
momento.

### O que foi conferido de novo

- **Compilação**: zero avisos com as opções do Makefile, com `-pedantic`, e
  com outras 20 opções de análise. O único aviso que aparece no conjunto
  ampliado é o `-Wsign-conversion` do `srand(time(NULL))`, mantido de
  propósito.
- **Recompilação incremental**: tocar um `.h` recompila os seis objetos; tocar
  um `.c` recompila só ele.
- **`make clean`**: testado no Git Bash e no PowerShell; nos dois não sobra
  nada.
- **`make run`**: executa.
- **Os 72 testes**: todos passando, antes e depois da troca do Makefile.
- **O `.zip`**: monta, o `apresentacao/slides.pdf` entra, compila do zero sem
  avisos e roda o exemplo, com o relatório idêntico ao esperado.
- **Os números da documentação**, um a um: 29 constantes, 7 include guards, 17
  linhas no `main.c`, nenhuma função `static`, 51 funções públicas nos
  cabeçalhos, `sizeof(Pokemon)` igual a 68, 19 casos válidos e 16 inválidos, 72
  verificações, 4 + 3n alocações. Todos conferem.

### O que foi olhado e ficou como está

**A mensagem de escape em `resgataPokemon`.** Se `treinadorCapturar` falhar, a
saída diz "está sem Pokébolas", mas a captura também pode falhar por `malloc`.
Nesse caso a mensagem estaria errada. O trecho é inalcançável no fluxo normal,
e separar os dois casos acrescentaria um `if` num caminho que nunca executa.
Fica anotado aqui em vez de no código.

**O relink a cada `make`.** No Windows o gcc grava `tp1.exe`, e o make procura
um arquivo chamado `tp1`, que nunca existe: a ligação refaz toda vez, mesmo sem
nada ter mudado. Custa uma fração de segundo. Corrigir exigiria voltar com um
condicional de sistema operacional no Makefile, que é justamente o que a
primeira revisão tirou.

## O que depende do Gabriel

1. **Nome e matrícula dos dois alunos**, em três lugares: o nome do `.zip`, que
   a especificação exige no formato `TP1Aluno1Matricula1Aluno2Matricula2`, a
   capa dos slides e a divisão de tarefas no slide 3. Nos slides, os dois
   lugares estão marcados com `<...>`.
2. **Rodar o `valgrind` uma vez**, num Linux ou no WSL. A contagem de `malloc` e
   `free` mostra que nada vazou, mas não cobre apontador pendurado nem escrita
   fora de vetor. O `-g` agora está nas opções de compilação, então o valgrind
   vai mostrar número de linha.
3. **Compilar e rodar uma vez no Linux**, pelo mesmo motivo: o ambiente de
   correção é provavelmente Linux e não foi possível testar aqui.
4. **Confirmar com os monitores** se o relatório deve trazer o Id, como o texto
   da especificação diz, ou o número da Pokédex, como o exemplo mostra. Se for o
   Id, é uma linha para mudar em `src/pokelista.c`.

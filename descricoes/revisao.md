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
| 5. Revisão independente | feita |
| 6. Slides, zip e fechamento | ver a seção "O que depende do Gabriel" |

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

O arquivo saiu de 56 linhas para 20.

### `src/missao.c`

| Saiu | Por quê | O que ficou no lugar |
|---|---|---|
| `static` nas 19 funções auxiliares | está na lista do que trocar | as funções sem `static` |
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

O `%d` do `scanf` trunca silenciosamente um número que não cabe num `int`:
`99999999999999999999` entrava como 1.661.992.959. Sem o teto, esse valor
passaria pela checagem de "não pode ser negativo" e entraria no programa como
dado válido.

### `pulaMarcaUtf8`

O Bloco de Notas do Windows escreve três bytes invisíveis no começo dos arquivos
UTF-8 que salva. Sem pular, eles entram colados no nome do primeiro treinador.
Como a entrevista usa arquivos novos, possivelmente criados no Bloco de Notas, o
tratamento ficou. São oito linhas com `fgetc` e `rewind`, ambas de `stdio.h`.

### A checagem de posição na recarga

`pokecenterRecarregarPokebolas` recusa recarregar um treinador que não está no
Centro. É o que dá uso ao parâmetro `cp` — sem ela, a operação de recarga não
usaria o Centro para nada, e um TAD cuja operação não recebe a própria
instância fica estranho. No fluxo normal nunca dispara.

### A escolha entre `del` e `rm` no `clean`

Duas variáveis de ambiente em `ifeq` aninhado. Ficou porque a versão
simplificada, que olhava só o sistema operacional, fazia o `make clean` **não
apagar nada em silêncio** quando rodado pelo Git Bash. Ver
[makefile.md](makefile.md).

### Dois trechos que o fluxo normal nunca alcança

A captura que falha por falta de Pokébola (`resgataPokemon`) e o aviso de
Pokémon não recuperado (`encerraMissao`). Ficam porque uma operação de TAD não
deve confiar em quem a chama: se alguém chamar `treinadorCapturar` fora de hora,
a quantidade de Pokébolas não pode ficar negativa. Para alcançá-los de verdade
seria preciso um `malloc` falhando.

### O relatório imprime o número da Pokédex, não o Id

O texto da especificação pede "o ID e o Nome", e o exemplo mostra `610 Axew`,
que é o número da Pokédex. O programa segue o exemplo. A consequência é que no
`teste2.txt` o relatório sai com quatro linhas idênticas `025 Pikachu`. Está na
lista de pendências, porque depende de confirmação com os monitores.

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

## O que depende do Gabriel

1. **Nome e matrícula dos dois alunos**, para o nome do `.zip`, que a
   especificação exige no formato `TP1Aluno1Matricula1Aluno2Matricula2`.
2. **Os slides em PDF**, no formato disponibilizado no Moodle.
3. **Rodar o `valgrind` uma vez**, num Linux ou no WSL. A contagem de `malloc` e
   `free` mostra que nada vazou, mas não cobre apontador pendurado nem escrita
   fora de vetor.
4. **Compilar e rodar uma vez no Linux**, pelo mesmo motivo: o ambiente de
   correção é provavelmente Linux e não foi possível testar aqui.
5. **Confirmar com os monitores** se o relatório deve trazer o Id, como o texto
   da especificação diz, ou o número da Pokédex, como o exemplo mostra. Se for o
   Id, é uma linha para mudar em `src/pokelista.c`.

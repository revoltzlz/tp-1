# TP1 — AEDS I (CCF211), UFV Campus Florestal

Trabalho do Gabriel Henrique, Ciência da Computação. Resgate de Pokémon com
listas encadeadas. **O trabalho está pronto e passou por uma revisão final.**

Fale em português.

## Antes de mexer em qualquer coisa

1. **Leia `descricoes/`.** É a documentação completa do projeto: um arquivo por
   arquivo do código, com as structs, as funções, os custos e as decisões de
   projeto. Comece por `descricoes/README.md`.
2. **Leia `descricoes/revisao.md`.** Ele registra o que a revisão final
   verificou, corrigiu e — principalmente — **o que foi avaliado e mantido de
   propósito**. Vários trechos que parecem complicados estão lá por correção, e
   simplificá-los reintroduz bugs já corrigidos.
3. O PDF `TP1CCF211-2026_2.pdf` é a fonte final. Em qualquer conflito, o PDF
   vence, e me avise.

## Hierarquia de fontes

1. o PDF do enunciado
2. este arquivo
3. `descricoes/`
4. seu julgamento, só para lacunas — a opção mais simples que um aluno de AEDS I
   consiga defender, registrada em `descricoes/revisao.md`

## Nível do código: AEDS I, sempre

O Gabriel precisa explicar **cada linha sozinho** numa entrevista individual. O
código foi deliberadamente simplificado para o nível de um primeiro período de
estrutura de dados.

**Não introduza:** `windows.h`, `SetConsoleOutputCP`, `#ifdef _WIN32`, `#ifdef`
de teste, `setvbuf`, `argc`/`argv`, `snprintf`, `fgets` com `sscanf`, `static`
em função, `size_t` onde `int` basta, `strtol`, `strtok`, `memcpy`, `memset`,
`assert`, `stdbool.h`, `enum`, `union`, ponteiro para função, macro com
parâmetros, recursão onde um laço resolve.

Todos esses **já estiveram no projeto e foram removidos** na revisão final. Se
reintroduzir algum, você está desfazendo trabalho feito.

A regra de padrão do Makefile (`%.o:`, `$<`, `$@`) também estava nessa lista e
**voltou de propósito**: é a forma do modelo que a disciplina disponibilizou, e
o Makefile agora segue esse modelo. Ver
[descricoes/makefile.md](descricoes/makefile.md).

**Pode usar:** `stdio.h`, `stdlib.h`, `string.h`, `math.h`, `time.h`; `#define`;
include guards; `typedef struct`; ponteiros e `->`; `const` em parâmetro de
leitura; `malloc`/`free` com teste de `NULL`; a lista do Ziviani com célula
cabeça; `if/else`, `switch`, `for`, `while`; `printf`, `scanf`, `fscanf`,
`fopen`, `fclose`; `strncpy` com `'\0'` explícito; `rand`, `srand`, `time`,
`sqrt`; funções que devolvem `int` 0 ou 1.

### O que parece complexo mas fica

Cada um destes está justificado em `descricoes/revisao.md`, seção "O que foi
avaliado e ficou". **Não remova sem ler o motivo:**

| Trecho | Por que fica |
|---|---|
| `long long` em `distanciaQuadrado` | a soma não cabe em `int`: chega a 8×10¹² |
| `COORD_MAX` e a validação das coordenadas | sem ela, a soma estoura o `long long`, `sqrt` devolve `nan` e a missão vai para o treinador errado. Esse bug já existiu |
| `MAX_QUANTIDADE` e as validações de faixa | o `%d` do `scanf` trunca em silêncio um número que não cabe em `int` |
| `pulaMarcaUtf8` | o Bloco de Notas põe 3 bytes invisíveis no começo do arquivo, e a entrevista usa arquivos novos |
| a checagem de posição na recarga | dá uso ao parâmetro do Centro e evita criar Pokébolas fora dele |
| o `ifeq` aninhado do `clean` | sem ele o `make clean` não apaga nada no Git Bash, em silêncio |
| os dois trechos que o fluxo nunca alcança | checagem defensiva: um TAD não confia em quem o chama |

## O programa não tem modo de teste dentro dele

A comparação exata com o exemplo do PDF é feita numa **cópia temporária** do
projeto, montada pelo `testes/rodar_testes.sh`, onde a linha do sorteio é
trocada. **Nunca ponha `#ifdef` de teste no código entregue.**

O mesmo vale para a contagem de memória: o script injeta um cabeçalho com
`gcc -include` numa segunda cópia, sem tocar em nenhum arquivo do projeto.

## Comentários

Enxutos, e a explicação longa mora em `descricoes/`.

**Fica:** um cabeçalho de uma ou duas linhas por arquivo; uma linha curta acima
de cada protótipo nos `.h`; nos `.c`, comentário só no que não se lê no código
(célula cabeça, o `ultimo` na remoção, a ordem dos dois testes da missão, a
distância ao quadrado, o `srand` uma vez, o `'\0'` depois do `strncpy`); o
comentário do Makefile, que o PDF exige.

**Sai:** comentário que repete o código, explicação de vários parágrafos,
banners decorativos, tom de tutorial ("IMPORTANTE:", "Passo 1:"), inglês,
emojis, código comentado.

Se mexer em comentários, prove que só eles mudaram:

```bash
gcc -fpreprocessed -dD -E -P arquivo.c   # -x c nos .h
diff -wB antes.pp depois.pp
```

## Estrutura

```
Makefile, main.c        na raiz
include/                os 7 cabeçalhos
src/                    as 5 implementações
testes/                 arquivos de entrada e rodar_testes.sh
testes/oficiais/        os 2 arquivos do Moodle — não altere
descricoes/             a documentação — não vai no zip
gerar_zip.sh            monta e testa o pacote de entrega
```

Os quatro TADs são `pokemon`, `pokelista`, `treinador` e `pokecenter`. O
`missao` não é TAD: é o módulo do sistema de controle. **Não renomeie arquivos
nem funções.**

## Decisões de projeto que não mudam

- **Id do Pokémon = ordem de leitura**, não o número da Pokédex. O
  `teste2.txt` oficial tem quatro Pikachus com o número 025.
- **Pokémon guardado por valor** na célula, nunca por ponteiro.
- **Lista do Ziviani com célula cabeça** e apontador `ultimo`.
- **Distância comparada pelo quadrado**, em inteiros; `sqrt` só ao imprimir.
- **Ordem dos dois testes após a captura:** primeiro "acabaram os fugitivos?",
  depois "ficou sem Pokébolas?". É o que o exemplo do PDF exige.
- **Entrega na ordem de captura**, e no retorno final o treinador 1 entrega
  antes do 2.
- **`srand` uma única vez**, no `main`.
- **Dois treinadores em duas variáveis**, não em vetor.
- **Mensagens com acento** (a saída tem de bater com o PDF), **comentários sem
  acento**.
- O relatório imprime o **número da Pokédex**, seguindo o exemplo do PDF, e não
  o Id que o texto dele menciona. Errata registrada.

## Regras de trabalho

- Commits pequenos, um por etapa, com mensagem que diz o que mudou. Explique em
  uma linha cada comando git que rodar — o Gabriel está aprendendo git.
- Nunca `git push`, `--force` ou `reset --hard`, nem apagar arquivos dele, sem
  perguntar. Para reorganizar, `git mv`.
- Nunca invente resultado de teste nem ajuste a saída esperada para bater com o
  programa.
- Depois de qualquer mudança: compile com zero avisos e rode
  `bash testes/rodar_testes.sh`. Se algum teste falhar, desfaça.
- Toda mudança vai para `descricoes/revisao.md`, com o motivo.

## Ambiente

Windows 11, MinGW (`gcc`, `mingw32-make`), Git Bash e PowerShell.

**O Smart App Control bloqueia os executáveis** que o gcc gera, de forma
imprevisível. O `rodar_testes.sh` contorna tentando várias opções de
otimização. As alternativas definitivas estão em `descricoes/README.md`.

Para os acentos no terminal do Windows: `chcp 65001` antes de executar.

## Os alunos

Gabriel Henrique R. Macedo — 07237
Diego Soares Carvalho — 07225

O `.zip` de entrega sai como
`TP1GabrielHenriqueRMacedo07237DiegoSoaresCarvalho07225.zip`; o comando está no
comentário do topo do `gerar_zip.sh`.

## O que ainda depende deles

1. rodar o `valgrind` uma vez, num Linux;
2. compilar e rodar uma vez no Linux — não foi possível testar aqui, porque o
   WSL não inicia (virtualização desligada no firmware);
3. confirmar com os monitores se o relatório leva o Id ou o número da Pokédex.

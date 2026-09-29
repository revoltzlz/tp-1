# REQUISITOS — TP1 AEDS I (CCF211 / 2026-2)

Cada exigência do PDF `TP1CCF211-2026_2.pdf` virou um item numerado (R01, R02...).
A coluna **Pág./Lin.** aponta a linha do texto extraído do PDF (`pdftotext -layout`),
que é a referência usada nesta sessão.

Status: `pendente` | `feito` | `verificado`
(`verificado` = conferido contra o PDF com evidência anotada na coluna Evidência)

---

## A. Entrega, prazos e regras gerais

| # | Exigência | Lin. | Trecho do PDF | Status | Evidência |
|---|---|---|---|---|---|
| R01 | Trabalho em dupla, valor 10 pontos | 9 | "Valor: 10 pontos" | verificado | — |
| R02 | Entrega no Moodle em `.zip` ou `.tar.gz` | 12 | "Forma de Entrega: Moodle (formato .zip ou .tar.gz)" | pendente | |
| R03 | `.zip` nomeado `TP1Aluno1Matricula1Aluno2Matricula2` | 318 | "nomeado no formato "TP1Aluno1Matricula1Aluno2Matricula2"" | pendente | |
| R04 | `.zip` contém **todo** o código-fonte, **incluindo os `.h`** | 318-319 | "(1) todo código-fonte produzido, incluindo os arquivos de cabeçalho" | pendente | |
| R05 | `.zip` contém slides em **PDF**, no formato disponibilizado no Moodle | 319-320 | "(2) um conjunto de slides, utilizando o formato disponibilizado no Moodle. O formato para a slides é pdf." | pendente | **Depende do Gabriel: baixar o modelo do Moodle.** |
| R06 | Avaliação por entrevista com monitores + avaliação do código | 322-324 | "avaliado através de uma apresentação/entrevista" | — | Cada aluno explica sozinho. |
| R07 | Trabalhos copiados serão penalizados | 307 | "Trabalhos copiados serão penalizados." | — | **O PDF não diz nada sobre uso de IA.** Única regra: não copiar. |
| R08 | Erros da especificação podem e devem ser reportados | 310-312 | "A especificação pode conter erros ou problemas não intencionais que podem e devem ser reportados" | pendente | Erratas na seção E; vão para os slides. |
| R09 | Decisões de projeto são responsabilidade da dupla | 312-313 | "diversas decisões de projeto e desenvolvimento precisam e devem ser feitas por cada dupla" | pendente | Seção F. |
| R10 | Implementações extras são permitidas, mas só depois de cumprir tudo o que foi pedido | 313-315 | "Implementações extras são possíveis e serão bem-vindas, desde que tudo o que foi explicitamente pedido [...] tenha sido contemplado" | — | |
| R11 | Soluções que não sejam TADs são "duramente penalizadas" | 309-310 | "Soluções que não correspondam à implementação de Tipos Abstratos de Dados serão duramente penalizadas" | pendente | |

## B. Estrutura de dados obrigatória

| # | Exigência | Lin. | Trecho do PDF | Status | Evidência |
|---|---|---|---|---|---|
| R12 | A estrutura das listas lineares é **OBRIGATORIAMENTE LISTA ENCADEADA** | 38-39 | "a estrutura de dados a ser utilizada [...] será OBRIGATORIAMENTE LISTA ENCADEADA" | pendente | Nenhuma coleção de Pokémon pode ser vetor. |

## C. Os quatro TADs

### TAD Pokémon (lin. 45-57)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R13 | Atributo: `Id` — inteiro de identificação **único** | 50 | pendente | |
| R14 | Atributo: `Número na Pokedex` | 51 | pendente | |
| R15 | Atributo: `Nome` | 52 | pendente | |
| R16 | Atributo: `Tipo` | 53 | pendente | |
| R17 | Atributo: `Localização` (coordenadas X e Y) | 54 | pendente | |
| R18 | Operações: get e set de valores, inicialização e impressão | 56-57 | pendente | |

### TAD PokeLista (lin. 59-67)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R19 | É uma lista encadeada de elementos do tipo Pokémon | 60 | pendente | |
| R20 | Operação: inicialização da lista | 63 | pendente | |
| R21 | Operação: inserção de um Pokémon | 64 | pendente | |
| R22 | Operação: remoção de um Pokémon | 65 | pendente | |
| R23 | Operação: busca de Pokémon **pelo Id** | 66 | pendente | |
| R24 | Operação: impressão dos Pokémon armazenados | 67 | pendente | |

### TAD Treinador (lin. 69-87)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R25 | Atributos: Identificador, Nome, Localização, PokeLista, Quantidade de Pokébolas | 73-77 | pendente | |
| R26 | Inicialização define identificação, **localização inicial (0,0)** e quantidade inicial de Pokébolas | 81-82 | pendente | |
| R27 | Operação: movimentação para uma determinada coordenada | 84 | pendente | |
| R28 | Operação: captura de um Pokémon, que **consome uma Pokébola** | 85 | pendente | |
| R29 | Operação: retirar um Pokémon da sua PokeLista | 86 | pendente | |
| R30 | Operação: impressão das informações do treinador | 87 | pendente | |

### TAD Centro de Pesquisa (lin. 89-104)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R31 | Contém **duas** instâncias de PokeLista: fugitivos e recuperados | 92-94 | pendente | |
| R32 | Localização do Centro é **(0,0)** | 94-95 | pendente | |
| R33 | Operação: inicialização do centro de pesquisa | 98 | pendente | |
| R34 | Operação: inserção dos registros de Pokémon fugitivos | 99 | pendente | |
| R35 | Operação: remoção de um Pokémon da lista de fugitivos | 100 | pendente | |
| R36 | Operação: impressão dos Pokémon que ainda não foram recuperados | 101 | pendente | |
| R37 | Operação: recebimento dos Pokémon recuperados pelos treinadores | 102 | pendente | |
| R38 | Operação: recarga de Pokébolas — quantidade **aleatória de 1 a 20** | 103-104 | pendente | |

## D. Sistema de Controle da Missão (programa principal)

| # | Exigência | Lin. | Trecho / regra | Status | Evidência |
|---|---|---|---|---|---|
| R39 | Programa principal valida os TADs "a partir da utilização de **todos** os elementos disponíveis" | 107-109 | — | pendente | Tabela função × uso no `GUIA_ENTREVISTA.md`. |
| R40 | Inicialização: cria 1 Centro de Pesquisa e **2** Treinadores | 112-114 | — | pendente | |
| R41 | Registro: lê o arquivo de entrada, registra os fugitivos na PokeLista do Centro e inicializa os dois treinadores | 116-119 | — | pendente | |
| R42 | Missão de captura: para cada Pokémon registrado, atribui ao treinador de **menor distância euclidiana naquele momento** | 121-125 | — | pendente | |
| R43 | Movimenta o treinador escolhido até a coordenada do Pokémon, captura e adiciona à PokeLista dele | 125-127 | — | pendente | |
| R44 | Empate de distância → treinador de **menor identificador** | 127-129 | — | pendente | |
| R45 | Atualização da listagem de fugas **a cada captura** | 131-133 | — | pendente | |
| R46 | Retorno (caso 1): último Pokémon resgatado → **ambos** retornam e devolvem todos os Pokémon da sua PokeLista à lista de recuperados | 138-141 | "removê-los da sua PokeLista e adicioná-los à lista de Pokémon recuperados" | pendente | |
| R47 | Retorno (caso 2): logo após uma captura, se o treinador ficou sem Pokébolas → retorna, devolve o que capturou e **solicita novo carregamento** | 142-145 | — | pendente | |
| R48 | Relatório final em arquivo **`.txt`** com os Pokémon recuperados | 147-150 | "emitir um relatório no formato de arquivo .txt" | pendente | |
| R49 | Conteúdo do relatório: **"o ID e o Nome"** (texto) — mas o exemplo mostra o **número da Pokédex** | 149-150 / 288-293 | — | pendente | **Errata E01.** |
| R50 | Modo de utilização **por arquivo** | 153 | "Para o modo de utilização por arquivo" | pendente | |
| R51 | Modo de utilização **interativo** | 170-171 | "criar seus próprios casos de teste, tanto interativos quanto por arquivo" | pendente | |
| R52 | Formato do arquivo de entrada conforme lin. 156-166 | 152-166 | Ver seção G | pendente | `testes/entrada_exemplo.txt` |
| R53 | Informações do Pokémon na entrada: Número na Pokédex, nome, tipo, coordenadas X e Y | 165-166 | — | pendente | |
| R54 | A dupla deve criar seus próprios casos de teste, interativos e por arquivo | 169-172 | — | pendente | pasta `testes/` |
| R55 | Saída no terminal deve **detalhar a execução das funcionalidades** | 174-175 | — | pendente | |
| R56 | A aleatoriedade da recarga permite resultados diferentes a cada execução; o **fluxo** é que precisa estar correto | 175-178 | "o fator de aleatoriedade [...] possibilitará diferentes resultados a cada execução, entretanto, o fluxo das operações deve ser corretamente detalhado" | pendente | A saída de exemplo é "uma possível solução". |
| R57 | Saída de exemplo do terminal (lin. 179-285) | 179-285 | — | pendente | `testes/saida_exemplo_pdf.txt` (transcrição validada por `diff`) |
| R58 | Saída de exemplo do relatório (lin. 288-293) | 287-293 | — | pendente | `testes/relatorio_exemplo_pdf.txt` (idem) |

## E. Informações Importantes (lin. 295-307)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R59 | Programa organizado em **módulos**, conforme estudado em sala | 298-300 | pendente | |
| R60 | O módulo do **programa principal separado** dos módulos dos TADs | 299-300 | pendente | |
| R61 | Programa **bem indentado** | 302 | pendente | 4 espaços |
| R62 | Programa **comentado** | 302 | pendente | |
| R63 | **Makefile com comentário explicando como compilar** | 303 | pendente | |
| R64 | Números fixos definidos como **constantes** | 304-306 | pendente | `#define` |

## F. Requisitos adicionais (decisões da dupla / boas práticas da disciplina)

Não são exigências literais do PDF, mas são cobrados na entrevista e no material da
disciplina. Registrados aqui para não se perderem.

| # | Item | Status | Evidência |
|---|---|---|---|
| R65 | Compilar com `gcc -Wall -Wextra -std=c99` com **zero avisos** | pendente | |
| R66 | Toda a memória alocada é liberada (inclusive a célula cabeça de cada lista) | pendente | O PDF só diz "limpem a memória" (lin. 35-36), na narrativa. |
| R67 | Todo `malloc` testado contra `NULL` | pendente | |
| R68 | Include guards em todo `.h`, únicos no projeto | pendente | |
| R69 | Sem ciclo de inclusão: `centro.h → treinador.h → pokelista.h → pokemon.h` | pendente | |
| R70 | Prefixo do TAD em todas as funções (`pokemonGetId`, `treinadorGetId`...) | pendente | C não tem sobrecarga. |
| R71 | Comentário acima de cada protótipo nos `.h` | pendente | |
| R72 | Nenhuma operação declarada fica sem uso (sem código morto) | pendente | Decorre de R39. |
| R73 | Entradas inválidas não travam o programa (arquivo inexistente, campos faltando, etc.) | pendente | Entrevista usa arquivos novos. |
| R74 | `srand(time(NULL))` chamado **uma única vez**, no `main` | pendente | |
| R75 | `.gitignore` com `*.exe`, `*.o`, `*.zip` e o relatório gerado | pendente | |
| R76 | Zip testado: extrair em pasta limpa, compilar do zero, rodar o exemplo | pendente | |

---

## G. Formato do arquivo de entrada (reconstrução do PDF)

O bloco de entrada do PDF (lin. 156-163) saiu **embaralhado** na extração de texto,
porque no documento original ele está em três colunas (dados + comentários ao lado).
O que o `pdftotext -layout` mostra é:

```
Rosa 2         Dragão   77       /*Nome dos treinadores e qnt*
Nate 2         Água     10 7     /*inicial de Pokebolas*/
5              Folha    11       /*Qnt de pokémon fugitivos*/
610 Axew       Voador   -10 -10  /*Informações dos pokémon*/
657 Frogadier  Noturno  57
387 Turtwig
715 Noivern
197 Umbreon
```

A extração **sem** `-layout` (ordem de conteúdo) separa as colunas e confirma a leitura:

```
Rosa 2 Nate 2 5 610 Axew 657 Frogadier 387 Turtwig 715 Noivern 197 Umbreon
Dragão Água Folha Voador Noturno
77 10 7 11 -10 -10 57
```

Ou seja, o arquivo real é:

```
Rosa 2
Nate 2
5
610 Axew Dragão 7 7
657 Frogadier Água 10 7
387 Turtwig Folha 1 1
715 Noivern Voador -10 -10
197 Umbreon Noturno 5 7
```

**Esta reconstrução foi validada de duas formas independentes:**

1. Recalculando as 10 distâncias do exemplo a partir dela, todas as 10 batem com os
   valores impressos no PDF (9.90/9.90, 3.00/12.21, 1.41/1.41, 15.56/14.14,
   7.21/22.67), e a ordem do relatório (610, 657, 387, 197, 715) também é reproduzida.
2. **Confirmada depois contra o arquivo oficial** `testes/oficiais/teste1.txt`, que o
   Gabriel baixou do Moodle: é idêntico byte a byte (a única diferença é que o oficial
   está em CRLF e não tem quebra de linha na última linha).

Fica gravado também em `testes/entrada_exemplo.txt` (versão em LF).

Formato, portanto:

1. Linha 1: nome do treinador 1 + quantidade inicial de Pokébolas
2. Linha 2: nome do treinador 2 + quantidade inicial de Pokébolas
3. Linha 3: quantidade `n` de Pokémon fugitivos
4. Linhas 4 a 3+`n`: `<numeroPokedex> <nome> <tipo> <x> <y>`

Observações:
- A entrada **não traz o identificador** dos treinadores nem o **Id** dos Pokémon
  (ver decisão D02 e D03).
- A entrada **não traz a localização** dos treinadores: ela é (0,0) por R26.

### Arquivos de teste oficiais (`testes/oficiais/`)

Os dois arquivos que o PDF menciona na lin. 169-170, baixados do Moodle.

- **`teste1.txt`** — o exemplo do PDF (5 Pokémon). Oráculo completo: a saída deve bater
  com `testes/saida_exemplo_pdf.txt` e o relatório com
  `testes/relatorio_exemplo_pdf.txt`, a menos da quantidade sorteada na recarga.
- **`teste2.txt`** — 20 Pokémon. **Contém quatro Pikachus com o mesmo número de
  Pokédex `025`** (linhas 9, 16, 19 e 23). Isso confirma R13 + D02 de forma definitiva:
  o Id **não pode** ser o número da Pokédex, porque ele não é único. Um programa que
  usasse a Pokédex como Id daria busca e remoção erradas neste arquivo.

Características dos dois arquivos que impõem requisitos de robustez:

| Característica | Consequência |
|---|---|
| Terminação de linha **CRLF** (`
`) | O `` não pode entrar no nome nem no tipo (R73). |
| **Sem quebra de linha** na última linha | A leitura do último Pokémon não pode depender de `
` final. |
| Número da Pokédex com **zeros à esquerda** (`025`, `004`, `007`, `001`, `039`, `094`) | Ver D18. |
| Acentos em UTF-8 nos tipos (`Dragão`, `Água`, `Elétrico`, `Psíquico`) | Cada acento gasta **2 bytes**; `TAM_TIPO` precisa de folga (D19). |
| Nomes de até 10 caracteres (`Charmander`, `Jigglypuff`) | `TAM_NOME 12` do rascunho era apertado (D19). |

---

## H. Erratas e ambiguidades do PDF

Amparadas por R08 ("erros [...] podem e devem ser reportados"). Vão para os slides.

| # | Onde | O que o PDF diz | Problema | Como foi tratado |
|---|---|---|---|---|
| **E01** | lin. 149-150 vs. 288-293 | O texto pede "o **ID** e o Nome" no relatório; o exemplo imprime `610 Axew`, `657 Frogadier`... | 610 e 657 são **números da Pokédex**, não Ids. Com Id = ordem de leitura (D02), o Axew teria Id 1. | Seguimos o **exemplo** (número da Pokédex + nome), por ser o artefato concreto de conferência. Registrado nos slides. **Pergunta aberta ao Gabriel: os monitores responderam?** |
| **E02** | lin. 156-163 | Bloco do arquivo de entrada | Em três colunas no documento; a cópia embaralha as colunas e junta coordenadas (`77` = `7 7`, `11` = `1 1`, `57` = `5 7`). | Reconstruído e validado numericamente (seção G). |
| **E03** | lin. 50 vs. 152-166 | "Id (valor inteiro de identificação que deve ser único)" | O formato de entrada não fornece o Id. | O programa gera o Id (D02). |
| **E04** | lin. 156 | `/*Nome dos treinadores e qnt*` | Comentário do exemplo sem o `/` de fechamento. | Irrelevante: são comentários explicativos do PDF, não fazem parte do arquivo. |
| **E05** | lin. 281-285 | Bloco final `MISSÃO CONCLUÍDA` | Traz uma linha em branco **extra** entre o `====` e o título, e um caractere invisível `U+200B` (zero-width space) antes dele — artefato de formatação do editor de texto. Os outros três blocos de cabeçalho não têm isso. | O programa usa o **mesmo padrão dos outros três blocos** (título na linha seguinte ao `====`, sem caractere invisível). Única diferença esperada no `diff` contra `testes/saida_exemplo_pdf.txt`, que é transcrição fiel. |
| **E06** | lin. 271 vs. 187 | "Todos **Pokemons** foram resgatados" (sem acento) vs. "**Pokémons** fugitivos" (com acento) | Inconsistência de acentuação no próprio exemplo. | Reproduzido **exatamente como está** nas duas linhas — é o que o exemplo manda. |
| **E07** | lin. 288-293 | Relatório com 1 espaço no início de **todas** as 6 linhas | Não há como distinguir "indentação do parágrafo no documento" de "conteúdo real do arquivo". | Geramos **sem** o espaço inicial (`610 Axew`). Justificativa: o espaço é uniforme nas 6 linhas, típico de recuo de parágrafo do editor. Registrado nos slides. |
| ~~E08~~ | lin. 261 | ~~Distância de Nate até o Umbreon~~ | **Não é errata.** O PDF imprime **22.67**, que é o valor correto de (−10,−10) até (5,7): √(15²+17²) = √514 = 22.671. O valor 17.72 que aparecia na nossa anotação não está no PDF. | Nada a fazer. |

**Todas as 10 distâncias do exemplo do PDF estão matematicamente corretas.** O exemplo é
100% reproduzível.

---

## I. Decisões de projeto

Amparadas por R09. Vão para os slides e para o `GUIA_ENTREVISTA.md`.

| # | Decisão | Por quê |
|---|---|---|
| D01 | Lista encadeada **com célula cabeça** e apontador `ultimo` (Ziviani, 2.1.2) | Inserção no fim em O(1); remoção sem caso especial para o primeiro elemento; lista vazia ⟺ `primeiro == ultimo`. |
| D02 | **Id do Pokémon = ordem de leitura** (1, 2, 3...) | R13 exige Id único. O número da Pokédex **não** é único (dois Pokémon da mesma espécie podem fugir). A entrada não traz Id (E03). |
| D03 | **Identificador do treinador = ordem de leitura** (1 e 2) | A entrada não traz identificador; R44 precisa de um para desempatar. Treinador 1 = primeira linha. |
| D04 | Pokémon guardado **por valor** na célula, não por ponteiro | Passar de uma lista para outra é remover (copia para fora) + inserir (copia para dentro). Nenhuma memória compartilhada entre listas: sem ponteiro pendurado e sem `free` duplo. |
| D05 | Comparação de distância por **quadrado inteiro** (`dx*dx + dy*dy`); `sqrt` só ao imprimir | Comparação exata, sem comparar `double` com `==`. O empate de R44 fica confiável. |
| D06 | Ordem dos testes após a captura: **1º** "acabaram os fugitivos?" → **2º** "ficou sem Pokébolas?" | Confirmado pelo exemplo: o Umbreon zera as Pokébolas da Rosa (lin. 268) e **não** dispara recarga — vai direto para o retorno final (lin. 270). |
| D07 | Entrega na **ordem de captura**: o treinador sempre retira o primeiro da sua lista e o Centro insere no fim dos recuperados | Reproduz a ordem `610, 657, 387, 197, 715` do relatório do PDF. |
| D08 | No retorno final, **Rosa (id 1) entrega antes de Nate (id 2)** | Confirmado pelo relatório: `387, 197` (Rosa) vêm antes de `715` (Nate). |
| D09 | Ao retornar ao Centro, o treinador **se move para (0,0)** | Confirmado pelo exemplo: depois da recarga, a distância da Rosa até o Turtwig (1,1) é 1.41, ou seja, ela está em (0,0). |
| D10 | `srand(time(NULL))` uma única vez no `main`; recarga = `MIN_RECARGA + rand() % (MAX_RECARGA - MIN_RECARGA + 1)` | Se o `srand` ficasse dentro da recarga, duas recargas no mesmo segundo sorteariam o mesmo número. |
| D11 | `pokemonInicializar` e `treinadorInicializar` preenchem os campos **chamando os próprios `set`** | Atende R18 (sets existem e têm uso real) sem criar código morto (R72) nem duplicar código. |
| D12 | Treinador que começa com **0 Pokébolas** recarrega no Centro antes da primeira missão | Ninguém parte para captura sem Pokébola. Os treinadores já começam em (0,0) = Centro (R26/R32). A captura continua devolvendo falha se não houver Pokébola (checagem defensiva), e nesse caso o Pokémon não sai da lista de fugitivos. |
| D13 | Uma única função de leitura recebendo `FILE *` | `fopen` no modo arquivo, `stdin` no interativo. Evita código duplicado; as mensagens que pedem cada dado só saem no modo interativo. |
| D14 | Nome do relatório fixado em uma constante | R64 (nenhum nome fixo solto). O PDF não define o nome do arquivo. |
| D15 | Dois treinadores em **duas variáveis**, não em vetor | R12 obriga lista encadeada para as listas lineares; manter o vetor fora do projeto evita qualquer dúvida na correção. R40 fixa o número em dois. |
| D16 | A impressão da lista de fugitivos (R36) é chamada no **modo interativo**, após o registro | Dá uso real a R24/R36 sem poluir a saída do modo arquivo, que precisa bater com o exemplo (R57). O exemplo do PDF é do modo arquivo (lin. 174: "Como resolução do primeiro arquivo de teste"). |
| D17 | A escrita do relatório fica no TAD **PokeLista** (`pokelistaEscreverRelatorio`), recebendo um `FILE *` já aberto; o Centro abre o arquivo, escreve o cabeçalho e fecha | Quem sabe percorrer a lista é a lista. Mantém O(n) e o encapsulamento: o Centro nunca toca em `primeiro`/`prox`. A alternativa (um `getPorPosicao` chamado de fora) seria O(n²). |
| D18 | Número da Pokédex impresso com **`%03d`** | Os arquivos oficiais trazem `025`, `004`, `007`, `001`. Lido como `int`, `025` vira 25; com `%03d` volta a sair `025`, igual à entrada, e é o formato canônico da Pokédex. Para os números do exemplo do PDF (610, 657, 387, 715, 197) `%03d` e `%d` dão o mesmo resultado. |
| D19 | `TAM_NOME` de 12 → **30**; `TAM_TIPO` **20** | `Jigglypuff`/`Charmander` têm 10 caracteres e já ocupam 11 dos 12 bytes. Os tipos acentuados (`Elétrico`, `Psíquico`) gastam 9 bytes em UTF-8. A entrevista usa arquivos novos (R73), então a folga é barata. |
| D20 | O `` do CRLF é removido de nome e tipo depois da leitura | Os arquivos oficiais estão em CRLF. Sem isso, `%s` no `fscanf` engole o `` no fim do tipo e a impressão sai com um retorno de carro no meio da linha. |

---

## J. Pendências que dependem do Gabriel

1. **Nomes e matrículas** dos dois alunos (R03).
2. **Modelo de slides do Moodle** (R05) — não está na pasta.
3. **Os "dois pequenos arquivos de teste"** que o PDF diz que foram disponibilizados
   junto com a especificação (lin. 169-170) — não estão na pasta. Vale baixar do Moodle
   e conferir contra `testes/entrada_exemplo.txt`.
4. **Resposta dos monitores sobre E01** (Id × número da Pokédex no relatório).

# Requisitos da especificação, um por um

Cada exigência do PDF `TP1CCF211-2026_2.pdf` virou um item numerado (R01, R02...).
A coluna **Pág./Lin.** aponta a linha do texto extraído do PDF (`pdftotext -layout`),
que é a referência usada nesta sessão.

> **Conferido de novo na revisão final.** Todas as evidências abaixo foram
> refeitas a partir do PDF e da execução do programa, não das marcações
> anteriores. O que a revisão mudou está em [revisao.md](revisao.md).

Status: `pendente` | `feito` | `verificado`
(`verificado` = conferido contra o PDF com evidência anotada na coluna Evidência)

---

## A. Entrega, prazos e regras gerais

| # | Exigência | Lin. | Trecho do PDF | Status | Evidência |
|---|---|---|---|---|---|
| R01 | Trabalho em dupla, valor 10 pontos | 9 | "Valor: 10 pontos" | verificado | — |
| R02 | Entrega no Moodle em `.zip` ou `.tar.gz` | 12 | "Forma de Entrega: Moodle (formato .zip ou .tar.gz)" | verificado | `.zip` gerado na Fase 8; formato aceito pelo Moodle. |
| R03 | `.zip` nomeado `TP1Aluno1Matricula1Aluno2Matricula2` | 318 | "nomeado no formato "TP1Aluno1Matricula1Aluno2Matricula2"" | pendente | **Depende do Gabriel: nomes e matriculas.** |
| R04 | `.zip` contém **todo** o código-fonte, **incluindo os `.h`** | 318-319 | "(1) todo código-fonte produzido, incluindo os arquivos de cabeçalho" | pendente | Conteudo do zip definido na Fase 8. |
| R05 | `.zip` contém slides em **PDF**, no formato disponibilizado no Moodle | 319-320 | "(2) um conjunto de slides, utilizando o formato disponibilizado no Moodle. O formato para a slides é pdf." | pendente | **Depende do Gabriel: baixar o modelo do Moodle.** **Depende do Gabriel: modelo de slides do Moodle.** |
| R06 | Avaliação por entrevista com monitores + avaliação do código | 322-324 | "avaliado através de uma apresentação/entrevista" | — | Cada aluno explica sozinho. |
| R07 | Trabalhos copiados serão penalizados | 307 | "Trabalhos copiados serão penalizados." | — | **O PDF não diz nada sobre uso de IA.** Única regra: não copiar. |
| R08 | Erros da especificação podem e devem ser reportados | 310-312 | "A especificação pode conter erros ou problemas não intencionais que podem e devem ser reportados" | verificado | Erratas na seção E; vão para os slides. 7 erratas na secao H, cada uma com o trecho do PDF e o tratamento. |
| R09 | Decisões de projeto são responsabilidade da dupla | 312-313 | "diversas decisões de projeto e desenvolvimento precisam e devem ser feitas por cada dupla" | verificado | Seção F. 20 decisoes de projeto na secao I. |
| R10 | Implementações extras são permitidas, mas só depois de cumprir tudo o que foi pedido | 313-315 | "Implementações extras são possíveis e serão bem-vindas, desde que tudo o que foi explicitamente pedido [...] tenha sido contemplado" | — | |
| R11 | Soluções que não sejam TADs são "duramente penalizadas" | 309-310 | "Soluções que não correspondam à implementação de Tipos Abstratos de Dados serão duramente penalizadas" | verificado | 4 TADs em `include/` e `src/`, cada um com struct e operacoes proprias. |

## B. Estrutura de dados obrigatória

| # | Exigência | Lin. | Trecho do PDF | Status | Evidência |
|---|---|---|---|---|---|
| R12 | A estrutura das listas lineares é **OBRIGATORIAMENTE LISTA ENCADEADA** | 38-39 | "a estrutura de dados a ser utilizada [...] será OBRIGATORIAMENTE LISTA ENCADEADA" | verificado | Nenhuma coleção de Pokémon pode ser vetor. `Pokelista` e lista encadeada (`include/pokelista.h`). Nenhum vetor de Pokemon no projeto. Os dois treinadores sao duas variaveis (D15). |

## C. Os quatro TADs

### TAD Pokémon (lin. 45-57)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R13 | Atributo: `Id` — inteiro de identificação **único** | 50 | verificado | `pokemon.h` campo `identificacao`, atribuido na ordem de leitura em `missao.c:leFugitivos`. Provado por `testes/pokedex_repetida.txt` e por `teste2.txt`, em que 4 Pikachus de Pokedex 025 sao capturados individualmente. |
| R14 | Atributo: `Número na Pokedex` | 51 | verificado | `pokemon.h` campo `numPokedex`. |
| R15 | Atributo: `Nome` | 52 | verificado | `pokemon.h` campo `nome`. |
| R16 | Atributo: `Tipo` | 53 | verificado | `pokemon.h` campo `tipo`. **Faltava no rascunho.** |
| R17 | Atributo: `Localização` (coordenadas X e Y) | 54 | verificado | `pokemon.h` campo `localizacao`, do tipo `cord` de `coordenadas.h`. |
| R18 | Operações: get e set de valores, inicialização e impressão | 56-57 | verificado | `pokemon.c`: `pokemonInicializar`, 5 Set, 5 Get e `pokemonImprimir`, todos com uso real (a impressao usa os proprios Get). |

### TAD PokeLista (lin. 59-67)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R19 | É uma lista encadeada de elementos do tipo Pokémon | 60 | verificado | `pokelista.h`: `primeiro`, `ultimo` e `tamanho`; celula em `conexao.h` guardando um `Pokemon` por valor. |
| R20 | Operação: inicialização da lista | 63 | verificado | `pokelista.c:pokelistaInicializar`, que aloca a celula cabeca. |
| R21 | Operação: inserção de um Pokémon | 64 | verificado | `pokelista.c:pokelistaInserir`, no fim, em O(1) pelo apontador `ultimo`. |
| R22 | Operação: remoção de um Pokémon | 65 | verificado | `pokelista.c:pokelistaRemover` por Id, e `pokelistaRemoverPrimeiro`, que reaproveita a primeira. |
| R23 | Operação: busca de Pokémon **pelo Id** | 66 | verificado | `pokelista.c:pokelistaBuscar`. Uso real: `missao.c:executaMissao` busca cada Id no Centro antes de montar o resgate. |
| R24 | Operação: impressão dos Pokémon armazenados | 67 | verificado | `pokelista.c:pokelistaImprimir`, usada por `pokecenterImprimirFugitivos` no modo interativo e no caso anomalo. |

### TAD Treinador (lin. 69-87)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R25 | Atributos: Identificador, Nome, Localização, PokeLista, Quantidade de Pokébolas | 73-77 | verificado | `treinador.h`: `identificador`, `nome`, `loccoach`, `lista` (PokeLista) e `qntdpokebolas`. |
| R26 | Inicialização define identificação, **localização inicial (0,0)** e quantidade inicial de Pokébolas | 81-82 | verificado | `treinador.c:treinadorInicializar` usa `TREINADOR_X_INICIAL` e `TREINADOR_Y_INICIAL`, ambos 0. **Os parametros de coordenada do rascunho foram removidos porque o PDF fixa (0,0).** |
| R27 | Operação: movimentação para uma determinada coordenada | 84 | verificado | `treinador.c:treinadorMovimentar(t, x, y)`. **O rascunho nao recebia a coordenada de destino.** |
| R28 | Operação: captura de um Pokémon, que **consome uma Pokébola** | 85 | verificado | `treinador.c:treinadorCapturar` insere na lista e so entao decrementa as Pokebolas; devolve 0 se nao havia Pokebola. |
| R29 | Operação: retirar um Pokémon da sua PokeLista | 86 | verificado | `treinador.c:treinadorRetirarPokemon`, que devolve o Pokemon retirado. |
| R30 | Operação: impressão das informações do treinador | 87 | verificado | `treinador.c:treinadorImprimir`, no formato exato do exemplo do PDF. |

### TAD Centro de Pesquisa (lin. 89-104)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R31 | Contém **duas** instâncias de PokeLista: fugitivos e recuperados | 92-94 | verificado | `pokecenter.h`: campos `fugitivos` e `recuperados`. |
| R32 | Localização do Centro é **(0,0)** | 94-95 | verificado | `pokecenter.c:pokecenterInicializar` usa `CENTRO_X` e `CENTRO_Y`, ambos 0. |
| R33 | Operação: inicialização do centro de pesquisa | 98 | verificado | `pokecenter.c:pokecenterInicializar`; se a segunda lista falhar, libera a primeira antes de retornar erro. |
| R34 | Operação: inserção dos registros de Pokémon fugitivos | 99 | verificado | `pokecenter.c:pokecenterRegistrarFugitivo`, inserindo no fim para preservar a ordem do arquivo. |
| R35 | Operação: remoção de um Pokémon da lista de fugitivos | 100 | verificado | `pokecenter.c:pokecenterRemoverFugitivo`, chamado a cada captura. |
| R36 | Operação: impressão dos Pokémon que ainda não foram recuperados | 101 | verificado | `pokecenter.c:pokecenterImprimirFugitivos`. Teste: modo interativo lista os fugitivos registrados. |
| R37 | Operação: recebimento dos Pokémon recuperados pelos treinadores | 102 | verificado | `pokecenter.c:pokecenterReceberPokemon`, retirando um a um do treinador e inserindo no fim dos recuperados. |
| R38 | Operação: recarga de Pokébolas — quantidade **aleatória de 1 a 20** | 103-104 | verificado | `pokecenter.c:pokecenterRecarregarPokebolas`, com MIN_RECARGA 1 e MAX_RECARGA 20. **Faltava no rascunho e estava no TAD errado.** |

## D. Sistema de Controle da Missão (programa principal)

| # | Exigência | Lin. | Trecho / regra | Status | Evidência |
|---|---|---|---|---|---|
| R39 | Programa principal valida os TADs "a partir da utilização de **todos** os elementos disponíveis" | 107-109 | — | verificado | As 51 funções declaradas nos `.h` têm chamada real. A conferência é automática: um script compara os nomes declarados nos `.h` com os usados nos `.c`. Quem chama cada uma está na descrição do TAD correspondente. |
| R40 | Inicialização: cria 1 Centro de Pesquisa e **2** Treinadores | 112-114 | — | verificado | `missao.c:executa` cria 1 `PokeCenter` e 2 `Treinador`. |
| R41 | Registro: lê o arquivo de entrada, registra os fugitivos na PokeLista do Centro e inicializa os dois treinadores | 116-119 | — | verificado | `missao.c:leTreinador` e `missao.c:leFugitivos`. |
| R42 | Missão de captura: para cada Pokémon registrado, atribui ao treinador de **menor distância euclidiana naquele momento** | 121-125 | — | verificado | `missao.c:resgataPokemon` recalcula as duas distancias a cada alvo, da posicao atual de cada treinador. Teste: as 10 distancias do exemplo conferidas uma a uma. |
| R43 | Movimenta o treinador escolhido até a coordenada do Pokémon, captura e adiciona à PokeLista dele | 125-127 | — | verificado | `missao.c:resgataPokemon`: `treinadorMovimentar` e depois `treinadorCapturar`. |
| R44 | Empate de distância → treinador de **menor identificador** | 127-129 | — | verificado | `missao.c:escolheTreinador` compara os identificadores no empate. Teste: empate de distancia vai para Rosa, o de menor id. |
| R45 | Atualização da listagem de fugas **a cada captura** | 131-133 | — | verificado | `missao.c:resgataPokemon` chama `pokecenterRemoverFugitivo` logo apos cada captura. |
| R46 | Retorno (caso 1): último Pokémon resgatado → **ambos** retornam e devolvem todos os Pokémon da sua PokeLista à lista de recuperados | 138-141 | "removê-los da sua PokeLista e adicioná-los à lista de Pokémon recuperados" | verificado | `missao.c:encerraMissao`. Teste: lista de fugas vazia e relatorio com exatamente n Pokemon nos 18 casos validos. |
| R47 | Retorno (caso 2): logo após uma captura, se o treinador ficou sem Pokébolas → retorna, devolve o que capturou e **solicita novo carregamento** | 142-145 | — | verificado | `missao.c:retornaAoCentro`, chamado quando as Pokebolas chegam a zero. Teste: `testes/uma_pokebola.txt` recarrega a cada captura. |
| R48 | Relatório final em arquivo **`.txt`** com os Pokémon recuperados | 147-150 | "emitir um relatório no formato de arquivo .txt" | verificado | `pokecenter.c:pokecenterGerarRelatorio` com `pokelista.c:pokelistaEscreverRelatorio`. |
| R49 | Conteúdo do relatório: **"o ID e o Nome"** (texto) — mas o exemplo mostra o **número da Pokédex** | 149-150 / 288-293 | — | verificado | **Errata E01.** Seguimos o exemplo: numero da Pokedex mais nome. O `diff` do relatorio passa. **Errata E01. Consequência concreta, que precisa ser dita na entrevista: em `teste2.txt` o relatório sai com quatro linhas idênticas `025 Pikachu`, indistinguíveis entre si — exatamente o problema que o Id único existe para resolver. Imprimir o Id resolveria, mas afastaria o relatório do exemplo do PDF.** |
| R50 | Modo de utilização **por arquivo** | 153 | "Para o modo de utilização por arquivo" | verificado | `missao.c:missaoExecutarPorArquivo`, alcançada pela opção 1 do menu. O atalho por linha de comando que existia foi removido na revisão: a especificação pede menu. |
| R51 | Modo de utilização **interativo** | 170-171 | "criar seus próprios casos de teste, tanto interativos quanto por arquivo" | verificado | `missao.c:missaoExecutarInterativo`. Teste: modo interativo executa a missao e gera o relatorio. |
| R52 | Formato do arquivo de entrada conforme lin. 156-166 | 152-166 | Ver seção G | verificado | `testes/entrada_exemplo.txt` `testes/entrada_exemplo.txt`, identico ao oficial `testes/oficiais/teste1.txt`. |
| R53 | Informações do Pokémon na entrada: Número na Pokédex, nome, tipo, coordenadas X e Y | 165-166 | — | verificado | `missao.c:leFugitivos`, com um unico `fscanf` de 5 campos. |
| R54 | A dupla deve criar seus próprios casos de teste, interativos e por arquivo | 169-172 | — | verificado | pasta `testes/` 32 arquivos de entrada em `testes/`, mais os 2 oficiais e 4 arquivos-oráculo, rodados por `testes/rodar_testes.sh`: 19 casos válidos e 16 inválidos, além do menu, do modo interativo e da memória. 72 verificações no total. Ver [testes.md](testes.md). |
| R55 | Saída no terminal deve **detalhar a execução das funcionalidades** | 174-175 | — | verificado | Saída do exemplo idêntica à do PDF, conferida por `diff`. O `diff` compara o trecho que o exemplo cobre: fora dele ficam só as molduras do menu, que o exemplo naturalmente não mostra. No caminho de sucesso o programa não imprime nada depois da moldura final — a mensagem que existia ali foi removida nesta revisão. |
| R56 | A aleatoriedade da recarga permite resultados diferentes a cada execução; o **fluxo** é que precisa estar correto | 175-178 | "o fator de aleatoriedade [...] possibilitará diferentes resultados a cada execução, entretanto, o fluxo das operações deve ser corretamente detalhado" | verificado | A saída de exemplo é "uma possível solução". A recarga e sorteada no programa entregue, que **nao tem nenhum modo de teste dentro dele**. Para o `diff` exato, o `testes/rodar_testes.sh` copia o projeto para uma pasta temporaria e troca a linha do sorteio so na copia. |
| R57 | Saída de exemplo do terminal (lin. 179-285) | 179-285 | — | verificado | `testes/saida_exemplo_pdf.txt` (transcrição validada por `diff`) `diff` contra `testes/saida_exemplo_esperada.txt` passa. A única diferença contra a transcrição fiel do PDF é a errata E05. A mensagem extra que o programa imprimia depois da moldura final foi removida na revisão, então o `diff` não precisa mais ignorar nada além do menu. |
| R58 | Saída de exemplo do relatório (lin. 288-293) | 287-293 | — | verificado | `testes/relatorio_exemplo_pdf.txt` (idem) `diff` contra `testes/relatorio_exemplo_esperado.txt` passa. Unica diferenca contra o PDF: errata E07. |

## E. Informações Importantes (lin. 295-307)

| # | Exigência | Lin. | Status | Evidência |
|---|---|---|---|---|
| R59 | Programa organizado em **módulos**, conforme estudado em sala | 298-300 | verificado | 6 modulos: os 4 TADs, o modulo da missao e o programa principal. |
| R60 | O módulo do **programa principal separado** dos módulos dos TADs | 299-300 | verificado | `main.c` na raiz, so com `srand`, o ajuste do terminal e a chamada do menu. |
| R61 | Programa **bem indentado** | 302 | verificado | 4 espaços 4 espacos em todo o projeto. |
| R62 | Programa **comentado** | 302 | verificado | Comentario acima de cada prototipo nos `.h`; conferido na Fase 6. |
| R63 | **Makefile com comentário explicando como compilar** | 303 | verificado | `Makefile`, linhas 1 a 40: como compilar, executar e limpar, no Linux e no Windows. |
| R64 | Números fixos definidos como **constantes** | 304-306 | verificado | `#define` 29 constantes, sem contar os 7 include guards. Nenhuma grandeza do problema aparece como numero solto: tamanhos, quantidade de treinadores, coordenadas do Centro, limites da recarga e do mapa, nome do relatorio e opcoes do menu sao todos `#define`. Continuam literais, por serem parte da propria expressao e nao grandezas configuraveis: o `1` de `i + 1` que transforma indice em Id, o `1` inicial do laco de Ids, as precisoes `%.2f` e `%03d` dos formatos, o `5` de `fscanf(...) != 5`, que e a quantidade de campos da propria chamada logo acima, e os tres bytes da marca UTF-8 (`0xEF`, `0xBB`, `0xBF`), que sao o valor fixo definido pelo padrao. |

## F. Requisitos adicionais (decisões da dupla / boas práticas da disciplina)

Não são exigências literais do PDF, mas são cobrados na entrevista e no material da
disciplina. Registrados aqui para não se perderem.

| # | Item | Status | Evidência |
|---|---|---|---|
| R65 | Compilar com `gcc -Wall -Wextra -std=c99` com **zero avisos** | verificado | Teste: build normal, como na entrega, com zero avisos. Conferido também acrescentando `-pedantic`. |
| R66 | Toda a memória alocada é liberada (inclusive a célula cabeça de cada lista) | verificado | O PDF só diz "limpem a memória" (lin. 35-36), na narrativa. Teste: contagem de `malloc` e `free` igual em 7 casos, incluindo os de erro. Em `teste2.txt`, 64 e 64, que é 4 células cabeça mais 3 por Pokémon; em `quinhentos_pokemon.txt`, 1504 e 1504. **Ressalva de método: contar `malloc` e `free` prova que nada vazou, mas não detecta apontador pendurado, uso depois do `free` nem escrita fora de vetor. Para isso seria preciso o `valgrind`, que não existe nesta máquina** — ver a seção J. |
| R67 | Todo `malloc` testado contra `NULL` | verificado | `pokelista.c`, nos dois unicos `malloc` do projeto. |
| R68 | Include guards em todo `.h`, únicos no projeto | verificado | 7 `.h` com 7 guards distintos, conferido compilando um `.c` que inclui todos duas vezes, em ordem inversa. **O rascunho tinha um `#ifndef` sem `#define` e 6 arquivos sem guard.** |
| R69 | Sem ciclo de inclusão: `centro.h → treinador.h → pokelista.h → pokemon.h` | verificado | `coordenadas.h` para `pokemon.h` para `conexao.h` para `pokelista.h` para `treinador.h` para `pokecenter.h` para `missao.h`. Cada `.c` inclui somente o seu `.h`. |
| R70 | Prefixo do TAD em todas as funções (`pokemonGetId`, `treinadorGetId`...) | verificado | C não tem sobrecarga. As 51 funcoes publicas usam o prefixo do TAD. |
| R71 | Comentário acima de cada protótipo nos `.h` | verificado | Conferido na Fase 6, prototipo por prototipo. |
| R72 | Nenhuma operação declarada fica sem uso (sem código morto) | verificado | Decorre de R39. Nenhuma das 51 funções declaradas fica sem chamada, conferido por script nos dois sentidos. Há quatro *ramos* de código que o fluxo normal não alcança, todos caminhos de falha de `malloc`, registrados um a um em [revisao.md](revisao.md). |
| R73 | Entradas inválidas não travam o programa (arquivo inexistente, campos faltando, etc.) | verificado | Entrevista usa arquivos novos. 16 casos invalidos no `rodar_testes.sh`, todos recusados com `Erro:` e codigo de saida 1: arquivo inexistente, vazio, so com espacos, incompleto, campos faltando, quantidade maior que as linhas, texto no lugar de numero em tres campos, valores negativos em tres campos, numero grande demais para caber em `int` em dois campos, coordenada fora do mapa e nome maior que o vetor. |
| R74 | `srand(time(NULL))` chamado **uma única vez**, no `main` | verificado | `main.c`, uma unica chamada de `srand`. |
| R75 | `.gitignore` com `*.exe`, `*.o`, `*.zip` e o relatório gerado | verificado | `.gitignore`. |
| R76 | Zip testado: extrair em pasta limpa, compilar do zero, rodar o exemplo | pendente | Fase 8. |

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
`) | O `
` não pode entrar no nome nem no tipo (R73). |
| **Sem quebra de linha** na última linha | A leitura do último Pokémon não pode depender de `
` final. |
| Número da Pokédex com **zeros à esquerda** (`025`, `004`, `007`, `001`, `039`, `094`) | Ver D18. |
| Acentos em UTF-8 nos tipos (`Dragão`, `Água`, `Elétrico`, `Psíquico`) | Cada acento gasta **2 bytes**; `TAM_TIPO` precisa de folga (D19). |
| Nomes de até 10 caracteres (`Charmander`, `Jigglypuff`) | `TAM_NOME 12` do rascunho era apertado (D19). |

---

## H. Erratas e ambiguidades do PDF

Amparadas por R08 ("erros [...] podem e devem ser reportados"). Vão para os slides. A conta de cada uma está em [exemplo.md](exemplo.md).

| # | Onde | O que o PDF diz | Problema | Como foi tratado |
|---|---|---|---|---|
| **E01** | lin. 149-150 vs. 288-293 | O texto pede "o **ID** e o Nome" no relatório; o exemplo imprime `610 Axew`, `657 Frogadier`... | 610 e 657 são **números da Pokédex**, não Ids. Com Id = ordem de leitura (D02), o Axew teria Id 1. | Seguimos o **exemplo** (número da Pokédex + nome), por ser o artefato concreto de conferência. Registrado nos slides. **Pergunta aberta ao Gabriel: os monitores responderam?** |
| **E02** | lin. 156-163 | Bloco do arquivo de entrada | Em três colunas no documento; a cópia embaralha as colunas e junta coordenadas (`77` = `7 7`, `11` = `1 1`, `57` = `5 7`). | Reconstruído e validado numericamente (seção G). |
| **E03** | lin. 50 vs. 152-166 | "Id (valor inteiro de identificação que deve ser único)" | O formato de entrada não fornece o Id. | O programa gera o Id (D02). |
| **E04** | lin. 156 | `/*Nome dos treinadores e qnt*` | Comentário do exemplo sem o `/` de fechamento. | Irrelevante: são comentários explicativos do PDF, não fazem parte do arquivo. |
| **E05** | lin. 281-285 | Bloco final `MISSÃO CONCLUÍDA` | São **três** desvios em relação aos outros três blocos de moldura do exemplo: (1) uma linha em branco **extra** entre o `====` e o título; (2) um caractere invisível `U+200B` (zero-width space) antes do título; (3) por consequência, a indentação do título é de **2** espaços, contra 18, 12 e 7 dos outros blocos. Tudo indica artefato de formatação do editor de texto. | O programa usa o **mesmo padrão dos outros três blocos**: título na linha seguinte ao `====`, sem caractere invisível, com `INDENT_CONCLUIDA 12`, que centraliza o título nas 40 colunas da moldura. As **duas linhas** de diferença aparecem no `diff testes/saida_exemplo_pdf.txt testes/saida_exemplo_esperada.txt`, que é justamente a documentação da errata. |
| **E06** | lin. 271 vs. 187 | "Todos **Pokemons** foram resgatados" (sem acento) vs. "**Pokémons** fugitivos" (com acento) | Inconsistência de acentuação no próprio exemplo. | Reproduzido **exatamente como está** nas duas linhas — é o que o exemplo manda. |
| **E07** | lin. 288-293 | Relatório com 1 espaço no início de **todas** as 6 linhas | Não há como distinguir "indentação do parágrafo no documento" de "conteúdo real do arquivo". | Geramos **sem** o espaço inicial (`610 Axew`). Justificativa: o espaço é uniforme nas 6 linhas, típico de recuo de parágrafo do editor. Registrado nos slides. |
| ~~E08~~ | lin. 261 | ~~Distância de Nate até o Umbreon~~ | **Não é errata.** O PDF imprime **22.67**, que é o valor correto de (−10,−10) até (5,7): √(15²+17²) = √514 = 22.671. O valor 17.72 que aparecia na nossa anotação não está no PDF. | Nada a fazer. |

**Todas as 10 distâncias do exemplo do PDF estão matematicamente corretas.** O exemplo é
100% reproduzível.

---

## I. Decisões de projeto

Amparadas por R09. Vão para os slides e para o [entrevista.md](entrevista.md).

| # | Decisão | Por quê |
|---|---|---|
| D01 | Lista encadeada **com célula cabeça** e apontador `ultimo` (Ziviani, 2.1.2) | Inserção no fim em O(1); remoção sem caso especial para o primeiro elemento; lista vazia ⟺ `primeiro == ultimo`. |
| D02 | **Id do Pokémon = ordem de leitura** (1, 2, 3...) | R13 exige Id único. O número da Pokédex **não** é único (dois Pokémon da mesma espécie podem fugir). A entrada não traz Id (E03). |
| D03 | **Identificador do treinador = ordem de leitura** (1 e 2) | A entrada não traz identificador; R44 precisa de um para desempatar. Treinador 1 = primeira linha. |
| D04 | Pokémon guardado **por valor** na célula, não por ponteiro | Passar de uma lista para outra é remover (copia para fora) + inserir (copia para dentro). Nenhuma memória compartilhada entre listas: sem ponteiro pendurado e sem `free` duplo. |
| D05 | Comparação de distância pelo **quadrado inteiro** (`dx*dx + dy*dy`), em `long long`; `sqrt` só ao imprimir | Comparação exata, sem comparar `double` com `==`, então o empate de R44 é detectado com segurança. `long long` porque o resultado não cabe em `int`: com as coordenadas no limite do mapa a soma chega a 8×10¹², contra 2,1×10⁹ de um `int`. **A folga só existe porque a leitura recusa coordenadas fora do mapa — ver D21.** |
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
| D19 | `TAM_NOME` de 12 para **30**; `TAM_TIPO` **20** | `Jigglypuff` e `Charmander` tem 10 caracteres e ja ocupam 11 dos 12 bytes. Os tipos acentuados (`Eletrico`, `Psiquico`) gastam 9 bytes em UTF-8. A entrevista usa arquivos novos (R73), entao a folga e barata. |
| D20 | O `\r` do CRLF **nao precisa de tratamento** na leitura dos dados | O `%s` do `fscanf` para em qualquer espaco em branco, e o `\r` conta como espaco em branco. Por isso nome e tipo nunca recebem o `\r` dos arquivos gerados no Windows, e **nao existe nenhum tratamento de fim de linha no programa**. Houve uma versao que lia o caminho do arquivo com `fgets` e precisava de uma funcao `removeFimDeLinha`, porque o `fgets` guarda a quebra de linha; as duas sairam quando o menu passou a ler com `scanf`. |
| D21 | **Coordenadas limitadas a mais ou menos 1.000.000** (`COORD_MAX`), recusadas na leitura se sairem disso | Nao e so validacao de dados: e o que garante que o calculo da distancia nao estoure. Sem o limite, dois pontos nos extremos de `int` dao uma soma de quadrados de 3,2x10^19, que **estoura ate o `long long`** - a soma fica negativa, `sqrt` devolve `nan` e a missao vai para o treinador mais distante. Com o limite, a soma maxima e 8x10^12, que cabe com folga de seis ordens de grandeza. |
| D22 | O programa entregue **não tem nenhum modo de teste dentro dele**. A comparação exata com o exemplo é feita numa cópia temporária do projeto, onde o script de teste troca a linha do sorteio | Um `#ifdef` de teste dentro de um TAD, desligando o sorteio que a especificação exige, é difícil de defender numa entrevista. A cópia temporária resolve sem tocar no código entregue. Houve duas versões anteriores com macro (`RECARGA_FIXA` no TAD Centro e `SEMENTE_FIXA` no `main`), as duas removidas na revisão final. |
| D23 | Teto de sanidade de 1.000.000 (`MAX_QUANTIDADE`) para Pokebolas, quantidade de fugitivos e numero da Pokedex | O `%d` do `scanf`, diante de um numero que nao cabe em `int`, **nao garante o que grava**: o padrao diz que o comportamento e indefinido. Neste compilador `99999999999999999999` virou `1661992959`, um positivo que a checagem de negativo nao pega. Como nao da para confiar no valor, o teto o recusa. |
| D25 | `treinadorImprimir` imprime **nome, posicao e Pokebolas**, e nao o identificador nem a PokeLista | O exemplo da especificacao define o formato dessa linha: `Treinador(a) Rosa: posicao (0,0) | Pokebolas: 2`. Acrescentar campos afastaria a saida do exemplo. Os Pokemon que o treinador carrega aparecem no relatorio final, depois da entrega, e o TAD PokeLista tem a sua propria operacao de impressao, usada pelo Centro de Pesquisa. |
| D26 | Dois trechos defensivos que o fluxo normal nunca alcanca: a captura que falha por falta de Pokebola (`resgataPokemon`) e o aviso de Pokemon nao recuperado (`encerraMissao`) | Ficam porque uma operacao de TAD nao deve confiar em quem a chama. Nao ha teste que os exercite justamente porque o fluxo os torna inalcancaveis: para chegar la seria preciso um `malloc` falhando. Estao anotados como defensivos no proprio codigo. |
| D24 | A marca UTF-8 (BOM) no comeco do arquivo e pulada | O Bloco de Notas do Windows escreve tres bytes invisiveis (`EF BB BF`) no comeco dos arquivos UTF-8. Sem pular, eles entrariam colados no nome do primeiro treinador. So se aplica ao modo por arquivo: numa entrada vinda do teclado nao ha como voltar atras. |

---

## J. Pendências que dependem do Gabriel

1. **Nomes e matrículas** dos dois alunos (R03).
2. **Modelo de slides do Moodle** (R05) — não está na pasta.
3. **Os "dois pequenos arquivos de teste"** que o PDF diz que foram disponibilizados
   junto com a especificação (lin. 169-170) — não estão na pasta. Vale baixar do Moodle
   e conferir contra `testes/entrada_exemplo.txt`.
4. **Resposta dos monitores sobre E01** (Id × número da Pokédex no relatório).
5. **Rodar o `valgrind` uma vez**, no WSL ou em outra máquina Linux, antes da
   entrevista. A contagem de `malloc`/`free` mostra que nada vazou, mas não
   cobre ponteiro pendurado nem escrita fora de vetor. O
   `testes/rodar_testes.sh` já usa o valgrind automaticamente onde ele existir.
5. **Rodar o `valgrind` uma vez**, no WSL ou em outra máquina Linux, antes da
   entrevista. A contagem de `malloc`/`free` mostra que nada vazou, mas não
   cobre ponteiro pendurado nem escrita fora de vetor. O
   `testes/rodar_testes.sh` já usa o valgrind automaticamente onde ele existir.

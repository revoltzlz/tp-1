# `Makefile`

Descreve como compilar o projeto. A especificação exige que o Makefile tenha
"um comentário explicando como compilar o programa", e o comentário do topo
cobre compilar, executar e limpar.

Ele segue o **2º modelo do documento "Makefile" da disciplina**: as variáveis
`CC`, `CFLAGS`, `SRC_DIR`, `BIN`, `SRCS` e `OBJS`, os alvos `all`, `run` e
`clean`, e uma regra de padrão com `$<` e `$@` no lugar de uma regra por
arquivo. As diferenças em relação ao modelo estão na última seção, cada uma
com o motivo.

## As variáveis

```make
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LDLIBS = -lm

SRC_DIR = src
BIN     = tp1
```

| Variável | O que é |
|---|---|
| `CC` | o compilador |
| `CFLAGS` | as opções de compilação |
| `LDLIBS` | as bibliotecas da ligação |
| `SRC_DIR` | a pasta das implementações |
| `BIN` | o nome do executável |
| `SRCS` | os seis arquivos-fonte |
| `OBJS` | os seis arquivos-objeto |
| `HDRS` | os sete cabeçalhos |

### As opções, uma a uma

| Opção | Para quê | Vem do modelo? |
|---|---|---|
| `-Wall` | liga os avisos comuns do compilador | sim |
| `-Iinclude` | diz ao gcc para procurar os cabeçalhos em `include/`. É o que faz `#include "pokemon.h"` funcionar mesmo o arquivo estando em `include/pokemon.h` | sim |
| `-g` | guarda as informações de depuração que o `gdb` e o `valgrind` usam. O documento da disciplina chama isso de essencial | sim, do 1º modelo |
| `-Wextra` | liga mais avisos, que o `-Wall` não cobre, como parâmetro declarado e não usado | acréscimo |
| `-std=c99` | fixa o padrão da linguagem, para o código compilar igual em qualquer máquina em vez de depender do padrão do compilador instalado | acréscimo |
| `-lm` | liga a biblioteca matemática, necessária por causa do `sqrt` | acréscimo |

O projeto compila com **zero avisos** com essas opções, e também acrescentando
`-pedantic`.

### Por que o `-lm` fica no fim

```make
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)
```

O `-lm` vem **depois** dos arquivos-objeto. O ligador processa o que recebe da
esquerda para a direita: quando ele lê `missao.o` e encontra uma chamada a
`sqrt` que não sabe resolver, guarda essa pendência; quando chega em `-lm`,
procura ali e resolve. Se o `-lm` viesse antes, o ligador leria a biblioteca
quando ainda não havia nenhuma pendência, não pegaria nada dela, e depois
reclamaria de `undefined reference to sqrt`.

## As regras

```make
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(HDRS)
```

A compilação tem duas etapas. Primeiro cada `.c` vira um `.o` separado, com a
opção `-c` (compilar sem ligar). Depois os seis `.o` são ligados num único
executável.

Cada regra tem a forma:

```
alvo: arquivos de que ele depende
<TAB>comando que gera o alvo
```

**A linha do comando começa com um TAB, não com espaços.** Isso é regra do
make, não escolha de estilo: com espaços, o erro é `missing separator`.

### A regra de padrão, e por que são duas

```make
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

O `%` é um coringa: a regra diz "qualquer `x.o` é obtido compilando `x.c`". O
`$<` é o primeiro arquivo da lista de dependências (o `.c`) e o `$@` é o alvo
(o `.o`). É a forma do modelo da disciplina, que escreve
`$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c`.

São **duas** regras porque os fontes estão em dois lugares: o `main.c` fica na
raiz, e os cinco TADs ficam em `src/`. O make tenta as regras na ordem e usa a
primeira cujo arquivo de entrada exista de verdade:

| Alvo | tenta `%.o: %.c` | resultado |
|---|---|---|
| `main.o` | procura `main.c` — existe | usa esta |
| `pokemon.o` | procura `pokemon.c` na raiz — não existe | cai na segunda, e acha `src/pokemon.c` |

### Como os cabeçalhos entram

```make
$(OBJS): $(HDRS)
```

O make regera um alvo quando **algum** dos arquivos de que ele depende está
mais novo que ele. Esta linha diz que os seis objetos dependem dos sete
cabeçalhos: mudar qualquer `.h` recompila tudo.

Sem ela, o make só olharia o `.c`, e uma mudança num `.h` não recompilaria
nada — o programa seria ligado com objetos desatualizados, que é um erro
difícil de entender, porque o código-fonte na tela não corresponde ao
executável.

Dá para ser mais fino, dizendo exatamente quais cabeçalhos cada objeto usa. Em
um projeto de seis arquivos isso economizaria frações de segundo e custaria
seis linhas, então não vale.

Está conferido que funciona: tocando `include/pokemon.h`, os seis objetos são
recompilados; tocando só `src/treinador.c`, só o `treinador.o`.

### `.PHONY`

```make
.PHONY: all run clean
```

Diz ao make que `all`, `run` e `clean` são nomes de **tarefa**, e não de
arquivo. Sem isso, se alguém criasse um arquivo chamado `clean` na pasta, o
make acharia que a tarefa já está feita e não faria nada.

## `run`

```make
run: $(BIN)
	./$(BIN)
```

O alvo `run` do modelo executa o programa passando um arquivo de teste na
linha de comando. O nosso programa **não recebe o arquivo por argumento**: ele
abre um menu e pergunta o caminho. Então o `run` só compila, se precisar, e
executa.

## A limpeza

```make
clean:
	-rm -f $(BIN) $(BIN).exe $(OBJS)
	-del /Q /F $(BIN).exe $(OBJS)
```

O comando de apagar depende de **em qual shell o make está rodando**, e não só
do sistema operacional. No Windows o make pode rodar sob o `cmd` ou o
PowerShell, onde existe `del` e não existe `rm`, ou sob o `sh` do Git Bash, onde
é o contrário.

A solução é tentar os dois. O `-` no começo de cada linha manda o make **seguir
em frente** se o comando falhar, então em cada ambiente um dos dois apaga os
arquivos e o outro reclama que não existe:

| Ambiente | o que acontece |
|---|---|
| Linux ou Git Bash | `rm` apaga; `del` não existe e é ignorado |
| Windows, cmd ou PowerShell | `rm` não existe e é ignorado; `del` apaga |

Testado nos dois shells: em ambos não sobra nenhum `.o` nem o executável. O
custo é uma mensagem de "comando não encontrado" na tela, que é ruído, mas
honesto.

**Como isso já esteve errado.** Uma versão anterior escolhia o comando por um
`ifeq` que olhava só a variável `OS`. Como `OS` vale `Windows_NT` mesmo dentro
do Git Bash, ela escolhia `del`, que não existe ali: o comando falhava, o `-`
engolia o erro, e **os arquivos ficavam onde estavam** — falha silenciosa, que
é pior que falhar. A correção seguinte somou um segundo `ifeq` para distinguir
o Git Bash, o que resolvia mas custava 13 linhas. A versão atual não tem
condicional nenhum: são duas linhas de comando.

O `clean` apaga `$(OBJS)`, e não `*.o`: assim ele não leva junto algum `.o` de
outra coisa que esteja na pasta.

## Como usar

| O que | Linux ou Git Bash | Windows, PowerShell |
|---|---|---|
| compilar | `make` | `mingw32-make` |
| executar | `make run`, ou `./tp1` | `mingw32-make run`, ou `.\tp1.exe` |
| limpar | `make clean` | `mingw32-make clean` |

## Onde ele difere do modelo da disciplina

| No modelo | Aqui | Por quê |
|---|---|---|
| `CFLAGS = -Wall -Iinclude` | mais `-Wextra -std=c99 -g` | mais rigor na checagem, a versão da linguagem fixada, e o `-g` que o próprio documento recomenda para o `gdb` e o `valgrind` |
| sem `-lm` | `LDLIBS = -lm` | o programa chama `sqrt` |
| `BIN = programa` | `BIN = tp1` | `programa` é o nome de exemplo do documento |
| `SRCS = $(SRC_DIR)/main.c ...` | `SRCS = main.c ...` | a especificação pede o programa principal **separado** dos TADs, então o `main.c` fica na raiz e o `src/` guarda só os TADs. É por isso que existem duas regras de padrão em vez de uma |
| `OBJS = $(SRCS:.c=.o)` | lista escrita por extenso | com o `main.c` na raiz, o `$(SRCS:.c=.o)` deixaria os objetos de `src/` dentro de `src/`, misturando fonte e compilado. Assim os seis ficam na raiz, e o `clean` fica simples nos dois shells |
| `run` passa o arquivo por argumento | `run` só executa | o programa pergunta o caminho no menu |
| `clean: rm -f ...` | `rm` e `del` | para funcionar também no `cmd` e no PowerShell |
| sem `.PHONY` | `.PHONY: all run clean` | para um arquivo chamado `clean` não travar a tarefa |

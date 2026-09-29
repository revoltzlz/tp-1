# `Makefile`

Descreve como compilar o projeto. A especificação exige que o Makefile tenha
"um comentário explicando como compilar o programa", e o comentário do topo
cobre compilar, executar e limpar.

## As variáveis

```make
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDLIBS = -lm

ALVO = tp1
OBJS = main.o pokemon.o pokelista.o treinador.o pokecenter.o missao.o
```

| Variável | O que é |
|---|---|
| `CC` | o compilador |
| `CFLAGS` | as opções de compilação |
| `LDLIBS` | as bibliotecas da ligação |
| `ALVO` | o nome do executável |
| `OBJS` | os seis arquivos-objeto |

### As opções, uma a uma

| Opção | Para quê |
|---|---|
| `-Wall` | liga os avisos comuns do compilador |
| `-Wextra` | liga mais avisos, que o `-Wall` não cobre |
| `-std=c99` | fixa o padrão da linguagem em C99, para o compilador não aceitar extensões que não seriam aceitas em outro ambiente |
| `-Iinclude` | diz ao gcc para procurar os cabeçalhos dentro da pasta `include/`. É o que faz `#include "pokemon.h"` funcionar mesmo o arquivo estando em `include/pokemon.h` |
| `-lm` | liga a biblioteca matemática, necessária por causa do `sqrt` |

O projeto compila com **zero avisos** com essas opções, e também acrescentando
`-pedantic`.

### Por que o `-lm` fica no fim

```make
$(ALVO): $(OBJS)
	$(CC) $(CFLAGS) -o $(ALVO) $(OBJS) $(LDLIBS)
```

O `-lm` vem **depois** dos arquivos-objeto. O ligador processa o que recebe da
esquerda para a direita: quando ele lê `missao.o` e encontra uma chamada a
`sqrt` que não sabe resolver, guarda essa pendência; quando chega em `-lm`,
procura ali e resolve. Se o `-lm` viesse antes, o ligador leria a biblioteca
quando ainda não havia nenhuma pendência, não pegaria nada dela, e depois
reclamaria de `undefined reference to sqrt`.

## As regras

```make
.PHONY: all clean

all: $(ALVO)

$(ALVO): $(OBJS)
	$(CC) $(CFLAGS) -o $(ALVO) $(OBJS) $(LDLIBS)

main.o: main.c include/missao.h include/pokecenter.h ...
	$(CC) $(CFLAGS) -c main.c -o main.o

pokemon.o: src/pokemon.c include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c src/pokemon.c -o pokemon.o

...
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

### Por que as dependências listam os cabeçalhos

```make
pokelista.o: src/pokelista.c include/pokelista.h include/conexao.h \
             include/pokemon.h include/coordenadas.h
```

O make regera um alvo quando **algum** dos arquivos de que ele depende está
mais novo que ele. Listando os cabeçalhos, mudar `include/pokemon.h` faz o make
recompilar todos os `.o` que o incluem — direta ou indiretamente. Sem isso, o
make só olharia o `.c`, e uma mudança num `.h` não recompilaria nada: o
programa seria ligado com objetos desatualizados, que é um erro difícil de
entender, porque o código-fonte na tela não corresponde ao executável.

A barra invertida no fim da linha continua a lista na linha seguinte.

Está conferido que funciona: tocando `include/pokemon.h` ou
`include/coordenadas.h`, os seis objetos são recompilados.

### `.PHONY`

```make
.PHONY: all clean
```

Diz ao make que `all` e `clean` são nomes de **tarefa**, e não de arquivo. Sem
isso, se alguém criasse um arquivo chamado `clean` na pasta, o make acharia que
a tarefa já está feita e não faria nada.

## A limpeza

```make
clean:
	-rm -f $(ALVO) $(ALVO).exe *.o
	-del /Q /F $(ALVO).exe *.o
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

## Como usar

| O que | Linux ou Git Bash | Windows, PowerShell |
|---|---|---|
| compilar | `make` | `mingw32-make` |
| executar | `./tp1` | `.\tp1.exe` |
| limpar | `make clean` | `mingw32-make clean` |

## O que não está aqui

A versão anterior usava uma **regra de padrão**:

```make
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
```

Ela diz "qualquer `x.o` é obtido compilando `src/x.c`", com `$<` sendo o
primeiro arquivo da lista de dependências e `$@` sendo o alvo. Funciona e é
mais curto, mas exige conhecer três coisas a mais — o `%`, o `$<` e o `$@` — e
as dependências dos cabeçalhos ficavam em linhas separadas, longe da regra. Foi
trocada por uma regra explícita por módulo: fica mais longo, mas cada linha diz
exatamente de que arquivos aquele objeto depende e como ele é gerado.

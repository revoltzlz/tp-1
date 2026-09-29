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
ifeq ($(OS),Windows_NT)
    ifeq ($(MSYSTEM),)
        LIMPAR = del /Q /F $(ALVO).exe *.o
    else
        LIMPAR = rm -f $(ALVO) $(ALVO).exe *.o
    endif
else
    LIMPAR = rm -f $(ALVO) $(ALVO).exe *.o
endif

clean:
	-$(LIMPAR)
```

O comando de apagar depende de **em qual shell o make está rodando**, e não só
do sistema operacional. No Windows o make pode rodar sob o `cmd` ou o
PowerShell, onde existe `del` e não existe `rm`, ou sob o `sh` do Git Bash, onde
é o contrário. Duas variáveis de ambiente respondem isso:

| Variável | Vale |
|---|---|
| `OS` | `Windows_NT` no Windows; vazia no Linux |
| `MSYSTEM` | `MINGW64` dentro do Git Bash; vazia nos outros shells |

Combinando as duas:

| Ambiente | `OS` | `MSYSTEM` | Comando |
|---|---|---|---|
| Linux | vazia | vazia | `rm` |
| Windows, Git Bash | `Windows_NT` | `MINGW64` | `rm` |
| Windows, cmd ou PowerShell | `Windows_NT` | vazia | `del` |

**Por que isso não foi simplificado.** A versão anterior olhava só a variável
`OS` e escolhia `del` sempre que estava no Windows. Rodando
`mingw32-make clean` de dentro do Git Bash, o `del` não existia, o comando
falhava, o `-` do começo da linha fazia o make seguir em frente, e **os
arquivos ficavam onde estavam** — uma falha silenciosa, que é pior do que
falhar. Um `ifeq` aninhado é mais código do que se gostaria, mas a explicação é
de uma frase e o resultado é correto nos três ambientes, o que foi testado.

O `-` antes do comando manda o make ignorar o erro. Serve para
`make clean` funcionar quando não há nada para apagar, em vez de reclamar que
não encontrou os arquivos.

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

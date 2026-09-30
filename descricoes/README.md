# TP1 — Resgate de Pokémon com listas encadeadas

Documentação do trabalho. Um arquivo por arquivo do projeto, com as structs, as
funções, os custos e as decisões de projeto. É material para estudar e para
explicar na entrevista.

Esta pasta **não** vai no `.zip` da entrega: a especificação pede o
código-fonte e os slides, não documentação.

## Índice

| Arquivo | Descreve |
|---|---|
| [coordenadas.md](coordenadas.md) | `include/coordenadas.h` — o par (X,Y) e os limites do mapa |
| [pokemon.md](pokemon.md) | `include/pokemon.h` e `src/pokemon.c` — o TAD Pokémon |
| [pokelista.md](pokelista.md) | `include/conexao.h`, `include/pokelista.h` e `src/pokelista.c` — o TAD PokeLista, com desenhos das operações |
| [treinador.md](treinador.md) | `include/treinador.h` e `src/treinador.c` — o TAD Treinador |
| [pokecenter.md](pokecenter.md) | `include/pokecenter.h` e `src/pokecenter.c` — o TAD Centro de Pesquisa |
| [missao.md](missao.md) | `include/missao.h` e `src/missao.c` — o sistema de controle da missão |
| [main.md](main.md) | `main.c` — o programa principal |
| [makefile.md](makefile.md) | `Makefile` |
| [testes.md](testes.md) | a pasta `testes/`: o que cada arquivo testa e o resultado |
| [exemplo.md](exemplo.md) | o exemplo do PDF traçado à mão e comparado com o programa |
| [requisitos.md](requisitos.md) | cada exigência do PDF, com a evidência de onde foi atendida |
| [entrevista.md](entrevista.md) | perguntas prováveis com resposta curta |
| [revisao.md](revisao.md) | o que a revisão final encontrou e mudou |

Os slides ficam em [`../apresentacao/`](../apresentacao/), com um README
próprio dizendo o que falta preencher antes de entregar.

## Mapa dos arquivos

```
tp1 aedes/
├── Makefile
├── main.c                 programa principal: srand e o menu
├── include/
│   ├── coordenadas.h      o tipo cord (X,Y) e os limites do mapa
│   ├── pokemon.h          TAD Pokémon
│   ├── conexao.h          a célula da lista encadeada
│   ├── pokelista.h        TAD PokeLista
│   ├── treinador.h        TAD Treinador
│   ├── pokecenter.h       TAD Centro de Pesquisa
│   └── missao.h           sistema de controle da missão
├── src/
│   ├── pokemon.c
│   ├── pokelista.c        onde está toda a memória dinâmica do projeto
│   ├── treinador.c
│   ├── pokecenter.c
│   └── missao.c
├── testes/                arquivos de entrada e o script que roda tudo
├── apresentacao/          os slides, em .pptx e em .pdf
└── descricoes/            esta pasta
```

## Dependência entre os módulos

Cada `.h` inclui só o de baixo. A cadeia sobe numa linha e nunca volta, então
não existe inclusão circular:

```
coordenadas.h
     ↓
 pokemon.h
     ↓
 conexao.h        (a célula guarda um Pokemon)
     ↓
pokelista.h
     ↓
treinador.h       (o treinador tem uma PokeLista)
     ↓
pokecenter.h      (o Centro tem duas PokeLista e recebe um Treinador)
     ↓
  missao.h
     ↓
  main.c
```

Cada `.c` inclui **apenas o seu próprio `.h`**. Como o `.h` já traz os de
baixo, isso é suficiente, e evita que um módulo de baixo passe a depender de um
de cima por acidente.

Os quatro TADs que a especificação pede são **Pokémon**, **PokeLista**,
**Treinador** e **Centro de Pesquisa**. O `missao.h`/`missao.c` não é um TAD: é
o módulo do sistema de controle da missão, separado dos TADs e do programa
principal, como a especificação exige. O `coordenadas.h` e o `conexao.h` não
têm `.c` porque só declaram tipos, sem operações.

## Como compilar e rodar

### Git Bash (Windows) ou Linux

```bash
mingw32-make          # no Windows
make                  # no Linux
./tp1                 # ou ./tp1.exe no Windows
bash testes/rodar_testes.sh
mingw32-make clean
```

### PowerShell (Windows)

```powershell
mingw32-make
.\tp1.exe
mingw32-make clean
```

### Acentos no terminal do Windows

As mensagens do programa têm acento e os arquivos-fonte estão em UTF-8. O
terminal do Windows não usa UTF-8 por padrão, então pode aparecer `DragÃ£o` no
lugar de `Dragão`. Isso é do terminal, não do programa. Antes de executar,
rode:

```
chcp 65001
```

No Linux e no Git Bash não é preciso fazer nada.

### Se o programa não executar no Windows

Se aparecer `Permission denied` ou o programa não imprimir nada, o motivo mais
provável é o **Smart App Control** do Windows 11, que bloqueia executáveis sem
assinatura digital — inclusive os que o gcc acabou de gerar. Para confirmar,
rode no PowerShell:

```powershell
Get-WinEvent -LogName Microsoft-Windows-CodeIntegrity/Operational -MaxEvents 5
```

Se aparecer "Smart App Control Block", as alternativas são:

1. **Desligar o Smart App Control**, em Configurações → Privacidade e segurança
   → Segurança do Windows → Controle de aplicativo e navegador. **Atenção: é
   irreversível — uma vez desligado, só volta reinstalando o Windows.**
2. **Usar o WSL.** O Ubuntu já está instalado nesta máquina, mas não inicia
   porque a virtualização está desligada no firmware. É preciso ativar Intel
   VT-x ou AMD-V no BIOS e rodar `wsl --install --no-distribution`. É
   reversível, e de quebra permite usar o `valgrind`.
3. **Compilar e rodar em outra máquina**, num laboratório ou no computador de
   um colega.

O `testes/rodar_testes.sh` contorna isso automaticamente: quando o binário é
bloqueado, ele recompila com outras opções de otimização até um deles passar.
Funciona, mas depende de sorte — não convém contar com isso no dia da
entrevista.

## O que a entrada precisa ter

```
<nome do treinador 1> <Pokébolas iniciais>
<nome do treinador 2> <Pokébolas iniciais>
<quantidade n de Pokémon fugitivos>
<número na Pokédex> <nome> <tipo> <X> <Y>      (n linhas assim)
```

Exemplo, que é o `testes/oficiais/teste1.txt`:

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

Os treinadores não têm coordenada no arquivo: a especificação fixa a posição
inicial dos dois em (0,0). Os Pokémon não têm Id no arquivo: o programa atribui
o Id na ordem de leitura (ver [pokemon.md](pokemon.md)).

## Saída

A saída no terminal detalha a missão resgate por resgate. Ao terminar, o Centro
de Pesquisa grava `relatorio.txt` na pasta de onde o programa foi executado,
com uma linha por Pokémon recuperado.

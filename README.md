# TP1 — Resgate de Pokémon

Trabalho Prático 1 de Algoritmos e Estruturas de Dados I (CCF 211), UFV
Campus Florestal.

Uma explosão no laboratório fez vários Pokémon fugirem do Centro de Pesquisa.
Dois treinadores saem para resgatá-los: a cada Pokémon vai o que estiver mais
perto e, em caso de empate, o de menor identificador. Cada captura gasta uma
Pokébola; sem nenhuma, o treinador volta ao Centro, entrega o que capturou e
recebe uma carga nova. No fim, o Centro grava um relatório em `relatorio.txt`.

Todas as coleções de Pokémon são **listas encadeadas com célula cabeça**, como
a especificação exige. Nenhuma é vetor.

## Compilar e executar

```sh
make            # ou mingw32-make, no Windows
./tp1           # ou .\tp1.exe, no PowerShell
```

O programa abre um menu: a opção **1** lê os dados de um arquivo, a **2** pede
os dados pelo teclado e a **0** encerra. Há arquivos de entrada de exemplo em
`testes/`, e os dois oficiais em `testes/oficiais/`.

```sh
printf '1\ntestes/oficiais/teste1.txt\n0\n' | ./tp1
```

Para apagar o executável e os arquivos-objeto: `make clean`.

## Organização

| Pasta | O que tem |
|---|---|
| `include/` | os cabeçalhos: um por TAD, mais `coordenadas.h`, `conexao.h` e `missao.h` |
| `src/` | as implementações |
| `testes/` | 32 arquivos de entrada e o `rodar_testes.sh`, que roda tudo |
| `descricoes/` | a documentação: um arquivo por arquivo do projeto |
| `apresentacao/` | os slides, em `.pptx` e em `.pdf` |

Os quatro TADs que a especificação pede são **Pokémon**, **PokeLista**,
**Treinador** e **Centro de Pesquisa**, cada um usando só o de baixo:

```
Pokémon → PokeLista → Treinador → Centro de Pesquisa
```

Acima deles fica o módulo da missão, que conduz o resgate, e acima dele o
`main.c`, que só prepara o sorteio e chama o menu.

## Testes

```sh
bash testes/rodar_testes.sh
```

São 72 verificações. Entre elas, a saída do exemplo do enunciado é comparada
caractere por caractere com `diff`, e a contagem de `malloc` e `free` confere
se nada vazou. O script usa o `valgrind` automaticamente onde ele existir.

## Entrega

```sh
bash gerar_zip.sh "Aluno1" "Matricula1" "Aluno2" "Matricula2"
```

Monta o `.zip` com o Makefile, o `main.c`, os `include/` e `src/` e o
`apresentacao/slides.pdf`, e depois extrai o pacote numa pasta temporária,
compila do zero e roda o exemplo, para conferir que está completo.

## Onde ler mais

- [`descricoes/`](descricoes/) — um arquivo por módulo, com as structs, as
  funções, os custos e as decisões de projeto
- [`descricoes/requisitos.md`](descricoes/requisitos.md) — cada exigência do
  enunciado com a evidência de onde foi atendida
- [`descricoes/entrevista.md`](descricoes/entrevista.md) — perguntas prováveis
  com resposta curta
- [`apresentacao/README.md`](apresentacao/README.md) — o que falta preencher
  nos slides antes de entregar

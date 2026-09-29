# A pasta `testes/`

Os arquivos de entrada e o script que roda tudo. A especificação pede que a
dupla crie os próprios casos de teste, "tanto interativos quanto por arquivo", e
avisa que "novos testes por arquivo serão utilizados no dia da entrevista" — daí
a preocupação em cobrir entradas estranhas, e não só as que dão certo.

## Como rodar

```bash
bash testes/rodar_testes.sh
```

Resultado da última execução: **72 verificações, 72 passaram.**

## Como o script funciona

O programa não tem modo de teste nenhum dentro dele: a única forma de usá-lo é
pelo menu. Então todo teste alimenta o menu pela entrada padrão, como se
alguém estivesse digitando:

```bash
printf '1\narquivo.txt\n0\n' | ./tp1
#        1  → executar a missão a partir de um arquivo
#           → o caminho do arquivo
#        0  → sair
```

Duas coisas exigem uma **cópia temporária** do projeto, que o script cria e
apaga:

1. **Comparar a saída com o exemplo da especificação** exige saber quanto a
   recarga vai sortear. O script copia o projeto para `tmp_testes/copia` e
   troca, só na cópia, a linha do sorteio pelo valor do exemplo (2). O código
   entregue continua sorteando.
2. **Contar `malloc` e `free`** exige interceptar as duas funções. O script
   copia o projeto para `tmp_testes/memoria` e injeta um cabeçalho com
   `gcc -include`, só no `pokelista.c`, que é o único arquivo do projeto que
   aloca memória. Nenhum arquivo do projeto é alterado.

O script também confere que os dois builds compilam com **zero avisos** usando
exatamente as opções do Makefile.

### O contorno do Smart App Control

O Smart App Control do Windows 11 bloqueia executáveis sem assinatura digital,
inclusive os que o gcc acabou de gerar, e a decisão depende do conteúdo do
binário — alguns passam, outros não. O script tenta até 14 combinações de
opções de otimização até um binário conseguir rodar. As opções de otimização
não mudam o comportamento do programa, só o código de máquina gerado.

Se nenhuma passar, o script **para** e explica o problema, em vez de deixar os
testes seguintes darem resultado falso. Isso é importante: numa versão anterior
do script, a mensagem `Permission denied` do próprio shell era confundida com a
mensagem de erro do programa, e 30 testes apareciam como "passou" sem que nada
tivesse rodado.

## As cinco seções do script

| Seção | O que verifica |
|---|---|
| 1 | a saída e o relatório do exemplo, comparados com `diff`, e as regras da missão |
| 2 | 19 casos válidos, cada um com as invariantes |
| 3 | 16 casos inválidos, que precisam ser recusados |
| 4 | o menu e o modo interativo |
| 5 | memória |

### O que cada caso válido precisa cumprir

Em todo arquivo válido o script confere cinco coisas:

1. a missão chega ao fim (a moldura `MISSÃO CONCLUÍDA` aparece);
2. a lista de fugitivos termina **vazia** (o aviso "ainda há N Pokémon" não
   aparece);
3. nenhuma quantidade de Pokébolas aparece negativa;
4. **nenhuma distância impressa é negativa, `nan` ou `inf`**;
5. o relatório tem exatamente **n** Pokémon.

A quarta verificação foi acrescentada na revisão final, e é a que faltava: um
estouro no cálculo do quadrado da distância produzia `nan` e mandava a missão
para o treinador mais distante, e as outras quatro invariantes continuavam
todas válidas — o teste passava com o programa errado. Ver
[revisao.md](revisao.md).

---

## Os arquivos de entrada

### Oficiais, disponibilizados pela professora

| Arquivo | n | O que tem de especial |
|---|---|---|
| `oficiais/teste1.txt` | 5 | é o exemplo da especificação. Oráculo completo: a saída tem de bater com `saida_exemplo_esperada.txt` e o relatório com `relatorio_exemplo_esperado.txt` |
| `oficiais/teste2.txt` | 20 | **quatro Pikachus com o mesmo número de Pokédex `025`**. É o que prova que o Id não pode ser o número da Pokédex |

Os dois estão em CRLF e o `teste1.txt` não tem quebra de linha na última linha.

### Casos válidos

| Arquivo | n | O que testa |
|---|---|---|
| `entrada_exemplo.txt` | 5 | o mesmo exemplo, em LF, para conferir que o fim de linha não muda nada |
| `crlf_windows.txt` | 5 | fim de linha do Windows (`\r\n`) |
| `bom_utf8.txt` | 5 | a marca UTF-8 de três bytes que o Bloco de Notas põe no começo do arquivo |
| `empate_origem.txt` | 3 | empate de distância, com os dois treinadores na origem |
| `pokemon_na_origem.txt` | 2 | Pokémon exatamente em (0,0), onde os treinadores estão |
| `uma_pokebola.txt` | 4 | treinadores com 1 Pokébola: recarrega depois de cada captura |
| `zero_pokebolas.txt` | 3 | um treinador começa com 0 Pokébolas |
| `ambos_zero_pokebolas.txt` | 2 | os dois começam com 0 |
| `um_pokemon.txt` | 1 | o primeiro resgate já é o último |
| `zero_pokemon.txt` | 0 | nenhum fugitivo: a missão termina sem captura nenhuma |
| `pokedex_repetida.txt` | 4 | quatro Pokémon com o mesmo número de Pokédex |
| `coordenadas_negativas.txt` | 3 | só coordenadas negativas |
| `coordenadas_no_limite.txt` | 3 | coordenadas em ±1.000.000, o limite do mapa. É o caso que exercita o `long long`: a soma dos quadrados chega a 8×10¹² |
| `nomes_maximos.txt` | 2 | nome de treinador com 29 caracteres, nome de Pokémon com 29 e tipo com 19 — exatamente o tamanho dos vetores menos um |
| `espacos_e_linhas_extras.txt` | 3 | espaços, tabulações e linhas em branco a mais entre os campos |
| `mesma_coordenada.txt` | 4 | vários Pokémon na mesma coordenada: depois da primeira captura a distância do treinador escolhido é zero |
| `quinhentos_pokemon.txt` | 500 | escala. Roda em menos de um segundo e faz exatamente 1504 alocações |

### Casos inválidos

Cada um precisa ser recusado com uma mensagem que começa com `Erro:`, e o
programa precisa voltar ao menu — nunca travar. Estas são as mensagens reais,
como o programa as imprime:

| Arquivo | Mensagem |
|---|---|
| `erro_vazio.txt` | `Erro: nao foi possivel ler o nome do treinador 1.` |
| `erro_so_espacos.txt` | `Erro: nao foi possivel ler o nome do treinador 1.` |
| `erro_incompleto.txt` | `Erro: nao foi possivel ler o nome do treinador 2.` |
| `erro_pokebolas_texto.txt` | `Erro: nao foi possivel ler a quantidade de Pokebolas de Rosa.` |
| `erro_pokebolas_negativas.txt` | `Erro: quantidade de Pokebolas invalida para Rosa (-3).` |
| `erro_pokebolas_gigante.txt` | `Erro: quantidade de Pokebolas invalida para Rosa (1661992959).` |
| `erro_quantidade_texto.txt` | `Erro: nao foi possivel ler a quantidade de Pokemon fugitivos.` |
| `erro_fugitivos_negativos.txt` | `Erro: quantidade de Pokemon fugitivos invalida (-4).` |
| `erro_campos_faltando.txt` | `Erro: dados incompletos do Pokemon 2 de 2.` |
| `erro_quantidade_maior.txt` | `Erro: dados incompletos do Pokemon 3 de 5.` |
| `erro_coordenada_texto.txt` | `Erro: dados incompletos do Pokemon 1 de 1.` |
| `erro_pokedex_negativa.txt` | `Erro: numero na Pokedex invalido para o Pokemon Axew (-610).` |
| `erro_pokedex_gigante.txt` | `Erro: numero na Pokedex invalido para o Pokemon Axew (1661992959).` |
| `erro_coordenada_fora_do_mapa.txt` | `Erro: o Pokemon Bulbasaur esta em (2000000000,2000000000), fora do mapa, que vai de -1000000 a 1000000 nos dois eixos.` |
| `nomes_longos_demais.txt` | `Erro: nao foi possivel ler a quantidade de Pokebolas de TreinadorComNome...` |
| (arquivo inexistente) | `Erro: nao foi possivel abrir o arquivo "nao_existe.txt".` |

Três desses merecem explicação:

**`erro_pokebolas_gigante.txt`** tem `Rosa 99999999999999999999`. Esse número
não cabe num `int`, e o `%d` do `scanf` guarda um valor truncado sem avisar:
1.661.992.959. A checagem de "não pode ser negativo" não pegaria, porque o valor
truncado é positivo. É o teto `MAX_QUANTIDADE` que o recusa.

**`erro_coordenada_fora_do_mapa.txt`** tem coordenadas de 2 bilhões. É o teste
que guarda a correção do estouro da distância: o programa tem de **recusar** o
arquivo, não calcular nada.

**`nomes_longos_demais.txt`** tem um nome de treinador com 50 caracteres. O
`%29s` lê os primeiros 29 e o resto da palavra fica na entrada; o `%d` seguinte
tenta ler esse resto como número e falha. O importante é que não há escrita fora
do vetor: o programa recusa o arquivo com mensagem.

### Oráculos

Não são entradas: são os arquivos com os quais a saída é comparada.

| Arquivo | O que é |
|---|---|
| `saida_exemplo_pdf.txt` | a transcrição **fiel** do exemplo da especificação, conferida contra quatro extrações independentes do PDF |
| `saida_exemplo_esperada.txt` | o mesmo, com a errata do bloco final corrigida. É contra este que o programa é comparado |
| `relatorio_exemplo_pdf.txt` | a transcrição fiel do relatório do exemplo |
| `relatorio_exemplo_esperado.txt` | o mesmo, sem o recuo de um espaço por linha |

O comando abaixo mostra exatamente qual é a diferença entre a transcrição fiel e
o que o programa produz, que é a documentação da errata:

```bash
diff testes/saida_exemplo_pdf.txt testes/saida_exemplo_esperada.txt
```

A explicação está em [exemplo.md](exemplo.md).

---

## O modo interativo

Testado com a entrada digitada, simulada com pipe:

```bash
printf '2\nRosa 2\nNate 2\n5\n610 Axew Dragão 7 7\n...\n0\n' | ./tp1
```

O script confere que a missão chega ao fim, que o relatório sai com os 5
Pokémon, que a lista de fugitivos registrados é impressa para conferência, e
que ao voltar do modo interativo o menu **não** reclama de uma opção inválida
que ninguém digitou.

Também testa o erro no meio do modo interativo (`Rosa -5`): o programa avisa,
volta ao menu e sai limpo pela opção 0.

## O menu

| Teste | Resultado esperado |
|---|---|
| opção `9` | `Opção inválida.` e o menu continua |
| letra no lugar de número | `Entrada encerrada.` e o programa termina, sem laço infinito |
| entrada que termina vazia | o programa encerra sem erro |
| caminho de arquivo inexistente | mensagem de erro e volta ao menu |

O caso da letra é o que justifica a forma da leitura da opção: `scanf("%d")`
não consome o caractere que não conseguiu converter, então insistir no laço
faria o menu girar para sempre imprimindo a mesma mensagem. O programa encerra
com mensagem.

## Memória

Não há `valgrind` nesta máquina — ele existe no Linux, e o WSL não inicia aqui
porque a virtualização está desligada no firmware. O script usa `valgrind`
automaticamente onde ele existir; onde não existe, conta `malloc` e `free`.

A contagem é feita na cópia temporária, com um cabeçalho injetado por
`gcc -include` que troca `malloc` e `free` por versões que incrementam um
contador antes de chamar as originais. Ao terminar o programa, uma função
registrada com `atexit` imprime os dois números.

Resultado em sete arquivos, incluindo os de erro: **`malloc` igual a `free` em
todos**. Os números absolutos:

| Arquivo | n | Alocações | Liberações | Conta |
|---|---|---|---|---|
| `oficiais/teste2.txt` | 20 | 64 | 64 | 4 + 3×20 = 64 |
| `quinhentos_pokemon.txt` | 500 | 1504 | 1504 | 4 + 3×500 = 1504 |

A conta `4 + 3n` é: quatro células cabeça (as duas listas do Centro e a lista de
cada treinador) mais três células por Pokémon — uma no registro da fuga, uma na
captura e uma na entrega. Ver [pokelista.md](pokelista.md).

**O que esse método não cobre.** Contar `malloc` e `free` prova que nada vazou,
mas não detecta apontador pendurado, uso de memória depois do `free` nem escrita
fora de um vetor. Para isso seria preciso o `valgrind`. Vale rodá-lo uma vez
antes da entrevista, num Linux.

## Teste direto do TAD PokeLista

O fluxo da missão não exercita todos os casos da lista — em particular, nunca
remove o único elemento de uma lista. Na revisão final foi feito um teste
direto, num programa temporário, que cobriu:

| Caso | Resultado |
|---|---|
| buscar, remover e remover o primeiro em lista **vazia** | os três devolvem 0, sem quebrar |
| imprimir lista vazia | `(nenhum Pokémon na lista)` |
| remover o **único** elemento | `ultimo` volta a apontar para a célula cabeça |
| inserir depois disso | funciona, e o elemento é encontrado pela busca |
| remover um Id que **não existe** | devolve 0 e a lista não muda |
| remover do **meio** | os vizinhos ficam ligados |
| remover o **primeiro** | mesmo código, sem caso especial |
| remover o **último** | `ultimo` passa a apontar para o anterior |
| inserir depois de remover o último | a ordem fica correta (2, 4, 6) |
| liberar | os apontadores ficam nulos |

A invariante `ultimo->prox == NULL` foi conferida em todos os estados
intermediários. O programa de teste era temporário e não ficou no projeto.

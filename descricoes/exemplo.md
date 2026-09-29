# O exemplo do PDF, traçado à mão

Este arquivo refaz o exemplo da especificação passo a passo, com as contas na
mão, e compara três coisas: o traço, a saída que o PDF mostra e a saída que o
programa produz.

## A entrada

O bloco do arquivo de entrada, no PDF, está em três colunas: os dados de um
lado e os comentários explicativos do outro. Copiar e colar embaralha as
colunas e junta as coordenadas (`77` no lugar de `7 7`). A extração que segue a
ordem do conteúdo (`pdftotext -raw`) mostra o bloco alinhado, com os
comentários ao lado:

```
Rosa 2 /*Nome dos treinadores e qnt*
Nate 2 /*inicial de Pokebolas*/
5 /*Qnt de pokémon fugitivos*/
610 Axew Dragão 7 7 /*Informações dos pokémon*/
657 Frogadier Água 10 7
387 Turtwig Folha 1 1
715 Noivern Voador -10 -10
197 Umbreon Noturno 5 7
```

Tirando os comentários, sobra o arquivo:

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

Isso é **idêntico** ao `testes/oficiais/teste1.txt`, o arquivo que a professora
disponibilizou no Moodle. Ou seja: a leitura do bloco de três colunas foi
confirmada por um caminho independente.

## O traço

Os dois treinadores começam em (0,0), cada um com 2 Pokébolas. A distância é
comparada pelo **quadrado**, `dx² + dy²`, que é inteiro e exato; a raiz aparece
só na hora de imprimir.

| # | Alvo | Está em | Rosa em | Nate em | d² Rosa | d² Nate | Rosa | Nate | Vai | Pokébolas depois | Volta e recarrega? |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | Axew | (7,7) | (0,0) | (0,0) | 7²+7² = **98** | 7²+7² = **98** | 9.90 | 9.90 | **Rosa** (empate → menor id) | Rosa: 1 | não |
| 2 | Frogadier | (10,7) | (7,7) | (0,0) | 3²+0² = **9** | 10²+7² = **149** | 3.00 | 12.21 | **Rosa** | Rosa: 0 | **sim** → volta a (0,0), entrega Axew e Frogadier, recebe 2 |
| 3 | Turtwig | (1,1) | (0,0) | (0,0) | 1²+1² = **2** | 1²+1² = **2** | 1.41 | 1.41 | **Rosa** (empate → menor id) | Rosa: 1 | não |
| 4 | Noivern | (-10,-10) | (1,1) | (0,0) | 11²+11² = **242** | 10²+10² = **200** | 15.56 | 14.14 | **Nate** | Nate: 1 | não |
| 5 | Umbreon | (5,7) | (1,1) | (-10,-10) | 4²+6² = **52** | 15²+17² = **514** | 7.21 | 22.67 | **Rosa** | Rosa: 0 | **não** — era o último fugitivo |

As raízes, conferidas uma a uma:

| d² | Raiz | Com `%.2f` | PDF |
|---|---|---|---|
| 98 | 9.899494... | 9.90 | 9.90 |
| 9 | 3.0 | 3.00 | 3.00 |
| 149 | 12.206555... | 12.21 | 12.21 |
| 2 | 1.414213... | 1.41 | 1.41 |
| 242 | 15.556349... | 15.56 | 15.56 |
| 200 | 14.142135... | 14.14 | 14.14 |
| 52 | 7.211102... | 7.21 | 7.21 |
| 514 | 22.671568... | 22.67 | 22.67 |

**As dez distâncias do traço batem com as dez do PDF.** Nenhuma divergência.

## O retorno final e o relatório

Depois do último resgate, os dois voltam ao Centro e entregam, na ordem do
identificador: primeiro a Rosa (id 1), depois o Nate (id 2). Cada um entrega na
ordem em que capturou.

| Momento | Quem entrega | O que entrega | Lista de recuperados fica |
|---|---|---|---|
| passo 2, sem Pokébolas | Rosa | Axew, Frogadier | 610, 657 |
| retorno final | Rosa | Turtwig, Umbreon | 610, 657, 387, 197 |
| retorno final | Nate | Noivern | 610, 657, 387, 197, **715** |

Relatório pelo traço: `610, 657, 387, 197, 715`
Relatório no PDF: `610, 657, 387, 197, 715`

**Batem.** E essa ordem, que parece estranha à primeira vista, é o que confirma
três decisões de uma vez:

1. o teste "acabaram os fugitivos?" vem **antes** de "ficou sem Pokébolas?" —
   se fosse o contrário, a Rosa recarregaria depois do Umbreon e apareceria uma
   recarga a mais na saída;
2. a entrega sai na **ordem de captura** (o treinador tira sempre o primeiro da
   lista dele e o Centro insere no fim da lista de recuperados);
3. no retorno final, **a Rosa entrega antes do Nate**, por ter o menor
   identificador. É isso que põe o 715 no fim.

## A posição do Nate, passo a passo

Havia uma dúvida anotada de que o PDF mostraria 17.72 como distância do Nate
até o Umbreon. **Não mostra.** O valor no PDF é 22.67, e foi confirmado em
quatro extrações independentes do arquivo (`-layout`, padrão, `-raw` e
`-table`): as dez distâncias que aparecem no PDF são exatamente 9.90, 9.90,
3.00, 12.21, 1.41, 1.41, 15.56, 14.14, 7.21 e 22.67. A sequência `17.72` não
existe em nenhum lugar do texto do PDF.

De onde 17.72 poderia vir: √314 = 17.720045, e 314 = 5² + 17², que é a
distância de **(10,−10)** até (5,7). O Nate nunca está em (10,−10). Pelo traço,
a posição dele é:

| Momento | Nate está em | Por quê |
|---|---|---|
| início | (0,0) | posição inicial exigida pela especificação |
| passos 1, 2 e 3 | (0,0) | nunca foi escolhido, então não se moveu |
| passo 4 | (−10,−10) | foi escolhido e se movimentou até o Noivern |
| passo 5 | (−10,−10) | não foi escolhido, continua lá |

De (−10,−10) até (5,7): dx = 15, dy = 17, d² = 225 + 289 = 514, √514 = 22.67.
**O PDF está certo e o programa está certo.** Não havia erro para corrigir, e
nada foi alterado por causa disso.

## Comparação com a saída do programa

Para comparar de forma exata, o projeto foi copiado para uma pasta temporária e
**só nessa cópia** a linha do sorteio foi trocada pelo valor da recarga do
exemplo (2). O código entregue continua sorteando e não tem nada de teste
dentro dele.

O `diff` entre a transcrição fiel do PDF (`testes/saida_exemplo_pdf.txt`) e a
saída do programa deu **duas** diferenças, as duas no fim do arquivo:

```
104,105c104
<
< ​  MISSÃO CONCLUÍDA
---
>             MISSÃO CONCLUÍDA
```

Esta é a errata do PDF. No bloco final, e **só nele**, o PDF traz uma linha em
branco a mais entre a linha de `=` e o título, e no lugar da indentação traz um
caractere invisível (`U+200B`, espaço de largura zero) seguido de dois espaços.
Os outros três blocos de moldura do exemplo não têm nem a linha extra nem o
caractere invisível, e têm indentação de 18, 12 e 7 espaços. É artefato de
formatação do editor de texto em que a especificação foi escrita.

O programa usa o mesmo padrão dos outros três blocos e indenta com 12 espaços,
que é o centro de uma moldura de 40 colunas: (40 − 16) ÷ 2 = 12. Imprimir um
caractere invisível de propósito seria indefensável.

A segunda diferença era duas linhas que o programa imprimia a mais depois da
moldura, dizendo onde o relatório foi gravado. **Foram removidas nesta
revisão**, porque não são errata do PDF: eram acréscimo nosso. A mensagem de
erro, para o caso de o arquivo não poder ser gravado, continua.

Com isso, a saída do programa fica idêntica à do PDF em 105 das 106 linhas, e a
única diferença é a errata acima.

## Comparação com o relatório do PDF

O relatório gerado é idêntico ao do PDF, exceto que no PDF as seis linhas
aparecem com **um espaço de recuo** cada:

```
 Pokemon recuperados:
 610 Axew
```

O recuo é igual nas seis linhas, o que é típico de recuo de parágrafo do
documento e não de saída de programa. O programa grava sem recuo. Tirando esse
espaço, as seis linhas são idênticas.

Observação: no Windows o arquivo sai com fim de linha CRLF, porque `fopen` em
modo texto traduz `\n` para `\r\n`. No Linux sai com LF. O conteúdo é o mesmo.

## Como refazer esta comparação

```bash
bash testes/rodar_testes.sh
```

A primeira seção do script faz exatamente isso: monta a cópia com a recarga
fixa, roda o exemplo e compara com `diff`.

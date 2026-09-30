# Apresentação

Slides do TP1, montados sobre o `TemplateApresentacaoUFV.pptx` que a
professora disponibilizou. São 12 slides, na ordem que o template propõe:
capa, o problema, modelagem, as partes do código com screenshots, a execução
(entrada e saída) e as referências em ABNT.

| Arquivo | O que é |
|---|---|
| `slides.pptx` | o arquivo editável, para abrir no PowerPoint |
| `slides.pdf` | a versão em PDF, que é o formato pedido para entrega |
| `figuras/` | as imagens usadas nos slides, caso precise trocar alguma |

## Antes de entregar

Os slides estão completos: a capa traz os dois nomes e matrículas, e a divisão
de tarefas está na última linha do slide 3. Não ficou nenhum campo para
preencher.

Vale abrir o `slides.pptx` no PowerPoint e exportar o PDF de novo, em
*Arquivo → Exportar → Criar Documento PDF/XPS*. O `slides.pdf` que está aqui
foi gerado sem o PowerPoint — ele serve como está, mas o exportado pelo próprio
PowerPoint é o que mais se parece com o que a banca vai ver na projeção.

## De onde vêm as figuras

Nenhuma imagem foi desenhada à mão. As de código são recortes dos
arquivos-fonte de verdade (`src/pokelista.c`, `src/missao.c`,
`include/conexao.h`, `include/pokelista.h`), com os números de linha reais. As
de terminal são a saída de uma **execução real**: o programa é compilado e
rodado com `testes/oficiais/teste1.txt` na hora de gerar as figuras, e o
`relatorio.txt` que aparece é o que ele gravou nessa mesma execução. Assim
nenhuma figura pode ficar desatualizada em relação ao código.

Por isso duas coisas mudam se as figuras forem geradas de novo: os **números
de linha**, se o código tiver mudado, e a quantidade da **recarga** na figura
08, que é sorteada a cada execução.

Todas as nove figuras têm a mesma aparência: a do **VS Code no tema claro**
(Light+) — mesmo fundo branco, mesma moldura, mesma aba no topo, mesma fonte
(Consolas) e as cores de sintaxe reais desse tema. Isso é o que o template
pede:

> *Dica: Caso esteja usando o VSCode, use a extensão CodeSnap, e use um tema
> claro, para facilitar a visualização na projeção.*

Nas figuras 03 e 05, as linhas importantes aparecem com o fundo azul da
**seleção** do VS Code, que é como um trecho realmente selecionado aparece na
tela.

## Os pontos de cada slide

| # | Slide | O que defender |
|---|---|---|
| 1 | Capa | — |
| 2 | O problema | o enunciado em cinco frases, incluindo a exigência de lista encadeada |
| 3 | Modelagem | os quatro TADs, a dependência em linha reta, a missão em módulo separado |
| 4 | A lista encadeada | Pokémon por valor, `struct conec`, célula cabeça, `ultimo` |
| 5 | A remoção | por que o `ultimo` tem que ser corrigido antes do `free` |
| 6 | A escolha do treinador | distância ao quadrado, empate exato, `long long` e o limite |
| 7 | A ordem dos dois testes | por que "acabaram os fugitivos?" vem antes de "sem Pokébolas?" |
| 8 | Execução | o formato da entrada e um resgate completo |
| 9 | Execução | a recarga e o relatório |
| 10 | Testes | 72 verificações, o `diff` do exemplo, a conta de memória 4 + 3n |
| 11 | O que não fechava | as quatro inconsistências do enunciado e o que foi decidido |
| 12 | Referências | ABNT |

As explicações longas de cada um desses pontos estão em
[`../descricoes/`](../descricoes/), um arquivo por módulo.

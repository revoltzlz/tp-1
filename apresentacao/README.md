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

Três coisas ficaram marcadas com `<...>` porque só você pode preencher:

1. **Capa** — nome e matrícula dos dois alunos.
2. **Slide 3 (Modelagem)** — a divisão de tarefas: quem fez o quê.
3. **Slide 3** — os mesmos nomes aparecem na última linha.

Depois de preencher no PowerPoint, gere o PDF de novo com
*Arquivo → Exportar → Criar Documento PDF/XPS*. O `slides.pdf` que está aqui
foi gerado sem o PowerPoint, então ele serve como está, mas o exportado pelo
próprio PowerPoint é o que mais se parece com o que a banca vai ver na
projeção.

## De onde vêm as figuras

Nenhuma imagem foi desenhada à mão. As de código foram recortadas dos
arquivos-fonte de verdade (`src/pokelista.c`, `src/missao.c`,
`include/conexao.h`, `include/pokelista.h`), com os números de linha reais. As
de terminal são a saída de uma execução real com
`testes/oficiais/teste1.txt`, e o `relatorio.txt` é o que o programa gravou
nessa mesma execução.

Por isso duas coisas mudam se o código mudar: os **números de linha** nas
figuras e o valor da **recarga** (9 Pokébolas, na figura 08), que é sorteado a
cada execução.

O template recomenda tema claro nos screenshots de código, "para facilitar a
visualização na projeção" — é o que está sendo usado. As figuras de terminal
ficaram escuras porque é assim que o terminal realmente aparece.

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

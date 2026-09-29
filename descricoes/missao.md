# Sistema de controle da missão

Arquivos: `include/missao.h` e `src/missao.c`.

É o módulo que conduz a missão de recuperação: lê a entrada, decide qual
treinador vai a cada resgate, manda entregar e recarregar, e pede o relatório
ao Centro. **Não é um TAD**: é o "Sistema de Controle da Missão" que a
especificação pede, separado dos quatro TADs e do programa principal.

Inclui o `pokecenter.h`, que já traz toda a hierarquia abaixo dele.

---

## `include/missao.h`

O cabeçalho expõe apenas três funções — todo o resto de `missao.c` é detalhe
interno:

```c
void missaoMenu(void);
int missaoExecutarPorArquivo(const char *nomeArquivo);
int missaoExecutarInterativo(void);
```

### As constantes

```c
#define ID_TREINADOR_1 1
#define ID_TREINADOR_2 2
#define ARQ_RELATORIO "relatorio.txt"
#define TAM_CAMINHO 256
#define FMT_CAMINHO "%255s"
#define MAX_QUANTIDADE 1000000
#define LARGURA_MOLDURA 40
#define INDENT_MENU 8
#define INDENT_INICIO 18
#define INDENT_SEM_POKEBOLAS 12
#define INDENT_RESGATADOS 7
#define INDENT_CONCLUIDA 12
#define OPCAO_SAIR 0
#define OPCAO_ARQUIVO 1
#define OPCAO_INTERATIVO 2
```

| Constante | Para que serve |
|---|---|
| `ID_TREINADOR_1`, `ID_TREINADOR_2` | os identificadores, atribuídos na ordem de leitura. O primeiro treinador do arquivo recebe o menor, porque é ele que ganha o desempate |
| `ARQ_RELATORIO` | o nome do arquivo `.txt` do relatório. A especificação não define o nome |
| `TAM_CAMINHO` e `FMT_CAMINHO` | o vetor do caminho digitado no menu e a largura de leitura correspondente |
| `MAX_QUANTIDADE` | teto de sanidade para as quantidades lidas — explicado abaixo |
| `LARGURA_MOLDURA` | as linhas de `=` e de `-` da saída têm 40 colunas |
| os cinco `INDENT_*` | espaços antes do título de cada moldura |
| os três `OPCAO_*` | as opções do menu |

**Sobre `MAX_QUANTIDADE`.** Não vem da especificação. Existe porque o `%d` do
`scanf`, diante de um número que não cabe num `int`, **não garante o que
grava**: o padrão da linguagem diz que o comportamento é indefinido. Na
prática, um arquivo com `Rosa 99999999999999999999` entrou neste compilador
como 1.661.992.959 Pokébolas — um valor positivo, que a checagem de "não pode
ser negativo" não pegava. Como não dá para confiar no que o `scanf` deixou na
variável, o teto recusa o valor. Os testes `erro_pokebolas_gigante.txt` e
`erro_pokedex_gigante.txt` cobrem isso.

Repare na diferença entre dizer "trunca" e "não garante o que grava": o número
1.661.992.959 é específico deste compilador, e outro poderia parar em
`INT_MAX`. A afirmação correta é a segunda.

**Sobre os `INDENT_*`.** Os três primeiros (18, 12 e 7) foram medidos linha por
linha no exemplo da especificação. Os outros dois não vieram de lá:
`INDENT_MENU` é do menu, que o exemplo não mostra, e `INDENT_CONCLUIDA` vale 12
porque centraliza o título nas 40 colunas — (40 − 16) ÷ 2 = 12 — já que no
exemplo essa linha vem com um caractere invisível no lugar da indentação. Isso
é a errata do bloco final, explicada em [exemplo.md](exemplo.md).

---

## `src/missao.c`

O arquivo tem quatro grupos de funções: impressão, cálculo, leitura e execução.
Só as três últimas do arquivo são públicas.

### Impressão

```c
void imprimeRepetido(char caractere, int vezes);
void imprimeSeparador(void);
void imprimeMoldura(const char *titulo, int indentacao);
void imprimeMolduraSemPokebolas(const Treinador *t);
```

`imprimeRepetido` imprime um caractere N vezes, num laço com `putchar`. Serve
para as três coisas que se repetem na saída: as linhas de `=`, as linhas de `-`
e os espaços de indentação dos títulos. **Custo O(n)** nas vezes pedidas.

`imprimeSeparador` é a linha de 40 hifens que separa um resgate do seguinte.

`imprimeMoldura` monta o bloco de destaque: linha de `=`, título recuado, linha
em branco, linha de `=`.

`imprimeMolduraSemPokebolas` faz o mesmo para o bloco do treinador sem
Pokébolas, mas imprime direto, sem passar pela `imprimeMoldura`, porque o
título inclui o nome do treinador. A alternativa seria montar o título num
vetor com `snprintf` — o que existia antes e foi simplificado na revisão final.

### Cálculo

```c
long long distanciaQuadrado(cord a, cord b);
Treinador *escolheTreinador(Treinador *t1, Treinador *t2,
                            long long distancia1, long long distancia2);
```

#### `distanciaQuadrado` — **O(1)**

```c
long long dx = (long long) a.cordX - b.cordX;
long long dy = (long long) a.cordY - b.cordY;
return dx * dx + dy * dy;
```

Devolve o **quadrado** da distância euclidiana, sem tirar a raiz. Duas razões:

1. **Exatidão.** O quadrado é um inteiro, e dois inteiros se comparam sem
   ambiguidade. A especificação manda detectar o empate ("caso ambos os
   treinadores estejam à uma mesma distância") e mandar o de menor
   identificador — com números de ponto flutuante, dois valores que deveriam
   ser iguais podem diferir na última casa por arredondamento, e o empate
   passaria batido. Usar `pow` seria pior ainda, porque ela trabalha em ponto
   flutuante mesmo para expoente inteiro.
2. **A ordem não muda.** A raiz quadrada é uma função crescente: se
   `d1² < d2²`, então `d1 < d2`. Comparar os quadrados dá a mesma resposta que
   comparar as distâncias.

A raiz aparece **só na impressão**, em `resgataPokemon`, com
`sqrt((double) distancia)` e `%.2f`.

**Por que `long long`:** com as coordenadas no limite do mapa, a soma chega a
8 × 10¹², e um `int` guarda só até cerca de 2,1 × 10⁹. O cast está no primeiro
operando de cada subtração, o que faz a conta inteira ser feita em `long long`
— sem ele, `a.cordX - b.cordX` seria calculado em `int` e só depois promovido,
o que não resolveria nada. A conta completa, e o que acontece se as coordenadas
não forem limitadas, está em [coordenadas.md](coordenadas.md).

#### `escolheTreinador` — **O(1)**

Devolve o treinador mais próximo. Em caso de empate, compara os identificadores
e devolve o de menor número, que é o que a especificação exige. São três `if`
encadeados, sem ternário, porque assim cada caso fica numa linha própria.

### Leitura

```c
void pulaMarcaUtf8(FILE *entrada);
int leTreinador(FILE *entrada, int interativo, Treinador *t, int id);
int leFugitivos(FILE *entrada, int interativo, PokeCenter *cp, int *quantidade);
```

#### `pulaMarcaUtf8` — **O(1)**

Pula a marca de ordem de bytes do UTF-8: três bytes invisíveis (`EF BB BF`) que
o Bloco de Notas do Windows escreve no começo dos arquivos que salva. Sem pular,
eles entram colados no nome do primeiro treinador, que aparece com lixo na
frente.

Lê os três primeiros bytes com `fgetc`; se não forem a marca, o `rewind`
devolve a leitura ao começo do arquivo. Por isso só serve para o modo por
arquivo: numa entrada vinda do teclado não há como voltar atrás. O teste
`bom_utf8.txt` cobre o caso.

#### `leTreinador` — **O(1)**

Lê o nome e a quantidade inicial de Pokébolas de um treinador e o inicializa
com o identificador recebido. Devolve 1 em caso de sucesso e 0 se os dados
forem inválidos ou a PokeLista não puder ser criada.

Três checagens, nesta ordem:

```c
if (fscanf(entrada, FMT_NOME_COACH, nome) != 1) { ... return 0; }
if (fscanf(entrada, "%d", &pokebolas) != 1)     { ... return 0; }
if (pokebolas < 0 || pokebolas > MAX_QUANTIDADE) { ... return 0; }
```

O retorno de cada `fscanf` é testado: ele devolve quantos campos conseguiu
converter, então `!= 1` significa que o campo não estava lá ou não era do tipo
esperado. Ignorar esse retorno é o erro que faz o programa seguir com variáveis
não inicializadas.

O `FMT_NOME_COACH` é `"%29s"`, um a menos que o vetor de 30, então um nome
comprido não escreve fora dele.

#### `leFugitivos` — **O(n)**

Lê a quantidade `n` de fugitivos e depois os `n` Pokémon, registrando cada um
no Centro. A quantidade lida sai pelo parâmetro `quantidade`.

Os cinco campos de cada Pokémon vêm num `fscanf` só:

```c
if (fscanf(entrada, "%d " FMT_NOME " " FMT_TIPO " %d %d",
           &numPokedex, nome, tipo, &cordX, &cordY) != 5) { ... return 0; }
```

A string de formato é a junção de três literais — `"%d "`, `"%29s"`, `" %19s"`
e `" %d %d"` — porque o compilador junta literais adjacentes antes de compilar.
O `!= 5` exige que os cinco campos tenham sido lidos: é isso que detecta linha
incompleta e arquivo com menos linhas do que o número declarado.

**O `\r` do Windows não precisa de tratamento.** O `%s` do `fscanf` para em
qualquer espaço em branco, e o `\r` conta como espaço em branco — então o nome
e o tipo nunca o recebem. Os dois arquivos de teste oficiais estão em CRLF e
funcionam sem nenhum cuidado extra.

Depois da leitura, duas validações e a atribuição do Id:

```c
if (numPokedex < 0 || numPokedex > MAX_QUANTIDADE) { ... return 0; }
if (cordX < COORD_MIN || cordX > COORD_MAX ||
    cordY < COORD_MIN || cordY > COORD_MAX)       { ... return 0; }

pokemonInicializar(&p, i + 1, numPokedex, nome, tipo, cordX, cordY);
```

O `i + 1` é o **Id**: a ordem de leitura começando em 1. A validação das
coordenadas não é só higiene de dados — é o que garante que o cálculo da
distância não estoure, como explicado em [coordenadas.md](coordenadas.md).

### Execução

```c
void recarregaNoCentro(PokeCenter *cp, Treinador *t);
void retornaAoCentro(PokeCenter *cp, Treinador *t);
void recarregaSeComecouSemPokebola(PokeCenter *cp, Treinador *t);
Treinador *resgataPokemon(PokeCenter *cp, Treinador *t1, Treinador *t2,
                          const Pokemon *alvo);
void encerraMissao(PokeCenter *cp, Treinador *t1, Treinador *t2);
void executaMissao(PokeCenter *cp, Treinador *t1, Treinador *t2, int qtdFugitivos);
int executa(FILE *entrada, int interativo);
```

#### `resgataPokemon` — **O(n)**

O bloco de um resgate, na ordem em que a saída aparece:

1. imprime o separador, o nome do alvo e a posição dele;
2. calcula as duas distâncias (ao quadrado) e imprime as duas com `sqrt` e
   `%.2f`;
3. escolhe o treinador e anuncia a missão;
4. movimenta o escolhido e imprime o deslocamento;
5. captura;
6. avisa o Centro, que remove o Pokémon da lista de fugas;
7. imprime as Pokébolas restantes.

Devolve o treinador que capturou, ou `NULL` se a captura falhou — o que só
acontece se o escolhido estiver sem Pokébolas, situação que o fluxo evita. É um
caminho defensivo: nesse caso o Pokémon continua na lista de fugitivos e a
mensagem diz que ele escapou.

O O(n) vem do passo 6, que percorre a lista de fugas.

#### `executaMissao` — **O(n²)** no total

O laço principal:

```c
imprimeMoldura("INÍCIO DA MISSÃO", INDENT_INICIO);
treinadorImprimir(t1);
treinadorImprimir(t2);
/* "Pokémons fugitivos a serem resgatados: n" */

if (qtdFugitivos > 0) {
    recarregaSeComecouSemPokebola(cp, t1);
    recarregaSeComecouSemPokebola(cp, t2);
}

for (id = 1; id <= qtdFugitivos; id++) {
    if (!pokecenterBuscarFugitivo(cp, id, &alvo)) {
        continue;
    }
    escolhido = resgataPokemon(cp, t1, t2, &alvo);
    if (escolhido == NULL) {
        continue;
    }

    if (!pokecenterTemFugitivos(cp)) {      /* PRIMEIRO teste */
        break;
    }
    if (treinadorGetPokebolas(escolhido) == 0) {   /* SEGUNDO teste */
        retornaAoCentro(cp, escolhido);
    }
}

encerraMissao(cp, t1, t2);
```

**A ordem dos dois testes é a parte mais delicada do programa.** Depois de cada
captura, o programa pergunta primeiro "acabaram os fugitivos?" e só depois
"o treinador ficou sem Pokébolas?".

Por que nessa ordem: no exemplo da especificação, o último resgate é o do
Umbreon, e ele zera as Pokébolas da Rosa. O exemplo mostra
`Pokébolas restantes para o Treinador(a) Rosa: 0` e, logo em seguida, o bloco
`Todos Pokemons foram resgatados` — **sem nenhuma recarga**. Se os dois testes
estivessem trocados, a Rosa voltaria ao Centro e receberia Pokébolas que não
vai usar, e apareceria um bloco a mais na saída.

O `for` percorre os Ids de 1 a n. Como os Ids são a ordem de leitura e a lista
de fugitivos é montada inserindo no fim, a ordem do laço é a ordem do arquivo.
A busca no começo do corpo confirma que aquele Pokémon ainda está fugido — é o
que manteria a corretude se algum resgate falhasse.

O custo total é O(n²): são n iterações, e a busca e a remoção dentro de cada
uma são O(n). Para o tamanho do problema isso é irrelevante — com 500 Pokémon o
programa roda em menos de um segundo.

#### `encerraMissao` — **O(n)**

Anuncia o fim, move os dois treinadores para o Centro e recebe o que eles ainda
carregam, **na ordem do identificador**: primeiro o `t1`, depois o `t2`.

Essa ordem é o que faz o relatório sair na ordem do exemplo. No exemplo, a Rosa
(id 1) entrega o Turtwig e o Umbreon, e só depois o Nate (id 2) entrega o
Noivern — é daí que vem a sequência `610, 657, 387, 197, 715`, com o 715 no
fim. A conta completa está em [exemplo.md](exemplo.md).

Se sobrou algum fugitivo, o título muda e a lista de quem ficou é impressa.

#### `recarregaSeComecouSemPokebola` — **O(1)**

Trata o treinador que começou a missão com zero Pokébolas. Ninguém parte para
uma captura sem Pokébola, e os dois treinadores já começam no Centro, então ele
é recarregado antes do primeiro resgate — sem precisar se movimentar nem
entregar nada.

A especificação só descreve a recarga "logo após realizar uma captura", e não
diz o que fazer com um treinador que começa sem nenhuma Pokébola. Esta é uma
decisão de projeto, não uma exigência. Os testes `zero_pokebolas.txt` e
`ambos_zero_pokebolas.txt` cobrem o caso.

#### `executa` — **O(n²)**

O caminho comum dos dois modos de uso. Recebe um `FILE *`, que é o arquivo
aberto no modo por arquivo e o `stdin` no modo interativo; o parâmetro
`interativo` só liga as mensagens que pedem cada dado.

A parte mais delicada é a liberação nos caminhos de erro:

```c
if (!leTreinador(entrada, interativo, &treinador1, ID_TREINADOR_1)) {
    pokecenterLiberar(&centro);
    return 0;
}

if (!leTreinador(entrada, interativo, &treinador2, ID_TREINADOR_2)) {
    treinadorLiberar(&treinador1);
    pokecenterLiberar(&centro);
    return 0;
}

if (!leFugitivos(entrada, interativo, &centro, &qtdFugitivos)) {
    treinadorLiberar(&treinador1);
    treinadorLiberar(&treinador2);
    pokecenterLiberar(&centro);
    return 0;
}
```

São três saídas de erro, e cada uma libera **por extenso** o que chegou a ser
criado até ali. Liberar uma PokeLista que nunca foi inicializada leria
apontadores com lixo, então a ordem importa: se o primeiro treinador falhou,
não há nada dele para liberar; se o segundo falhou, libera-se o primeiro; e
assim por diante.

Uma versão anterior fazia isso com um contador `treinadoresProntos` e um `&&`
de curto-circuito, que ocupava menos linhas mas escondia o fluxo: era preciso
saber que o `&&` não avalia o lado direito quando o esquerdo é falso para
entender por que `leFugitivos` não era chamada. Três `if` explícitos custam
seis linhas a mais e não escondem nada.

No fim, no caminho normal, os três `liberar` recuperam tudo: as duas listas do
Centro e a lista de cada treinador, com as quatro células cabeça.

O retorno é o resultado da geração do relatório, porque emiti-lo é uma das
operações que a especificação exige — não conseguir gravá-lo é falha da
execução.

### As três funções públicas

```c
int missaoExecutarPorArquivo(const char *nomeArquivo);
int missaoExecutarInterativo(void);
void missaoMenu(void);
```

`missaoExecutarPorArquivo` abre o arquivo, testa o `fopen` contra `NULL`, pula
a marca UTF-8, chama `executa` e fecha o arquivo.

`missaoExecutarInterativo` chama `executa(stdin, 1)`. **É a mesma função de
leitura**, com o `stdin` no lugar do arquivo — não existe código duplicado entre
os dois modos.

`missaoMenu` é o laço do menu, com um `switch` de três opções. A leitura da
opção usa `scanf("%d")`; se falhar, a entrada acabou ou o usuário digitou algo
que não é número, e o programa encerra com mensagem em vez de insistir com um
caractere preso na entrada e girar sozinho.

```
========================================
        TP1 - RESGATE DE POKÉMON

========================================

1 - Executar a missão a partir de um arquivo
2 - Executar a missão no modo interativo
0 - Sair

Opção:
```

---

## Decisões de projeto

### Uma só função de leitura para os dois modos

`executa(FILE *entrada, int interativo)` serve ao modo por arquivo e ao
interativo. O que muda é de onde vem o `FILE *` e se as mensagens que pedem
cada dado são impressas. A alternativa — duas funções de leitura — significaria
manter o mesmo formato de entrada em dois lugares.

### O programa não tem nenhum modo de teste dentro dele

Para comparar a saída com o exemplo da especificação caractere por caractere é
preciso saber quanto a recarga vai sortear. Isso **não** é resolvido com uma
macro de compilação no código entregue: o `testes/rodar_testes.sh` copia o
projeto para uma pasta temporária e troca, só na cópia, a linha do sorteio pelo
valor do exemplo. O programa entregue sorteia sempre.

Uma versão anterior tinha um `#ifdef` que fixava a recarga dentro do TAD Centro
de Pesquisa, e outra tinha um `#ifdef` que fixava a semente no `main`. As duas
foram removidas na revisão final: um TAD com um interruptor que desliga o
sorteio exigido pela especificação é difícil de defender, e a cópia temporária
resolve o problema sem tocar no código entregue.

### O menu não aceita arquivo pela linha de comando

Houve uma versão que aceitava `./tp1 arquivo.txt` como atalho. Foi removida: a
especificação pede os dois modos de uso, e o menu já atende os dois. Os testes
alimentam o menu pela entrada padrão, que é como um usuário o usaria.

### As mensagens de erro saem por `printf`, não por `fprintf(stderr, ...)`

`stdout` e `stderr` têm buffers separados, e o que é escrito primeiro não é
necessariamente o que aparece primeiro quando a saída é redirecionada para um
arquivo. Com as mensagens de erro saindo pelo mesmo caminho do resto, a ordem
das linhas é sempre a ordem em que foram impressas. Houve uma versão que
resolvia isso com `setvbuf` desligando os dois buffers; usar só `printf` é mais
simples e tem o mesmo efeito.

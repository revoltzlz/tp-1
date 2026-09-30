#!/bin/bash
# ============================================================================
# Bateria de testes do TP1 - Resgate de Pokemon
#
# COMO RODAR (no Git Bash ou no Linux, a partir da raiz do projeto):
#
#     bash testes/rodar_testes.sh
#
# O programa nao tem modo de teste nenhum dentro dele: a unica forma de usa-lo
# e pelo menu. Por isso todo teste aqui alimenta o menu pela entrada padrao,
# como se alguem estivesse digitando:
#
#     printf '1\narquivo.txt\n0\n' | ./tp1
#       1  -> executar a missao a partir de um arquivo
#          -> o caminho do arquivo
#       0  -> sair
#
# Para comparar a saida com o exemplo da especificacao caractere por caractere
# seria preciso saber quanto a recarga vai sortear. O script resolve isso
# fazendo uma COPIA do projeto numa pasta temporaria e trocando, so na copia,
# a linha do sorteio pelo valor que aparece no exemplo (2). O codigo entregue
# continua sorteando.
# ============================================================================

cd "$(dirname "$0")/.." || exit 1

T=testes
PASTA=tmp_testes
PASSOU=0
FALHOU=0

verde()    { echo "  PASSOU  $1"; PASSOU=$((PASSOU + 1)); }
vermelho() { echo "  FALHOU  $1"; FALHOU=$((FALHOU + 1)); }

# Opcoes de otimizacao que o script vai tentando ate o sistema aceitar executar
# o binario. Elas nao mudam o comportamento do programa, apenas o codigo de
# maquina: no Windows 11 o Smart App Control decide bloquear ou nao cada
# executavel pelo conteudo dele.
VARIANTES=("" "-O1" "-O2" "-O3" "-Os" "-Og" "-g" "-O1 -g" "-O2 -g" "-O3 -g"
           "-O1 -fno-inline" "-O2 -fno-inline" "-O3 -fno-inline" "-Og -g")

# roda <arquivo de entrada>
# Executa a missao sobre o arquivo passando pelo menu, e escreve a saida na
# saida padrao.
roda() {
    printf '1\n%s\n0\n' "$1" | "$EXE" 2>&1
}

# compila <diretorio> <rotulo>
# Compila o projeto que esta no diretorio indicado, deixando o executavel em
# <diretorio>/tp1, e define EXE. A primeira compilacao usa exatamente as
# opcoes do Makefile, e e ela que decide o resultado "zero avisos".
compila() {
    local dir="$1"
    local rotulo="$2"
    local saida opt

    rm -f "$dir/tp1" "$dir/tp1.exe"
    saida=$( (cd "$dir" && gcc -Wall -Wextra -std=c99 -g -Iinclude -o tp1 \
                               main.c src/*.c -lm) 2>&1 )
    if [ -n "$saida" ]; then
        vermelho "$rotulo: com avisos ou erros"
        echo "$saida"
        return 1
    fi
    verde "$rotulo: zero avisos"

    for opt in "${VARIANTES[@]}"; do
        if [ -n "$opt" ]; then
            rm -f "$dir/tp1" "$dir/tp1.exe"
            (cd "$dir" && gcc -Wall -Wextra -std=c99 -Iinclude $opt -o tp1 \
                              main.c src/*.c -lm 2>/dev/null)
        fi
        EXE="$dir/tp1"
        [ -f "$dir/tp1.exe" ] && EXE="$dir/tp1.exe"
        if roda "$T/oficiais/teste1.txt" | grep -q 'MISSÃO CONCLUÍDA'; then
            if [ -n "$opt" ]; then
                echo "          (o sistema bloqueou o binario sem otimizacao;"
                echo "           os testes seguem com o binario gerado com $opt)"
            fi
            return 0
        fi
    done

    echo
    echo "ERRO: o executavel foi gerado, mas nao produziu a saida esperada."
    echo "      Saida obtida:"
    roda "$T/oficiais/teste1.txt" | head -5 | sed 's/^/      /'
    echo
    echo "  Se a mensagem acima for \"Permission denied\" ou vier vazia, a causa"
    echo "  mais provavel no Windows 11 e o Smart App Control, que bloqueia"
    echo "  executaveis sem assinatura digital. Para confirmar, rode no"
    echo "  PowerShell:"
    echo
    echo "    Get-WinEvent -LogName Microsoft-Windows-CodeIntegrity/Operational -MaxEvents 5"
    echo
    echo "  Alternativas: desligar o Smart App Control (mudanca irreversivel:"
    echo "  so volta reinstalando o Windows) ou compilar e rodar no WSL ou em"
    echo "  outra maquina."
    echo
    echo "Testes interrompidos: nenhum resultado seria confiavel."
    exit 1
}

# Corta da saida so o trecho que o exemplo do PDF cobre: da linha de "=" que
# abre a moldura INICIO DA MISSAO ate a ultima linha de "=". Fora desse trecho
# ficam o menu, que o exemplo nao mostra, e a despedida.
corta_missao() {
    local arquivo="$1"
    local ini titulo fim
    ini=$(grep -n 'INÍCIO DA MISSÃO' "$arquivo" | head -1 | cut -d: -f1)
    titulo=$(grep -n 'MISSÃO CONCLUÍDA' "$arquivo" | head -1 | cut -d: -f1)
    if [ -z "$ini" ] || [ -z "$titulo" ]; then
        return 1
    fi
    # O fim e a primeira linha de "=" depois do titulo final. Nao serve pegar a
    # ultima linha de "=" do arquivo, porque depois da missao o menu aparece de
    # novo e traz a moldura dele.
    fim=$(grep -n '^=\{40\}$' "$arquivo" | cut -d: -f1 \
          | awk -v t="$titulo" '$1 > t {print; exit}')
    if [ -z "$fim" ]; then
        return 1
    fi
    sed -n "$((ini - 1)),${fim}p" "$arquivo"
}

rm -rf "$PASTA"

# ---------------------------------------------------------------------------
echo "=== 1. Exemplo da especificacao, comparado caractere por caractere ==="

# A copia temporaria e identica ao projeto, com uma unica linha trocada: a do
# sorteio da recarga, que passa a devolver o valor do exemplo do PDF.
mkdir -p "$PASTA/copia"
cp -r Makefile main.c include src "$PASTA/copia/"
sed -i 's|^    quantidade = MIN_RECARGA + rand() % (MAX_RECARGA - MIN_RECARGA + 1);$|    quantidade = 2; /* copia de teste: valor da recarga do exemplo do PDF */|' \
    "$PASTA/copia/src/pokecenter.c"

if ! grep -q 'copia de teste' "$PASTA/copia/src/pokecenter.c"; then
    vermelho "nao foi possivel fixar a recarga na copia de teste"
    echo "       a linha do sorteio em src/pokecenter.c mudou de forma?"
    exit 1
fi

compila "$PASTA/copia" "build da copia de teste (recarga fixada em 2)"

rm -f relatorio.txt
roda "$T/oficiais/teste1.txt" | tr -d '\r' > "$PASTA/saida.txt"
corta_missao "$PASTA/saida.txt" > "$PASTA/saida_corte.txt"

# A comparacao e contra saida_exemplo_esperada.txt, que e a transcricao fiel do
# PDF (saida_exemplo_pdf.txt) com a errata do bloco final corrigida. O comando
#   diff testes/saida_exemplo_pdf.txt testes/saida_exemplo_esperada.txt
# mostra exatamente qual e a diferenca. A explicacao esta em
# descricoes/exemplo.md.
if diff -u "$T/saida_exemplo_esperada.txt" "$PASTA/saida_corte.txt" > "$PASTA/diff.txt" 2>&1; then
    verde "saida do exemplo identica a esperada"
else
    vermelho "saida do exemplo difere"
    head -40 "$PASTA/diff.txt"
fi

if diff -u "$T/relatorio_exemplo_esperado.txt" <(tr -d '\r' < relatorio.txt) \
        > "$PASTA/diff_rel.txt" 2>&1; then
    verde "relatorio identico ao esperado"
else
    vermelho "relatorio difere"
    cat "$PASTA/diff_rel.txt"
fi

# ---------------------------------------------------------------------------
echo
echo "--- as regras da missao, sobre a mesma saida ---"

REGRAS="$PASTA/saida.txt"

# checa_regra <descricao> <quantidade esperada> <padrao egrep>
checa_regra() {
    local desc="$1" esperado="$2" padrao="$3" achado
    achado=$(grep -cE "$padrao" "$REGRAS")
    if [ "$achado" -eq "$esperado" ]; then
        verde "$desc"
    else
        vermelho "$desc (encontrou $achado, esperava $esperado)"
    fi
}

# O primeiro alvo, o Axew, esta a 9.90 dos dois treinadores, que comecam em
# (0,0): e o empate. A missao tem de ir para a Rosa, de identificador 1.
primeira=$(grep -m1 '^Missão atribuída' "$REGRAS")
if [ "$primeira" = "Missão atribuída ao Treinador(a) Rosa." ]; then
    verde "empate de distancia vai para a Rosa, de menor identificador"
else
    vermelho "empate nao foi para o menor identificador (veio: $primeira)"
fi

checa_regra "os 5 Pokemon foram capturados" 5 'capturado com sucesso!'
checa_regra "a Rosa fez 4 das 5 capturas" 4 '^Missão atribuída ao Treinador\(a\) Rosa\.$'
checa_regra "o Nate fez 1 captura, a do Noivern" 1 '^Missão atribuída ao Treinador\(a\) Nate\.$'
checa_regra "houve exatamente uma recarga no meio da missao" 1 'recebeu [0-9]+ Pokébolas'
checa_regra "a Rosa zerou as Pokebolas duas vezes" 2 'restantes para o Treinador\(a\) Rosa: 0'
checa_regra "as 10 distancias foram impressas" 10 '^Distância Treinador\(a\) '

# A segunda vez que a Rosa zera as Pokebolas e no ultimo resgate. Se a ordem
# dos dois testes apos a captura estivesse trocada, apareceria uma recarga
# depois dessa linha.
ULTIMO_ZERO=$(grep -n 'restantes para o Treinador(a) Rosa: 0' "$REGRAS" | tail -1 | cut -d: -f1)
ULTIMA_RECARGA=$(grep -n 'recebeu .* Pokébolas' "$REGRAS" | tail -1 | cut -d: -f1)
if [ "${ULTIMO_ZERO:-0}" -gt "${ULTIMA_RECARGA:-0}" ]; then
    verde "o ultimo resgate zera as Pokebolas e NAO dispara recarga"
else
    vermelho "houve recarga depois do ultimo resgate"
fi

# As dez distancias do exemplo, uma a uma.
for d in "Rosa: 9.90" "Nate: 9.90" "Rosa: 3.00" "Nate: 12.21" "Rosa: 1.41" \
         "Nate: 1.41" "Rosa: 15.56" "Nate: 14.14" "Rosa: 7.21" "Nate: 22.67"; do
    if grep -qF "Distância Treinador(a) $d" "$REGRAS"; then
        verde "distancia do PDF conferida: $d"
    else
        vermelho "distancia do PDF ausente: $d"
    fi
done

# ---------------------------------------------------------------------------
echo
echo "=== 2. Casos validos: invariantes ==="

# A partir daqui o programa e exatamente o que vai na entrega.
compila "." "build normal, como na entrega"

# checa_valido <arquivo> <quantidade esperada de Pokemon no relatorio>
checa_valido() {
    local arquivo="$1"
    local esperado="$2"
    local nome saida linhas
    nome=$(basename "$arquivo")

    rm -f relatorio.txt
    saida=$(roda "$arquivo")

    if ! echo "$saida" | grep -q 'MISSÃO CONCLUÍDA'; then
        vermelho "$nome: a missao nao chegou ao fim"
        return
    fi

    if echo "$saida" | grep -q 'ainda há'; then
        vermelho "$nome: sobrou Pokemon na lista de fugitivos"
        return
    fi

    if echo "$saida" | grep -qE 'Pokébolas[^:]*: *-'; then
        vermelho "$nome: quantidade de Pokebolas negativa"
        return
    fi

    # Toda distancia impressa precisa ser um numero real e nao negativo. Esta
    # checagem existe porque um estouro no calculo do quadrado da distancia
    # produzia "nan" e mandava a missao para o treinador MAIS DISTANTE, sem que
    # nenhuma das outras invariantes percebesse.
    if echo "$saida" | grep -qiE '^Distância.*: *(-|nan|inf)'; then
        vermelho "$nome: distancia invalida (negativa, nan ou inf)"
        echo "$saida" | grep -iE '^Distância.*: *(-|nan|inf)' | head -3 | sed 's/^/            /'
        return
    fi

    if [ ! -f relatorio.txt ]; then
        vermelho "$nome: relatorio nao foi gerado"
        return
    fi

    # O relatorio tem uma linha de cabecalho mais uma linha por Pokemon.
    linhas=$(( $(wc -l < relatorio.txt) - 1 ))
    if [ "$linhas" -ne "$esperado" ]; then
        vermelho "$nome: relatorio com $linhas Pokemon, esperado $esperado"
        return
    fi

    verde "$nome: $esperado recuperados, lista de fugas vazia, distancias validas"
}

checa_valido "$T/oficiais/teste1.txt" 5
checa_valido "$T/oficiais/teste2.txt" 20
checa_valido "$T/entrada_exemplo.txt" 5
checa_valido "$T/crlf_windows.txt" 5
checa_valido "$T/bom_utf8.txt" 5
checa_valido "$T/empate_origem.txt" 3
checa_valido "$T/pokemon_na_origem.txt" 2
checa_valido "$T/uma_pokebola.txt" 4
checa_valido "$T/zero_pokebolas.txt" 3
checa_valido "$T/ambos_zero_pokebolas.txt" 2
checa_valido "$T/um_pokemon.txt" 1
checa_valido "$T/zero_pokemon.txt" 0
checa_valido "$T/pokedex_repetida.txt" 4
checa_valido "$T/coordenadas_negativas.txt" 3
checa_valido "$T/coordenadas_no_limite.txt" 3
checa_valido "$T/nomes_maximos.txt" 2
checa_valido "$T/espacos_e_linhas_extras.txt" 3
checa_valido "$T/mesma_coordenada.txt" 4
checa_valido "$T/quinhentos_pokemon.txt" 500

# ---------------------------------------------------------------------------
echo
echo "=== 3. Casos invalidos: avisa o erro e volta ao menu ==="

# checa_invalido <arquivo ou caminho inexistente>
# A mensagem procurada e "Erro:", exatamente como o programa escreve.
checa_invalido() {
    local arquivo="$1"
    local nome saida
    nome=$(basename "$arquivo")

    saida=$(roda "$arquivo")

    if echo "$saida" | grep -q 'MISSÃO CONCLUÍDA'; then
        vermelho "$nome: aceitou dados invalidos e executou a missao"
    elif ! echo "$saida" | grep -q 'Erro:'; then
        vermelho "$nome: nao avisou o erro"
    elif ! echo "$saida" | grep -q 'Até a próxima'; then
        vermelho "$nome: nao voltou ao menu depois do erro"
    else
        verde "$nome: recusou com mensagem de erro e voltou ao menu"
    fi
}

checa_invalido "$T/erro_quantidade_maior.txt"
checa_invalido "$T/erro_campos_faltando.txt"
checa_invalido "$T/erro_pokebolas_negativas.txt"
checa_invalido "$T/erro_fugitivos_negativos.txt"
checa_invalido "$T/erro_pokebolas_texto.txt"
checa_invalido "$T/erro_vazio.txt"
checa_invalido "$T/erro_incompleto.txt"
checa_invalido "$T/nomes_longos_demais.txt"
checa_invalido "$T/erro_pokedex_negativa.txt"
checa_invalido "$T/erro_coordenada_texto.txt"
checa_invalido "$T/erro_quantidade_texto.txt"
checa_invalido "$T/erro_so_espacos.txt"
checa_invalido "$T/erro_coordenada_fora_do_mapa.txt"
checa_invalido "$T/erro_pokebolas_gigante.txt"
checa_invalido "$T/erro_pokedex_gigante.txt"
checa_invalido "$T/este_arquivo_nao_existe.txt"

# ---------------------------------------------------------------------------
echo
echo "=== 4. Menu e modo interativo ==="

# Numero fora das opcoes: avisa e volta ao menu.
saida=$(printf '9\n0\n' | "$EXE" 2>&1)
if echo "$saida" | grep -q 'Opção inválida' && echo "$saida" | grep -q 'Até a próxima'; then
    verde "numero fora das opcoes e recusado e o menu continua"
else
    vermelho "numero fora das opcoes nao foi tratado"
fi

# Letra no lugar de numero: encerra com mensagem, sem girar sozinho.
saida=$(printf 'x\n' | "$EXE" 2>&1)
if echo "$saida" | grep -q 'Entrada encerrada'; then
    verde "letra no lugar de numero encerra com mensagem, sem laco infinito"
else
    vermelho "letra no lugar de numero nao foi tratada"
    echo "$saida" | tail -3
fi

# Entrada que termina sem nada (Ctrl+D no Linux, Ctrl+Z no Windows).
if printf '' | "$EXE" > /dev/null 2>&1; then
    verde "menu encerra sem erro quando a entrada termina"
else
    vermelho "menu nao tratou fim de entrada"
fi

# Modo interativo: os mesmos dados do exemplo, digitados.
rm -f relatorio.txt
saida=$(printf '2\nRosa 2\nNate 2\n5\n610 Axew Dragão 7 7\n657 Frogadier Água 10 7\n387 Turtwig Folha 1 1\n715 Noivern Voador -10 -10\n197 Umbreon Noturno 5 7\n0\n' | "$EXE" 2>&1)
if echo "$saida" | grep -q 'MISSÃO CONCLUÍDA' && [ -f relatorio.txt ] \
   && [ "$(( $(wc -l < relatorio.txt) - 1 ))" -eq 5 ]; then
    verde "modo interativo executa a missao e gera o relatorio"
else
    vermelho "modo interativo nao completou a missao"
    echo "$saida" | tail -10
fi

if echo "$saida" | grep -q 'fugitivos registrados no Centro de Pesquisa'; then
    verde "modo interativo lista os fugitivos registrados para conferencia"
else
    vermelho "modo interativo nao listou os fugitivos registrados"
fi

if echo "$saida" | grep -q 'Opção inválida'; then
    vermelho "menu reclamou de opcao invalida depois do modo interativo"
else
    verde "menu nao inventa opcao invalida ao voltar do modo interativo"
fi

# Erro no meio do modo interativo: avisa, volta ao menu e sai limpo.
saida=$(printf '2\nRosa -5\n0\n' | "$EXE" 2>&1)
if echo "$saida" | grep -q 'Erro:' && echo "$saida" | grep -q 'Até a próxima'; then
    verde "erro no modo interativo avisa, volta ao menu e sai limpo"
else
    vermelho "erro no modo interativo nao voltou ao menu"
fi

# ---------------------------------------------------------------------------
echo
echo "=== 5. Vazamento de memoria ==="

ARQUIVOS_MEMORIA=("$T/oficiais/teste1.txt"
                  "$T/oficiais/teste2.txt"
                  "$T/quinhentos_pokemon.txt"
                  "$T/zero_pokemon.txt"
                  "$T/pokedex_repetida.txt"
                  "$T/erro_campos_faltando.txt"
                  "$T/erro_incompleto.txt")

if command -v valgrind > /dev/null 2>&1; then
    for arquivo in "${ARQUIVOS_MEMORIA[@]}"; do
        nome=$(basename "$arquivo")
        rm -f "$PASTA/valgrind.txt"
        if printf '1\n%s\n0\n' "$arquivo" \
           | valgrind --leak-check=full --errors-for-leak-kinds=all \
                      --error-exitcode=99 "$EXE" > /dev/null 2> "$PASTA/valgrind.txt"; then
            verde "valgrind em $nome: nenhum vazamento"
        else
            vermelho "valgrind em $nome: problemas"
            grep -E 'lost|ERROR SUMMARY' "$PASTA/valgrind.txt" | sed 's/^/            /'
        fi
    done
else
    echo "  (valgrind nao existe neste sistema; contando malloc e free)"

    # Segunda copia temporaria. O cabecalho abaixo troca malloc e free por
    # versoes que contam as chamadas, e e injetado de fora, so no arquivo
    # pokelista.c, que e o unico do projeto que aloca memoria. Nenhum arquivo
    # do projeto e alterado.
    mkdir -p "$PASTA/memoria"
    cp -r Makefile main.c include src "$PASTA/memoria/"
    cat > "$PASTA/memoria/conta.h" <<'FIM'
#include <stdio.h>
#include <stdlib.h>
long cmAlocacoes = 0;
long cmLiberacoes = 0;
int cmRegistrado = 0;
void cmRelatorio(void)
{
    fprintf(stderr, "[memoria] malloc: %ld  free: %ld  diferenca: %ld\n",
            cmAlocacoes, cmLiberacoes, cmAlocacoes - cmLiberacoes);
}
void *cmMalloc(size_t bytes)
{
    void *bloco;
    if (!cmRegistrado) {
        cmRegistrado = 1;
        atexit(cmRelatorio);
    }
    bloco = malloc(bytes);
    if (bloco != NULL) {
        cmAlocacoes++;
    }
    return bloco;
}
void cmFree(void *bloco)
{
    if (bloco != NULL) {
        cmLiberacoes++;
    }
    free(bloco);
}
#define malloc(bytes) cmMalloc(bytes)
#define free(bloco) cmFree(bloco)
FIM

    EXE_MEM=""
    for opt in "${VARIANTES[@]}"; do
        (cd "$PASTA/memoria" && rm -f tp1 tp1.exe ./*.o && \
         gcc -Wall -Wextra -std=c99 -Iinclude $opt -include conta.h \
             -c src/pokelista.c -o pokelista.o 2>/dev/null && \
         gcc -Wall -Wextra -std=c99 -Iinclude $opt -c main.c -o main.o 2>/dev/null && \
         for m in pokemon treinador pokecenter missao; do \
             gcc -Wall -Wextra -std=c99 -Iinclude $opt -c "src/$m.c" -o "$m.o" 2>/dev/null; \
         done && \
         gcc $opt -o tp1 ./*.o -lm 2>/dev/null)
        candidato="$PASTA/memoria/tp1"
        [ -f "$PASTA/memoria/tp1.exe" ] && candidato="$PASTA/memoria/tp1.exe"
        if printf '1\n%s\n0\n' "$T/oficiais/teste1.txt" \
           | "$candidato" 2>&1 > /dev/null | grep -q '\[memoria\]'; then
            EXE_MEM="$candidato"
            [ -n "$opt" ] && echo "          (build de contagem gerado com $opt)"
            break
        fi
    done

    if [ -z "$EXE_MEM" ]; then
        vermelho "contagem de memoria: o sistema bloqueou todos os binarios"
    else
        for arquivo in "${ARQUIVOS_MEMORIA[@]}"; do
            nome=$(basename "$arquivo")
            conta=$(printf '1\n%s\n0\n' "$arquivo" | "$EXE_MEM" 2>&1 > /dev/null \
                    | grep '\[memoria\]' | sed 's/.*diferenca: //')
            if [ -z "$conta" ]; then
                vermelho "memoria em $nome: nao foi possivel medir"
            elif [ "$conta" -eq 0 ]; then
                verde "memoria em $nome: malloc = free"
            else
                vermelho "memoria em $nome: sobraram $conta blocos sem free"
            fi
        done

        # Arquivo inexistente: o programa desiste no fopen, antes de chegar a
        # alocar qualquer coisa. Como o contador so imprime depois do primeiro
        # malloc, o esperado aqui e nao aparecer contador nenhum - ou aparecer
        # com diferenca zero, se o compilador tiver reordenado algo.
        conta=$(printf '1\n%s\n0\n' "$T/este_arquivo_nao_existe.txt" \
                | "$EXE_MEM" 2>&1 > /dev/null | grep '\[memoria\]' | sed 's/.*diferenca: //')
        if [ -z "$conta" ]; then
            verde "memoria com arquivo inexistente: nenhuma alocacao foi feita"
        elif [ "$conta" -eq 0 ]; then
            verde "memoria com arquivo inexistente: malloc = free"
        else
            vermelho "memoria com arquivo inexistente: diferenca de ${conta}"
        fi

        # Os numeros absolutos, como evidencia. Sao 4 celulas cabeca (as duas
        # listas do Centro e a lista de cada treinador) mais 3 celulas por
        # Pokemon: uma no registro da fuga, uma na captura e uma na entrega.
        for par in "oficiais/teste2.txt:20" "quinhentos_pokemon.txt:500"; do
            arq="$T/${par%%:*}"
            n="${par##*:}"
            echo "          $(basename "$arq"): esperado 4 + 3x$n = $((4 + 3 * n))"
            printf '1\n%s\n0\n' "$arq" | "$EXE_MEM" 2>&1 > /dev/null \
                | grep '\[memoria\]' | sed 's/^/          /'
        done
    fi
fi

# ---------------------------------------------------------------------------
echo
echo "========================================"
printf 'Passou: %d    Falhou: %d\n' "$PASSOU" "$FALHOU"
echo "========================================"

rm -rf "$PASTA"
rm -f tp1 tp1.exe ./*.o

[ "$FALHOU" -eq 0 ]

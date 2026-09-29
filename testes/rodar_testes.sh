#!/bin/bash
# ============================================================================
# Bateria de testes do TP1 - Resgate de Pokemon
#
# COMO RODAR (no Git Bash, a partir da raiz do projeto):
#
#     bash testes/rodar_testes.sh
#
# O script compila o programa duas vezes, sempre com o nome tp1:
#
#   1. com -DRECARGA_FIXA=2, para que a saida do exemplo da especificacao
#      possa ser comparada caractere por caractere (2 e a quantidade de
#      Pokebolas que aparece na recarga do exemplo do PDF);
#   2. sem essa macro, exatamente como o programa e entregue, com recarga
#      aleatoria, para todos os outros casos.
#
# A macro RECARGA_FIXA existe somente para este teste. O Makefile nao a
# define, entao o programa entregue sempre sorteia a recarga.
#
# Em cada caso valido o script confere as invariantes: o programa termina
# normalmente, nenhum Pokemon fica na lista de fugitivos, o relatorio tem
# exatamente n Pokemon e nenhuma quantidade de Pokebolas aparece negativa.
# Em cada caso invalido, confere que o programa avisa o erro e termina sem
# travar.
# ============================================================================

cd "$(dirname "$0")/.." || exit 1

T=testes
PASSOU=0
FALHOU=0

verde()    { printf '  \033[32mPASSOU\033[0m  %s\n' "$1"; PASSOU=$((PASSOU + 1)); }
vermelho() { printf '  \033[31mFALHOU\033[0m  %s\n' "$1"; FALHOU=$((FALHOU + 1)); }

# compila <rotulo> [macros extras...]
# Deixa o executavel em tp1 (ou tp1.exe no Windows) e reprova se houver aviso.
#
# A primeira compilacao usa exatamente as opcoes do Makefile, e e ela que
# decide o resultado "zero avisos". Se o binario gerado nao puder ser
# executado, o laco seguinte repete a compilacao acrescentando uma opcao de
# otimizacao por vez. Isso existe por causa do Smart App Control do Windows 11,
# que decide bloquear ou nao cada executavel pelo conteudo dele: um binario
# diferente as vezes passa. As opcoes de otimizacao nao mudam o comportamento
# do programa, apenas o codigo de maquina gerado.
compila() {
    local rotulo="$1"
    shift
    local saida opt

    rm -f tp1 tp1.exe
    saida=$(gcc -Wall -Wextra -std=c99 -Iinclude "$@" -o tp1 main.c src/*.c -lm 2>&1)
    if [ -n "$saida" ]; then
        vermelho "$rotulo: com avisos ou erros"
        echo "$saida"
        return 1
    fi
    verde "$rotulo: zero avisos"

    EXE=./tp1
    [ -f ./tp1.exe ] && EXE=./tp1.exe

    for opt in "" "-O1" "-O2" "-O3" "-Os" "-Og" "-g" "-O1 -g" "-O2 -g" "-O3 -g" \
               "-O1 -fno-inline" "-O2 -fno-inline" "-O3 -fno-inline" \
               "-Og -g" "-Os -g" "-O2 -funroll-loops" "-O3 -funroll-loops"; do
        if [ -n "$opt" ]; then
            rm -f tp1 tp1.exe
            gcc -Wall -Wextra -std=c99 -Iinclude "$@" $opt -o tp1 \
                main.c src/*.c -lm 2>/dev/null
            EXE=./tp1
            [ -f ./tp1.exe ] && EXE=./tp1.exe
        fi
        if "$EXE" "$T/oficiais/teste1.txt" 2>/dev/null | grep -q 'MISSÃO CONCLUÍDA'; then
            if [ -n "$opt" ]; then
                echo "          (o sistema bloqueou o binario sem otimizacao;"
                echo "           os testes seguem com o binario gerado com $opt)"
            fi
            return 0
        fi
    done

    # Nenhuma variante conseguiu rodar.
    return 1
}

# Confere que o executavel realmente roda, exigindo que ele produza a moldura
# final do exemplo. Sem esta checagem, um programa que nem chega a comecar
# faria os testes de erro passarem por engano: a mensagem "Permission denied"
# do proprio shell seria confundida com a mensagem de erro do programa.
#
# No Windows 11 o Smart App Control, quando esta ligado, bloqueia executaveis
# sem assinatura digital, inclusive os que o gcc acabou de gerar.
confere_que_roda() {
    if "$EXE" "$T/oficiais/teste1.txt" 2>/dev/null | grep -q 'MISSÃO CONCLUÍDA'; then
        return 0
    fi

    echo
    echo "ERRO: o executavel foi gerado, mas nao produziu a saida esperada."
    echo "      Saida de $EXE $T/oficiais/teste1.txt :"
    echo
    "$EXE" "$T/oficiais/teste1.txt" 2>&1 | head -5 | sed 's/^/      /'
    echo
    echo "  Se a mensagem acima for \"Permission denied\" ou vier vazia, a causa"
    echo "  mais provavel no Windows 11 e o Smart App Control, que bloqueia"
    echo "  executaveis sem assinatura digital. Para confirmar, rode no"
    echo "  PowerShell:"
    echo
    echo "    Get-WinEvent -LogName Microsoft-Windows-CodeIntegrity/Operational -MaxEvents 5"
    echo
    echo "  Se aparecer \"Smart App Control Block\", as alternativas sao desligar"
    echo "  o Smart App Control (mudanca irreversivel: so volta reinstalando o"
    echo "  Windows) ou compilar e rodar no WSL ou em outra maquina."
    echo
    echo "Testes interrompidos: nenhum resultado abaixo seria confiavel."
    exit 1
}

# ---------------------------------------------------------------------------
echo "=== 1. Exemplo da especificacao, comparado caractere por caractere ==="

if ! compila "build de teste, com -DRECARGA_FIXA=2" -DRECARGA_FIXA=2; then
    confere_que_roda
fi

# Guarda uma copia do binario que o sistema aceitou executar, para o caso de o
# build seguinte ser bloqueado. Uma copia tem o mesmo conteudo, entao o Smart
# App Control decide por ela do mesmo jeito.
cp "$EXE" tp1_fixo_ok 2>/dev/null

rm -f relatorio.txt
"$EXE" "$T/oficiais/teste1.txt" > "$T/saida_obtida.txt" 2>&1

# O tr remove o '\r': no Windows a saida em modo texto sai com fim de linha
# CRLF e no Linux com LF. A comparacao e do conteudo, nao do fim de linha.
tr -d '\r' < "$T/saida_obtida.txt" > "$T/saida_obtida_lf.txt"

# A saida do programa tem, depois da moldura final, duas linhas informativas
# dizendo onde o relatorio foi gravado. O exemplo do PDF nao as mostra porque
# termina na moldura, entao o corte compara so o trecho que o PDF cobre.
FIM=$(grep -n '^=\{40\}$' "$T/saida_obtida_lf.txt" | tail -1 | cut -d: -f1)
head -n "${FIM:-0}" "$T/saida_obtida_lf.txt" > "$T/saida_obtida_corte.txt"

# A comparacao e contra saida_exemplo_esperada.txt, que e o
# saida_exemplo_pdf.txt (transcricao fiel do PDF) com a errata E05 corrigida.
# Rodar "diff testes/saida_exemplo_pdf.txt testes/saida_exemplo_esperada.txt"
# mostra exatamente qual e a diferenca: duas linhas, no bloco final.
if diff -u "$T/saida_exemplo_esperada.txt" "$T/saida_obtida_corte.txt" \
        > "$T/diff_exemplo.txt" 2>&1; then
    verde "saida do exemplo identica a esperada"
else
    vermelho "saida do exemplo difere - veja $T/diff_exemplo.txt"
    head -40 "$T/diff_exemplo.txt"
fi

# Idem para o relatorio: relatorio_exemplo_esperado.txt e o do PDF sem o recuo
# de um espaco por linha, registrado como errata E07.
if diff -u "$T/relatorio_exemplo_esperado.txt" <(tr -d '\r' < relatorio.txt) \
        > "$T/diff_relatorio.txt" 2>&1; then
    verde "relatorio identico ao esperado"
else
    vermelho "relatorio difere - veja $T/diff_relatorio.txt"
    cat "$T/diff_relatorio.txt"
fi

# ---------------------------------------------------------------------------
echo
echo "--- regras da missao, sobre o mesmo build de recarga fixa ---"

cp "$T/saida_obtida_lf.txt" "$T/saida_regras.txt"

# checa_regra <descricao> <quantidade esperada> <padrao egrep>
checa_regra() {
    local desc="$1" esperado="$2" padrao="$3" achado
    achado=$(grep -cE "$padrao" "$T/saida_regras.txt")
    if [ "$achado" -eq "$esperado" ]; then
        verde "$desc"
    else
        vermelho "$desc (encontrou $achado, esperava $esperado)"
    fi
}

# O primeiro alvo, o Axew, esta a 9.90 dos dois treinadores, que comecam em
# (0,0): e o empate. A missao tem de ir para Rosa, o treinador de id 1.
primeira=$(grep -m1 '^Missão atribuída' "$T/saida_regras.txt")
if [ "$primeira" = "Missão atribuída ao Treinador(a) Rosa." ]; then
    verde "empate de distancia vai para Rosa, o treinador de menor id"
else
    vermelho "empate nao foi para o menor id (veio: $primeira)"
fi

checa_regra "os 5 Pokemon foram capturados"             5 'capturado com sucesso!'
checa_regra "Rosa fez 4 das 5 capturas"             4 '^Missão atribuída ao Treinador\(a\) Rosa\.$'
checa_regra "Nate fez 1 captura, a do Noivern"             1 '^Missão atribuída ao Treinador\(a\) Nate\.$'
checa_regra "houve exatamente uma recarga no meio da missao"             1 'recebeu [0-9]+ Pokébolas'
checa_regra "Rosa zerou as Pokebolas duas vezes"             2 'Pokébolas restantes para o Treinador\(a\) Rosa: 0'
checa_regra "as 10 distancias do exemplo foram impressas"             10 '^Distância Treinador\(a\) '

# A segunda vez que Rosa zera e no ultimo resgate. Se a ordem dos testes
# estivesse trocada, apareceria uma segunda recarga depois dela.
if [ "$(grep -n 'Pokébolas restantes para o Treinador(a) Rosa: 0' "$T/saida_regras.txt"         | tail -1 | cut -d: -f1)" -gt      "$(grep -n 'recebeu .* Pokébolas' "$T/saida_regras.txt" | tail -1 | cut -d: -f1)" ]; then
    verde "o ultimo resgate zera as Pokebolas e NAO dispara recarga"
else
    vermelho "houve recarga depois do ultimo resgate"
fi

# As distancias exatas do exemplo do PDF, uma a uma.
for d in "Rosa: 9.90" "Nate: 9.90" "Rosa: 3.00" "Nate: 12.21" "Rosa: 1.41"          "Nate: 1.41" "Rosa: 15.56" "Nate: 14.14" "Rosa: 7.21" "Nate: 22.67"; do
    if grep -qF "Distância Treinador(a) $d" "$T/saida_regras.txt"; then
        verde "distancia do PDF conferida: $d"
    else
        vermelho "distancia do PDF ausente: $d"
    fi
done

rm -f "$T/saida_regras.txt"

# ---------------------------------------------------------------------------
echo
echo "=== 2. Casos validos: invariantes ==="

# A partir daqui o programa deveria ser exatamente o que vai na entrega.
if ! compila "build normal, como na entrega"; then
    # Todas as variantes do build da entrega foram bloqueadas pelo sistema. O
    # resto da bateria segue com o binario de recarga fixa, que e o mesmo
    # codigo com uma unica diferenca: a quantidade sorteada na recarga. Isso
    # esta avisado aqui para que o resultado nao seja lido como se fosse do
    # binario da entrega.
    echo "          AVISO: o sistema bloqueou todas as variantes deste build."
    echo "          Os testes abaixo usam o binario de recarga fixa, que e o"
    echo "          mesmo codigo com a recarga presa em 2 em vez de sorteada."
    cp tp1_fixo_ok tp1 2>/dev/null
    EXE=./tp1
    [ -f ./tp1.exe ] && cp tp1_fixo_ok tp1.exe 2>/dev/null && EXE=./tp1.exe
    confere_que_roda
fi

# checa_valido <arquivo> <quantidade esperada de Pokemon no relatorio>
checa_valido() {
    local arquivo="$1"
    local esperado="$2"
    local nome
    nome=$(basename "$arquivo")
    local saida codigo linhas

    rm -f relatorio.txt
    saida=$("$EXE" "$arquivo" 2>&1)
    codigo=$?

    if [ "$codigo" -ne 0 ]; then
        vermelho "$nome: terminou com codigo $codigo"
        return
    fi

    if echo "$saida" | grep -q 'não foram recuperados'; then
        vermelho "$nome: sobrou Pokemon na lista de fugitivos"
        return
    fi

    if echo "$saida" | grep -qE 'Pokébolas[^:]*: -'; then
        vermelho "$nome: quantidade de Pokebolas negativa"
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

    verde "$nome: $esperado recuperados, lista de fugas vazia, sem Pokebola negativa"
}

checa_valido "$T/oficiais/teste1.txt"          5
checa_valido "$T/oficiais/teste2.txt"          20
checa_valido "$T/entrada_exemplo.txt"          5
checa_valido "$T/crlf_windows.txt"             5
checa_valido "$T/empate_origem.txt"            3
checa_valido "$T/pokemon_na_origem.txt"        2
checa_valido "$T/uma_pokebola.txt"             4
checa_valido "$T/zero_pokebolas.txt"           3
checa_valido "$T/ambos_zero_pokebolas.txt"     2
checa_valido "$T/um_pokemon.txt"               1
checa_valido "$T/zero_pokemon.txt"             0
checa_valido "$T/pokedex_repetida.txt"         4
checa_valido "$T/coordenadas_negativas.txt"    3
checa_valido "$T/nomes_maximos.txt"            2
checa_valido "$T/espacos_e_linhas_extras.txt" 3
checa_valido "$T/coordenadas_grandes.txt"     3
checa_valido "$T/mesma_coordenada.txt"        4


# ---------------------------------------------------------------------------
echo
echo "=== 3. Casos invalidos: avisa o erro e nao trava ==="

# checa_invalido <arquivo ou caminho inexistente>
checa_invalido() {
    local arquivo="$1"
    local nome
    nome=$(basename "$arquivo")
    local saida codigo

    saida=$("$EXE" "$arquivo" 2>&1)
    codigo=$?

    # Codigo 0 significa que o programa aceitou dados invalidos. Codigo maior
    # que 2 significa que ele nao terminou normalmente: 126 ou 127 quando o
    # shell nao consegue nem executa-lo, 139 quando ele quebra no Linux.
    #
    # A mensagem procurada e "Erro:", exatamente como o programa escreve, e
    # nao a palavra "erro" em qualquer forma: assim uma mensagem do proprio
    # shell nao pode ser confundida com uma recusa do programa.
    if [ "$codigo" -eq 0 ]; then
        vermelho "$nome: aceitou dados invalidos"
    elif [ "$codigo" -gt 2 ]; then
        vermelho "$nome: nao terminou normalmente (codigo $codigo)"
    elif ! echo "$saida" | grep -q 'Erro:'; then
        vermelho "$nome: nao avisou o erro"
    else
        verde "$nome: recusou com mensagem de erro"
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
checa_invalido "$T/este_arquivo_nao_existe.txt"

# ---------------------------------------------------------------------------
echo
echo "=== 4. Menu e modo interativo ==="

# Opcao invalida, letra no lugar de numero, e saida pelo 0.
saida=$(printf 'x\n9\n0\n' | "$EXE" 2>&1)
codigo=$?
if [ "$codigo" -eq 0 ] && echo "$saida" | grep -q 'Opção inválida'; then
    verde "menu recusa letra e numero fora das opcoes, e sai pelo 0"
else
    vermelho "menu nao tratou entrada invalida (codigo $codigo)"
    echo "$saida" | tail -5
fi

# Fim de entrada no meio do menu (Ctrl+D no Linux, Ctrl+Z no Windows).
if printf '' | "$EXE" > /dev/null 2>&1; then
    verde "menu encerra sem erro quando a entrada termina"
else
    vermelho "menu nao tratou fim de entrada"
fi

# Modo interativo lendo os mesmos dados do exemplo pelo teclado.
rm -f relatorio.txt
saida=$(printf '2\nRosa 2\nNate 2\n5\n610 Axew Dragão 7 7\n657 Frogadier Água 10 7\n387 Turtwig Folha 1 1\n715 Noivern Voador -10 -10\n197 Umbreon Noturno 5 7\n0\n' \
        | "$EXE" 2>&1)
if echo "$saida" | grep -q 'MISSÃO CONCLUÍDA' && [ -f relatorio.txt ] \
   && [ "$(( $(wc -l < relatorio.txt) - 1 ))" -eq 5 ]; then
    verde "modo interativo executa a missao e gera o relatorio"
else
    vermelho "modo interativo nao completou a missao"
    echo "$saida" | tail -10
fi

# A lista de fugitivos registrados so aparece no modo interativo.
if echo "$saida" | grep -q 'fugitivos registrados no Centro de Pesquisa'; then
    verde "modo interativo lista os fugitivos registrados para conferencia"
else
    vermelho "modo interativo nao listou os fugitivos registrados"
fi

# Modo por arquivo escolhido pelo menu.
if printf '1\n%s\n0\n' "$T/oficiais/teste1.txt" | "$EXE" 2>&1 \
   | grep -q 'MISSÃO CONCLUÍDA'; then
    verde "menu executa a missao pela opcao de arquivo"
else
    vermelho "opcao de arquivo do menu nao completou a missao"
fi

# Arquivo inexistente digitado no menu: avisa e volta ao menu.
if printf '1\nnao_existe.txt\n0\n' | "$EXE" 2>&1 \
   | grep -q 'nao foi possivel abrir'; then
    verde "menu avisa arquivo inexistente e volta ao menu"
else
    vermelho "menu nao avisou arquivo inexistente"
fi

# ---------------------------------------------------------------------------
echo
echo "=== 5. Vazamento de memoria ==="

if command -v valgrind > /dev/null 2>&1; then
    rm -f "$T/valgrind.txt"
    if valgrind --leak-check=full --errors-for-leak-kinds=all                 --error-exitcode=99                 "$EXE" "$T/oficiais/teste2.txt" > /dev/null 2> "$T/valgrind.txt"; then
        verde "valgrind: nenhum vazamento em teste2.txt"
        grep 'ERROR SUMMARY' "$T/valgrind.txt" | sed 's/^/          /'
    else
        vermelho "valgrind encontrou problemas - veja $T/valgrind.txt"
        grep -E 'lost|ERROR SUMMARY' "$T/valgrind.txt"
    fi
else
    echo "  (valgrind nao existe neste sistema; usando contagem de malloc/free)"

    # Build de verificacao: injeta testes/conta_memoria.h somente em
    # pokelista.c, o unico arquivo do projeto que aloca e libera memoria.
    # Nenhum arquivo do projeto e alterado.
    #
    # O laco de opcoes de otimizacao existe pelo mesmo motivo do build
    # principal: gerar um binario que o sistema aceite executar.
    EXE_MEM=""
    for opt in "" "-O1" "-O2" "-O3" "-Os" "-Og" "-g" "-O1 -g" "-O2 -g" "-O3 -g"                "-O1 -fno-inline" "-O2 -fno-inline" "-O3 -fno-inline"; do
        rm -f tp1 tp1.exe ./*.o
        gcc -Wall -Wextra -std=c99 -Iinclude $opt             -include testes/conta_memoria.h -c src/pokelista.c -o pokelista.o 2>/dev/null
        gcc -Wall -Wextra -std=c99 -Iinclude $opt -c main.c -o main.o 2>/dev/null
        for m in pokemon treinador pokecenter missao; do
            gcc -Wall -Wextra -std=c99 -Iinclude $opt -c "src/$m.c" -o "$m.o" 2>/dev/null
        done
        gcc $opt -o tp1 ./*.o -lm 2>/dev/null
        candidato=./tp1
        [ -f ./tp1.exe ] && candidato=./tp1.exe
        if "$candidato" "$T/oficiais/teste1.txt" 2>&1 > /dev/null | grep -q 'conta_memoria'; then
            EXE_MEM="$candidato"
            [ -n "$opt" ] && echo "          (build de contagem gerado com $opt)"
            break
        fi
    done

    if [ -z "$EXE_MEM" ]; then
        vermelho "contagem de memoria: o sistema bloqueou todos os binarios de verificacao"
    else
        for arquivo in "$T/oficiais/teste1.txt" "$T/oficiais/teste2.txt"                        "$T/zero_pokemon.txt" "$T/pokedex_repetida.txt"                        "$T/erro_campos_faltando.txt" "$T/erro_incompleto.txt"; do
            nome=$(basename "$arquivo")
            conta=$("$EXE_MEM" "$arquivo" 2>&1 > /dev/null                     | grep 'diferenca' | tr -d ' ' | cut -d: -f2)
            if [ -z "$conta" ]; then
                vermelho "memoria em $nome: nao foi possivel medir"
            elif [ "$conta" -eq 0 ]; then
                verde "memoria em $nome: malloc = free, diferenca 0"
            else
                vermelho "memoria em $nome: sobraram $conta blocos sem free"
            fi
        done

        # Arquivo inexistente: o programa desiste no fopen, antes de alocar
        # qualquer coisa, entao nao ha nada a contar. O esperado aqui e
        # justamente nao aparecer nenhum contador.
        if "$EXE_MEM" "$T/este_arquivo_nao_existe.txt" 2>&1 > /dev/null            | grep -q 'malloc bem-sucedidos:      0'; then
            verde "memoria em arquivo inexistente: nenhuma alocacao feita"
        elif ! "$EXE_MEM" "$T/este_arquivo_nao_existe.txt" 2>&1 > /dev/null              | grep -q 'conta_memoria'; then
            verde "memoria em arquivo inexistente: nenhuma alocacao feita"
        else
            vermelho "memoria em arquivo inexistente: alocou sem precisar"
            "$EXE_MEM" "$T/este_arquivo_nao_existe.txt" 2>&1 > /dev/null | sed 's/^/          /'
        fi

        # Mostra os numeros absolutos de um caso, como evidencia.
        echo "          numeros de teste2.txt (20 Pokemon):"
        "$EXE_MEM" "$T/oficiais/teste2.txt" 2>&1 > /dev/null             | grep conta_memoria | sed 's/^/          /'
    fi

    rm -f ./*.o
fi

# ---------------------------------------------------------------------------
echo
echo "========================================"
printf 'Passou: %d    Falhou: %d\n' "$PASSOU" "$FALHOU"
echo "========================================"

rm -f "$T/saida_obtida_lf.txt" "$T/saida_obtida_corte.txt" tp1_fixo_ok

[ "$FALHOU" -eq 0 ]

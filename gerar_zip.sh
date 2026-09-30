#!/bin/bash
# ============================================================================
# Monta o .zip de entrega do TP1 e testa o pacote do zero.
#
# COMO RODAR (no Git Bash, a partir da raiz do projeto):
#
#     bash gerar_zip.sh "Aluno1" "Matricula1" "Aluno2" "Matricula2"
#
# Exemplo:
#
#     bash gerar_zip.sh "GabrielHenrique" "12345" "NomeDoColega" "67890"
#
# Trabalhando sozinho, passe apenas os dois primeiros argumentos.
#
# O nome do arquivo segue o formato que a especificacao exige:
# TP1Aluno1Matricula1Aluno2Matricula2, sem espacos e sem acentos.
#
# O que entra no pacote, conforme a especificacao ("todo codigo-fonte
# produzido, incluindo os arquivos de cabecalho" e "um conjunto de slides"):
#
#     Makefile
#     main.c
#     include/*.h
#     src/*.c
#     slides.pdf          (se existir em apresentacao/slides.pdf)
#
# O que NAO entra: a pasta descricoes/, a pasta testes/, este script, o
# CLAUDE.md, o PDF do enunciado, executaveis, arquivos-objeto e pastas
# temporarias. A especificacao pede o codigo-fonte e os slides, nao
# documentacao nem testes.
#
# Depois de montar, o script extrai o zip numa pasta temporaria, compila do
# zero com make e roda o exemplo, para conferir que o pacote esta completo.
# ============================================================================

set -u

cd "$(dirname "$0")" || exit 1

if [ "$#" -lt 2 ]; then
    echo "Uso: bash gerar_zip.sh \"Aluno1\" \"Matricula1\" [\"Aluno2\" \"Matricula2\"]"
    echo
    echo "Os nomes vao direto para o nome do arquivo, entao use-os sem espaco"
    echo "e sem acento. Exemplo:"
    echo
    echo "    bash gerar_zip.sh \"GabrielHenrique\" \"12345\" \"Colega\" \"67890\""
    exit 1
fi

ALUNO1="$1"
MATRICULA1="$2"
ALUNO2="${3:-}"
MATRICULA2="${4:-}"

NOME="TP1${ALUNO1}${MATRICULA1}${ALUNO2}${MATRICULA2}"
ZIP="${NOME}.zip"

# Recusa nome com espaco ou caractere fora de letras e numeros, que a
# especificacao nao preve e que atrapalha o envio.
if printf '%s' "$NOME" | grep -qvE '^[A-Za-z0-9]+$'; then
    echo "ERRO: o nome do pacote ficaria \"$NOME\", que tem espaco, acento ou"
    echo "      pontuacao. Passe os nomes sem espaco e sem acento."
    exit 1
fi

# ---------------------------------------------------------------------------
echo "=== 1. Conferindo o que vai no pacote ==="

ARQUIVOS="Makefile main.c"
for f in include/*.h src/*.c; do
    ARQUIVOS="$ARQUIVOS $f"
done

FALTANDO=""
for f in $ARQUIVOS; do
    [ -f "$f" ] || FALTANDO="$FALTANDO $f"
done
if [ -n "$FALTANDO" ]; then
    echo "ERRO: faltam arquivos:$FALTANDO"
    exit 1
fi

if [ -f apresentacao/slides.pdf ]; then
    ARQUIVOS="$ARQUIVOS apresentacao/slides.pdf"
    echo "  apresentacao/slides.pdf sera incluido"
else
    echo "  AVISO: apresentacao/slides.pdf nao existe."
    echo "         A especificacao exige os slides em PDF dentro do zip."
    echo "         O pacote vai sair sem eles."
fi

for f in $ARQUIVOS; do
    printf '  %s\n' "$f"
done

# ---------------------------------------------------------------------------
echo
echo "=== 2. Montando $ZIP ==="

rm -f "$ZIP"

# Tres formas de gravar o zip, em ordem de preferencia:
#
#   1. o comando zip, que e o mais previsivel e existe no Linux
#   2. o tar do Windows 10 e 11 (C:\Windows\System32\tar.exe), que e o bsdtar
#      e grava zip de verdade com -a. Cuidado: o tar do Git Bash e o GNU tar,
#      em que -a quer dizer outra coisa e o resultado NAO e um zip.
#   3. Compress-Archive, do PowerShell
TAR_WINDOWS=/c/Windows/System32/tar.exe

if command -v zip > /dev/null 2>&1; then
    echo "  usando o comando zip"
    zip -q -r "$ZIP" $ARQUIVOS
elif [ -x "$TAR_WINDOWS" ] && "$TAR_WINDOWS" --version 2>&1 | grep -qi bsdtar; then
    echo "  usando o tar do Windows (bsdtar)"
    "$TAR_WINDOWS" -a -c -f "$ZIP" $ARQUIVOS
elif command -v tar > /dev/null 2>&1 && tar --version 2>&1 | grep -qi bsdtar; then
    echo "  usando o tar do sistema (bsdtar)"
    tar -a -c -f "$ZIP" $ARQUIVOS
elif command -v powershell > /dev/null 2>&1; then
    echo "  usando Compress-Archive do PowerShell"
    LISTA=$(printf '%s,' $ARQUIVOS | sed 's/,$//')
    powershell -NoProfile -Command \
        "Compress-Archive -Path $LISTA -DestinationPath '$ZIP' -Force" > /dev/null
else
    echo "ERRO: nao achei o comando zip, um bsdtar nem o PowerShell."
    exit 1
fi

if [ ! -f "$ZIP" ]; then
    echo "ERRO: o pacote nao foi criado."
    exit 1
fi

echo "  $ZIP criado ($(wc -c < "$ZIP") bytes)"

# ---------------------------------------------------------------------------
echo
echo "=== 3. Testando o pacote do zero ==="

TEMP="teste_do_zip"
rm -rf "$TEMP"
mkdir -p "$TEMP"

if command -v unzip > /dev/null 2>&1; then
    unzip -q "$ZIP" -d "$TEMP"
else
    tar -x -f "$ZIP" -C "$TEMP"
fi

echo "  conteudo extraido:"
(cd "$TEMP" && find . -type f | sort | sed 's/^/    /')

echo
echo "  compilando do zero:"
SAIDA=$( (cd "$TEMP" && gcc -Wall -Wextra -std=c99 -g -Iinclude -o tp1 main.c src/*.c -lm) 2>&1 )
if [ -n "$SAIDA" ]; then
    echo "  FALHOU: a compilacao do pacote deu avisos ou erros:"
    echo "$SAIDA" | sed 's/^/    /'
    exit 1
fi
echo "    zero avisos"

echo
echo "  rodando o exemplo:"
cp testes/oficiais/teste1.txt "$TEMP/" 2>/dev/null

# O programa so funciona pelo menu, entao o teste alimenta a entrada padrao:
# 1 para abrir um arquivo, o caminho, e 0 para sair.
#
# O Smart App Control do Windows 11 bloqueia executaveis sem assinatura de
# forma imprevisivel. O laco tenta opcoes de otimizacao diferentes, que geram
# binarios diferentes, ate um deles conseguir rodar.
OK=0
for opt in "" "-O1" "-O2" "-O3" "-Os" "-Og" "-g"; do
    (cd "$TEMP" && rm -f tp1 tp1.exe && \
     gcc -Wall -Wextra -std=c99 -Iinclude $opt -o tp1 main.c src/*.c -lm 2>/dev/null)
    EXE="$TEMP/tp1"
    [ -f "$TEMP/tp1.exe" ] && EXE="$TEMP/tp1.exe"
    if (cd "$TEMP" && printf '1
teste1.txt
0
'          | "./$(basename "$EXE")" 2>/dev/null) | grep -q 'MISSÃO CONCLUÍDA'; then
        OK=1
        break
    fi
done

if [ "$OK" -eq 1 ]; then
    echo "    a missao rodou ate o fim"
    if [ -f "$TEMP/relatorio.txt" ]; then
        LINHAS=$(( $(wc -l < "$TEMP/relatorio.txt") - 1 ))
        echo "    relatorio gerado com $LINHAS Pokemon"
        if diff -q <(tr -d '\r' < "$TEMP/relatorio.txt") \
                   testes/relatorio_exemplo_esperado.txt > /dev/null 2>&1; then
            echo "    relatorio identico ao esperado"
        else
            echo "    AVISO: o relatorio difere do esperado (a recarga e sorteada,"
            echo "           entao a ordem pode mudar de uma execucao para outra)"
        fi
    else
        echo "    FALHOU: o relatorio nao foi gerado"
        exit 1
    fi
else
    echo "    NAO FOI POSSIVEL EXECUTAR o binario gerado a partir do pacote."
    echo "    A compilacao passou com zero avisos, entao o pacote esta completo,"
    echo "    mas a execucao foi bloqueada. No Windows 11 isso normalmente e o"
    echo "    Smart App Control. Confira com:"
    echo
    echo "      Get-WinEvent -LogName Microsoft-Windows-CodeIntegrity/Operational -MaxEvents 5"
fi

rm -rf "$TEMP"

# ---------------------------------------------------------------------------
echo
echo "========================================"
echo "Pronto: $ZIP"
echo "========================================"
echo
echo "Antes de enviar no Moodle, confira:"
echo "  - apresentacao/slides.pdf entrou no pacote"
echo "  - o nome do arquivo esta com os nomes e as matriculas certos"

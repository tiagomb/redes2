#!/bin/bash

# Verificar se foi fornecido um argumento
if [ $# -eq 0 ]; then
    echo "Erro: Você deve fornecer um argumento (TCP ou UDP)."
    exit 1
fi

# Captura o argumento passado para o script
entrada=$1

# Verificar a entrada e executar o comando correspondente
if [ "$entrada" == "TCP" ]; then
    ./TCP -s 1024
    sleep 3
    ./TCP -s 2048
elif [ "$entrada" == "UDP" ]; then
    ./UDP -s 1024
    sleep 3
    ./UDP -s 2048
else
    echo "Entrada inválida. Digite 'TCP' ou 'UDP'."
    exit 1
fi

#!/bin/bash

#Verificar se foi fornecido um argumento
if [ $# -eq 0 ]; then
    echo "Erro: Você deve fornecer um argumento (TCP ou UDP)."
    exit 1
fi

entrada=$1

#Verificar se a conexão será TCP ou UDP
if [ "$entrada" == "TCP" ]; then
    ./TCP -c 1024
    sleep 10
    ./TCP -c 2048
elif [ "$entrada" == "UDP" ]; then
    ./UDP -c 1024
    sleep 10
    ./UDP -c 2048
else
    echo "Entrada inválida. Digite 'TCP' ou 'UDP'."
    exit 1
fi

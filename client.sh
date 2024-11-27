#!/bin/bash

if [ -f Client_TCP.txt ]; then
    rm Client_TCP.txt
fi

if [ -f Client_UDP.txt ]; then
    rm Client_UDP.txt
fi

if [ -f Client_UDP_NOCHECK.txt ]; then
    rm Client_UDP_NOCHECK.txt
fi

if [ -f Client_TCP_LOG.txt ]; then
    rm Client_TCP_LOG.txt
fi

if [ -f Client_UDP_LOG.txt ]; then
    rm Client_UDP_LOG.txt
fi

if [ -f Client_UDP_NOCHECK_LOG.txt ]; then
    rm Client_UDP_NOCHECK_LOG.txt
fi

packets=(1 5 10 50 100 500 1000 5000 10000 50000 100000)

for i in "${packets[@]}"
do
    ./TCP -c $i >> Client_TCP_LOG.txt
    sleep 4
done

for i in "${packets[@]}"
do
    ./UDP -c $i >> Client_UDP_LOG.txt
    sleep 4
done

for i in "${packets[@]}"
do
    ./UDP -c $i -d >> Client_UDP_NOCHECK_LOG.txt
    sleep 4
done

gnuplot -p -e "set title 'Comparação entre TCP e UDP'; set xlabel 'Número de Pacotes'; set ylabel 'Tempo (em segundos)'; plot 'Client_TCP_LOG.txt' using 1:2 with lines title 'TCP', 'Client_UDP_LOG.txt' using 1:2 with lines title 'UDP'"

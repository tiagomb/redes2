#!/bin/bash

if [ -f Server_TCP.txt ]; then
    rm Server_TCP.txt
fi

if [ -f Server_UDP.txt ]; then
    rm Server_UDP.txt
fi

if [ -f Server_UDP_NOCHECK.txt ]; then
    rm Server_UDP_NOCHECK.txt
fi

if [ -f Server_TCP_LOG.txt ]; then
    rm Server_TCP_LOG.txt
fi

if [ -f Server_UDP_LOG.txt ]; then
    rm Server_UDP_LOG.txt
fi

if [ -f Server_UDP_NOCHECK_LOG.txt ]; then
    rm Server_UDP_NOCHECK_LOG.txt
fi

packets=(1 5 10 50 100 500 1000 5000 10000 50000 100000)

for i in "${packets[@]}"
do
    ./TCP -s $i >> Server_TCP_LOG.txt
done

sleep 3

for i in "${packets[@]}"
do
    ./UDP -s $i >> Server_UDP_LOG.txt
done

for i in "${packets[@]}"
do
    ./UDP -s $i -d >> Server_UDP_NOCHECK_LOG.txt
done

gnuplot -p -e "set title 'Comparação entre TCP e UDP'; set xlabel 'Número de Bytes enviados'; set ylabel 'Porcentagem recebida'; plot 'Server_TCP.txt' using 1:2 with linespoints pointtype 7 title 'TCP', 'Server_UDP.txt' using 1:2 with linespoints pointtype 7 title 'UDP', 'Server_UDP_NOCHECK.txt' using 1:2 with linespoints pointtype 7 title 'UDP\_NOCHECK'"



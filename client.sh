#!/bin/bash

packets=(1 5 10 100 500 1000 5000 10000 50000 100000)

for i in "${packets[@]}"
do
    ./TCP -c $i
    sleep 4
done

for i in "${packets[@]}"
do
    ./UDP -c $i
    sleep 4
done
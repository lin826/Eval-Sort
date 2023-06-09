#!/bin/bash

for k in 1 5 10 25 50 100; do
    OUTPUT="./workloads/createdata1_40MB_K"${k}"_L1.txt"
    ./sortedness_data_generator -N 10000000 -K $k -L 1 -o $OUTPUT -S 0 -a 1 -b 1 -P 0
done

for l in 1 5 10 25 50 100; do
    OUTPUT="./workloads/createdata2_40MB_K1_L"${l}".txt"
    ./sortedness_data_generator -N 10000000 -K 1 -L $l -o $OUTPUT -S 0 -a 1 -b 1 -P 0
done

for ((k=0; k<=100; k+=10)); do
    for ((l=0; l<=100; l+=10)); do
        OUTPUT="./workloads/createdata3_40MB_K"${k}"_L"${l}".txt"
        ./sortedness_data_generator -N 10000000 -K $k -L $l -o $OUTPUT -S 0 -a 1 -b 1 -P 0
    done
done

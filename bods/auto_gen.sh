#!/bin/bash

for ((k=0; k<=100; k+=10)); do
    for ((l=0; l<=100; l+=10)); do
        OUTPUT="./workloads/createdata_40MB_K"${k}"_L"${l}".txt"
        ./sortedness_data_generator -N 10000000 -K $k -L $l -o $OUTPUT -S 0 -a 1 -b 1 -P 0
    done
done

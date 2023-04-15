#!/bin/bash

g++ main.cpp -o main.out

OUTPUT="./output/result.csv"
declare -a ALGOS=("std_stable" "insertion_sort" "merge_sort" "quick_sort" "radix_sort" "selection_sort" "tim_sort")

for ALGO in "${ALGOS[@]}"
do
    for k in 1 5 10 25 50 100; do
        INPUT="./bods/workloads/createdata1_40MB_K"${k}"_L1.txt"
        ./main.out $INPUT $OUTPUT $ALGO
    done

    for l in 1 5 10 25 50 100; do
        INPUT="./bods/workloads/createdata2_40MB_K1_L"${l}".txt"
        ./main.out $INPUT $OUTPUT $ALGO
    done

    for ((k=0; k<=100; k+=10)); do
        for ((l=0; l<=100; l+=10)); do
            INPUT="./bods/workloads/createdata3_40MB_K"${k}"_L"${l}".txt"
            ./main.out $INPUT $OUTPUT $ALGO
        done
    done
done

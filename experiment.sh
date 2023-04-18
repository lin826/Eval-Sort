#!/bin/bash

g++ main.cpp -o main

OUTPUT="./output/result.csv"
declare -a ALGOS=("std_stable" "insertion_sort" "merge_sort" "quick_sort" "radix_sort" "selection_sort" "tim_sort")

clear_cache () {
    echo 3 > /proc/sys/vm/drop_caches && swapoff -a && swapon -a && printf '\n%s\n' 'Ram-cache and Swap Cleared';
}

for ALGO in "${ALGOS[@]}"
do
    for k in 100 50 25 10 5 1; do
        INPUT="./bods/workloads/createdata1_40MB_K"${k}"_L1.txt"
        ./main $INPUT $OUTPUT $ALGO
        clear_cache
    done

    for l in 1 5 10 25 50 100; do
        INPUT="./bods/workloads/createdata2_40MB_K1_L"${l}".txt"
        ./main $INPUT $OUTPUT $ALGO
        clear_cache
    done

    for ((k=0; k<=100; k+=10)); do
        for ((l=0; l<=100; l+=10)); do
            INPUT="./bods/workloads/createdata3_40MB_K"${k}"_L"${l}".txt"
            ./main $INPUT $OUTPUT $ALGO
            clear_cache
        done
    done
done

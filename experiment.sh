#!/bin/bash

OUTPUT="./output/result_april26.csv"

# screen -S k-${k}-${ALGO} -dm bash -c ""
g++ -std=c++11 -Wall ./src/main.cpp -o ./main.out

clear_cache () {
    # Clean cache
    # https://stackoverflow.com/questions/28845524/echo-3-proc-sys-vm-drop-caches-on-mac-osx
    # https://gist.github.com/dan-palmer/3082266
    sync && sudo purge;
#     launchctl unload -w /System/Library/LaunchDaemons/com.apple.dynamic_pager.plist;
#     rm /private/var/vm/swapfile*;
#     launchctl load /System/Library/LaunchDaemons/com.apple.dynamic_pager.plist;
}

ALGO="kl_sort"
for kdiv in 100 50 25 10 5 1; do
    for ldiv in 100 50 25 10 5 1; do
        for k in 100 50 25 10 5 1; do
            INPUT="./bods/workloads/createdata1_40MB_K"${k}"_L1.txt"
            ./main.out $INPUT $OUTPUT $ALGO $kdiv $ldiv
            clear_cache
        done

        INPUT="./bods/workloads/createdata3_40MB_K0_L0.txt"
        ./main.out $INPUT $OUTPUT $ALGO $kdiv $ldiv
        clear_cache

        for l in 1 5 10 25 50 100; do
            INPUT="./bods/workloads/createdata2_40MB_K1_L"${l}".txt"
            ./main.out $INPUT $OUTPUT $ALGO $kdiv $ldiv
            clear_cache
        done
    done
done

declare -a ALGOS=("radix_sort" "std_stable" "merge_sort" "quick_sort" "tim_sort" "insertion_sort")
for ALGO in "${ALGOS[@]}"
do

    for k in 100 50 25 10 5 1; do
        INPUT="./bods/workloads/createdata1_40MB_K"${k}"_L1.txt"
        ./main.out $INPUT $OUTPUT $ALGO
        clear_cache
    done

    INPUT="./bods/workloads/createdata3_40MB_K0_L0.txt"
    ./main.out $INPUT $OUTPUT $ALGO
    clear_cache

    for l in 1 5 10 25 50 100; do
        INPUT="./bods/workloads/createdata2_40MB_K1_L"${l}".txt"
        ./main.out $INPUT $OUTPUT $ALGO
        clear_cache
    done

    for ((k=0; k<=100; k+=10)); do
        for ((l=0; l<=100; l+=10)); do
            INPUT="./bods/workloads/createdata3_40MB_K"${k}"_L"${l}".txt"
            ./main.out $INPUT $OUTPUT $ALGO
            clear_cache
        done
    done
done

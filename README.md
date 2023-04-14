# CS561-Final-Sort

## Generate Workload

<!-- TODO: Add instructions to use BoDS -->

## Experiment

### Compile

```sh
g++ main.cpp -o main.out
```

### Run

```sh
./main.out <INPUT_FILE_PATH> <OUTPUT_FILE_PATH> <SORTING_ALGO>
```

The arguments include `INPUT_FILE_PATH`, `OUTPUT_FILE_PATH`, and `SORTING_ALGO`.

For example, we want to take `./bods/workloads/createdata_K100_L100.txt` as the input workload to sort with `std_stable`, and an output result saved to `./output/K100_L100.csv`. Then the command should be:

```sh
./main.out ./bods/workloads/createdata_K100_L100.txt ./output/K100_L100.csv std_stable
```

#### SORTING_ALGO selections

- std_stable
- insertion_sort
- merge_sort
- quick_sort
- radix_sort
- selection_sort
- tim_sort

## Sorting Algorithms

- [x] std::stable_sort
- [x] Insertion Sort
- [x] Merge Sort
- [x] Quick Sort
- [x] Radix Sort
- [x] Selection Sort
- [x] Tim Sort
- [ ] KL-addaptive Sort
- [ ] Hybrid Sort

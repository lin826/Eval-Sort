# Eval-Sort

## Generate Workload

<!-- TODO: Add instructions to use BoDS -->
Refer to [BU-DiSC/BoDS](https://github.com/BU-DiSC/bods), we compile the generator then move the resulting executable file under `./bods/` folder.

```sh
cd ./bods
mv <GENERATOR_FILE_PATH> ./sortedness_data_generator
bash ./auto_gen.sh
```

### Create scrambled data

```sh
g++ ./src/shuffle.cpp -o ./shuffle.out
./shuffle.out <SIZE_OF_ARRAY> > <OUTPUTFILE> 
```

## Experiment

To replicate the experiment, simply run `bash experiment.sh`.

The followings are step-by-step instructions.

### Compile

```sh
g++ -std=c++11 ./src/main.cpp -o ./main.out
```

### Run

```sh
./main.out <INPUT_FILE_PATH> <OUTPUT_FILE_PATH> <SORTING_ALGO> <K_DIV> <L_DIV> <KL_ESTIMATIOM>
```

The arguments include `INPUT_FILE_PATH`, `OUTPUT_FILE_PATH`, `SORTING_ALGO`, `K_DIV`, `L_DIV`, and `KL_ESTIMATIOM`. The last three arguments are optional and only work under `kl_sort`.

For example, we want to take `./bods/workloads/createdata_K100_L100.txt` as the input workload to sort with `std_stable`, and an output result saved to `./output/K100_L100.csv`. Then the command should be:

```sh
./main.out ./bods/workloads/createdata_K100_L100.txt ./output/K100_L100.csv std_stable
```

#### Sorting Algorithms

- std_stable
- insertion_sort
- merge_sort
- quick_sort
- radix_sort
- selection_sort
- tim_sort
- kl_sort
<!-- - [ ] Heap Sort -->
<!-- - [ ] K Sort -->
<!-- - [ ] KL-addaptive Sort -->
<!-- - [ ] Hybrid Sort -->

#### (K, L) Estimations

- 'a' <!-- TODO: Explaination -->
- 'b' <!-- TODO: Explaination -->
- 'c' <!-- TODO: Explaination -->

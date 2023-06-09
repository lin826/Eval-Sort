#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "kl_sort.h"

using namespace std;

const int RUN = 32; // TimSort individual subarrays of size RUN

namespace SortingEnum
{
    // Reference: https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
    enum SortingType {
        std_stable,
        insertion_sort,
        merge_sort,
        quick_sort,
        radix_sort,
        selection_sort,
        tim_sort,
        kl_sort,
        invalid_sort,
    };
    // static const SortingType All[] = { std_stable, insertion_sort, merge_sort, quick_sort, radix_sort, selection_sort, tim_sort };
    static SortingType getType(string s) {
        if (s == "std_stable") {
            return std_stable;
        }
        if (s == "insertion_sort") {
            return insertion_sort;
        }
        if (s == "merge_sort") {
            return merge_sort;
        }
        if (s == "quick_sort") {
            return quick_sort;
        }
        if (s == "radix_sort") {
            return radix_sort;
        }
        if (s == "selection_sort") {
            return selection_sort;
        }
        if (s == "tim_sort") {
            return tim_sort;
        }
        if (s == "kl_sort") {
            return kl_sort;
        }
        return invalid_sort;
    }
}

int getWorkloadLen(string filename) {
    // Reference: https://stackoverflow.com/questions/3072795/how-to-count-lines-of-a-file-in-c
    ifstream myfile(filename);
    return count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
}

bool readWorkload(string filename, int workload_arr[])
{
    ifstream myfile(filename, ios_base::in);
    if ( myfile.is_open() ) {
        int i = 0;
        while ( myfile >> workload_arr[i]) {
            i += 1;
        }
        myfile.close();
        return true;
    } else {
        return false;
    }
}

void writeResult(
    string filename,
    int k,
    int k_div,
    int l,
    int l_div,
    string algo,
    long long duration_nanoseconds,
    string estimation
) {
    ofstream myfile(filename, ios_base::app);
    // myfile << "./bods/workloads/createdata_K" << k << "_L" << l << ".txt,";
    myfile << k << "," << k_div << ",";
    myfile << l << "," << l_div << ",";
    myfile << algo << ",";
    myfile << duration_nanoseconds << ",";
    myfile << estimation << "\n";
    myfile.close();
}

void printArray(int arr[], int len)
{
    cout << arr[0] << ", " << arr[1] << ", " << arr[2] << ", ..., " ;
    cout << arr[len - 2] << ", " << arr[len - 1] << endl;
}

void stdStableSort(int start[], int end[]) {
    stable_sort(start, end);
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = new int[n1], *M = new int[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[mid + 1 + j];

    int i=0, j=0, k=left;
    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i]; i++;
        } else {
            arr[k] = M[j]; j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i]; i++; k++;
    }
    while (j < n2) {
        arr[k] = M[j]; j++; k++;
    }
    delete [] L;
    delete [] M;
}

void mergeSort(int array[], const int begin, const int end)
{
    if (begin >= end)
        return; // Returns recursively

    const int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void insertionSort(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp)
        {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = temp;
    }
}

void swap(int arr[], int i, int j) {
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

int compare(const void* a, const void* b)
{
	const int* x = (int*) a;
	const int* y = (int*) b;

	if (*x > *y)
		return 1;
	else if (*x < *y)
		return -1;

	return 0;
}

int getMax(int arr[], int size)
{
    int max = arr[0];
    for (int i = 1; i < size; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void CountingSort(int arr[], int size, int div)
{
    int *output = new int[size];
    int count[10] = {0};

    for (int i = 0; i < size; i++)
        count[ (arr[i]/div)%10 ]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = size - 1; i >= 0; i--)
    {
        output[count[ (arr[i]/div)%10 ] - 1] = arr[i];
        count[ (arr[i]/div)%10 ]--;
    }

    for (int i = 0; i < size; i++)
        arr[i] = output[i];

    delete [] output;
}

void radixSort(int arr[], int size)
{
    // Reference: https://simplesnippets.tech/radix-sort-algorithm-with-c-code-sorting-algorithms-data-structures-algorithms/
    int m = getMax(arr, size);
    for (int div = 1; m/div > 0; div *= 10)
        CountingSort(arr, size, div);
}

void selectionSort(int array[], int size) {
    // Reference: https://www.programiz.com/dsa/selection-sort
    for (int step = 0; step < size - 1; step++) {
        int min_idx = step;
        for (int i = step + 1; i < size; i++) {
            // To sort in descending order, change > to < in this line.
            // Select the minimum element in each loop.
            if (array[i] < array[min_idx])
                min_idx = i;
        }
        // put min at the correct position
        swap(array, min_idx, step);
    }
}

void timSort(int arr[], int n)
{
    // Reference: https://www.geeksforgeeks.org/timsort/
    for (int i = 0; i < n; i+=RUN)
        insertionSort(arr, i, min((i+RUN-1), (n-1)));
    for (int size = RUN; size < n; size = 2*size)
    {
        for (int left = 0; left < n; left += 2*size)
        {
            int mid = left + size - 1;
            int right = min((left + 2*size - 1), (n-1));
            if(mid < right)
                merge(arr, left, mid, right);
        }
    }
}

string get_target(string s, string begin_s, string end_s) {
    int shift_i = begin_s.size();
    size_t i = s.find(begin_s);
    size_t j = s.substr(i + shift_i, s.size()-i).find(end_s);
    return s.substr(i + shift_i, j);
}

int get_k(string s) {
    string k = get_target(s, "_K", "_L").c_str();
    cout << "K: " << k << endl;
    return atoll(k.c_str());
}

int get_l(string s) {
    string l = get_target(s, "_L", ".txt").c_str();
    cout << "L: " << l << endl;
    return atoll(l.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Error: Plsease specify input/output file and the sorting algorithm." << endl;
        return -1;
    }
    cout << "================================\n";
    cout << "Input file: " << argv[1] << "\n";
    cout << "Output file: " << argv[2] << "\n";
    cout << "Algorithm: " << argv[3] << "\n";

    const string INPUT_FILE = argv[1];
    const int k = get_k(INPUT_FILE), l = get_l(INPUT_FILE);
    const string OUTPUT_FILE = argv[2];
    const string ALGO = argv[3];

    int k_div = 1, l_div = 1;
    char estimation_type;
    if (argc == 6) {
        const string K_DIV = argv[4];
        k_div = atoll(K_DIV.c_str());
        const string L_DIV = argv[5];
        l_div = atoll(L_DIV.c_str());
    } else if (argc == 5) {
        estimation_type = argv[4][0];
    } else {
        cout << "Warning: Using default k_div=1 and l_div=1." << endl;
    }

    const int N_SIZE = getWorkloadLen(INPUT_FILE);
    cout << "N_SIZE: " << N_SIZE << "\n";
    int *workload = new int[N_SIZE];
    bool read_result = readWorkload(INPUT_FILE, workload);
    if (read_result == false) {
        cout << "Error: Failed to open the file." << endl;
        return -1;
    }
    int *OUT = new int[N_SIZE + 1];
    bool sort_result = true;
    cout << "================================\n";
    cout << "Start sorting...\n";
    auto start_time = chrono::high_resolution_clock::now();
    switch (SortingEnum::getType(ALGO)) {
        case SortingEnum::std_stable:
            stdStableSort(workload, workload + N_SIZE);
            break;
        case SortingEnum::insertion_sort:
            insertionSort(workload, 0, N_SIZE);
            break;
        case SortingEnum::merge_sort:
            mergeSort(workload, 0, N_SIZE);
            break;
        case SortingEnum::quick_sort:
            // Reference: https://www.programiz.com/cpp-programming/library-function/cstdlib/qsort
            qsort(workload, N_SIZE, sizeof(int), compare);
            break;
        case SortingEnum::radix_sort:
            radixSort(workload, N_SIZE);
            break;
        case SortingEnum::selection_sort:
            selectionSort(workload, N_SIZE);
            break;
        case SortingEnum::tim_sort:
            timSort(workload, N_SIZE);
            break;
        case SortingEnum::kl_sort:
            if (estimation_type == 'a') {
                sort_result = kl_search_a(workload, OUT, N_SIZE);
            } else if (estimation_type == 'b') {
                sort_result = kl_search_b(workload, OUT, N_SIZE);
            } else if (estimation_type == 'c') {
                sort_result = kl_search_c(workload, OUT, N_SIZE);
            } else {
                sort_result = kl_sort(workload, OUT, N_SIZE, k * (N_SIZE / 100) / k_div, l * (N_SIZE / 100) / l_div);
            }
            break;
        default:
            cout << "Error: Cannot find the sort type.\n";
            return 0;
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto proc_time = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

    if (!sort_result) {
        printArray(workload, N_SIZE);
    }
   
    writeResult(OUTPUT_FILE, k, k_div, l, l_div, ALGO, proc_time.count(), argv[4]);
    cout << INPUT_FILE << "(" << ALGO << "): " << proc_time.count() << " nanoseconds" << endl;

    delete [] workload;
    delete [] OUT;
}
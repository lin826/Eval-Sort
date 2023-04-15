#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

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
        invalid_sort,
    };
    static const SortingType All[] = { std_stable, insertion_sort, merge_sort, quick_sort, radix_sort, selection_sort, tim_sort };
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
        return invalid_sort;
    }
}

int getWorkloadLen(string filename) {
    // Reference: https://stackoverflow.com/questions/3072795/how-to-count-lines-of-a-file-in-c
    ifstream myfile(filename);
    return count(istreambuf_iterator<char>(myfile), istreambuf_iterator<char>(), '\n');
}

int readWorkload(string filename, int workload_arr[])
{
    ifstream myfile(filename, ios_base::in);
    if ( myfile.is_open() ) {
        int i = 0;
        while ( myfile >> workload_arr[i]) {
            i += 1;
        }
        myfile.close();
        return 0;
    } else {
        cout << "Error: Failed to open the file." << endl;
        return -1;
    }
}

void writeResult(string filename, string input, string algo, int duration_nanoseconds) {
    ofstream myfile(filename, ios_base::app);
    myfile << input << ",";
    myfile << algo << ",";
    myfile << duration_nanoseconds << "\n";
    myfile.close();
}

void printArray(int arr[], int len)
{
    cout << arr[0] << ", " << arr[1] << ", ..., " << arr[len - 1] << endl;
}

void stdStableSort(int start[], int end[]) {
    stable_sort(start, end);
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], M[n2];
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

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int array[], int low, int high) {
    int pivot = array[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(int array[], int low, int high) {
    // Reference: https://www.programiz.com/dsa/quick-sort

    if (low < high) {
        int pi = partition(array, low, high);

        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
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
    int output[size]; 
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
        swap(&array[min_idx], &array[step]);
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
    const string OUTPUT_FILE = argv[2];
    const string ALGO = argv[3];

    const int N_SIZE = getWorkloadLen(INPUT_FILE);
    cout << "N_SIZE: " << N_SIZE << "\n";
    int* workload = new int[N_SIZE];
    int read_result = readWorkload(INPUT_FILE, workload);
    if (read_result < 0) {
        return -1;
    }
    
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
            quickSort(workload, 0, N_SIZE);
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
        default:
            cout << "Error: Cannot find the sort type.\n";
            return 0;
    }
    // printArray(workload, N_SIZE); // DEBUG
    auto end_time = chrono::high_resolution_clock::now();
    auto proc_time = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    writeResult(OUTPUT_FILE, INPUT_FILE, ALGO, proc_time.count());
    cout << INPUT_FILE << "(" << ALGO << "): " << proc_time.count() << " nanoseconds" << endl;
}
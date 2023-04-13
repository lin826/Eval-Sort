#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

const int LEN = 1000000;
const int RUN = 32; // TimSort individual subarrays of size RUN

const string OUTPUT_FILE = "./experiment_result.csv";

namespace SortingEnum
{
    // Reference: https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
    enum SortingType { std_stable, insertion_sort, merge_sort, quick_sort, selection_sort, tim_sort };
    static const SortingType All[] = { std_stable, insertion_sort, merge_sort, quick_sort, selection_sort, tim_sort };
}

void readWorkload(string filename, int workload_arr[])
{
    ifstream myfile(filename, ios_base::in);
    if ( myfile.is_open() ) {
        int i = 0;
        while ( myfile >> workload_arr[i]) {
            i += 1;
        }
    } else {
        cout << "Failed to open the file." << endl;
    }
    myfile.close();
}

void writeColumnNames(string filename) {
    ofstream myfile(filename);
    myfile << "Type, K, L, Runtime\n";
    myfile.close();
}

void writeResult(string filename, int sorting_type, int k, int l, float proc_time) {
    ofstream myfile(filename, ios_base::app);
    myfile << sorting_type << ",";
    myfile << k << ",";
    myfile << l << ",";
    myfile << proc_time << "\n";
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

void quickSort(int array[], int low, int high) {
    // Reference: https://www.programiz.com/dsa/quick-sort
    auto partition = [](int array[], int low, int high) {
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
    };

    if (low < high) {
        int pi = partition(array, low, high);

        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
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

int main() {
    writeColumnNames(OUTPUT_FILE);

    int workload[LEN];
    for (const SortingEnum::SortingType sort_type : SortingEnum::All) {
        for (int k=0; k<=100; k+=10) {
            for (int l=0; l<=100; l+=10) {
                string input_file = "./bods/workloads/createdata_K" + to_string(k) + "_L" + to_string(l) + ".txt";
                readWorkload(input_file, workload);
                
                time_t start_time = clock();
                switch (sort_type) {
                    case SortingEnum::std_stable:
                        stdStableSort(workload, workload + LEN);
                        break;
                    case SortingEnum::insertion_sort:
                        insertionSort(workload, 0, LEN);
                        break;
                    case SortingEnum::merge_sort:
                        mergeSort(workload, 0, LEN);
                        break;
                    case SortingEnum::quick_sort:
                        quickSort(workload, 0, LEN);
                        break;
                    case SortingEnum::selection_sort:
                        selectionSort(workload, LEN);
                        break;
                    case SortingEnum::tim_sort:
                        timSort(workload, LEN);
                        break;
                    default:
                        cout << "Cannot find the sort type." << endl;
                        return 0;
                }
                // printArray(workload, len); // DEBUG

                float proc_time = (float)(clock() - start_time) / CLOCKS_PER_SEC;
                writeResult(OUTPUT_FILE, sort_type, k, l, proc_time);
                cout << input_file << "[" << sort_type << "]: " << proc_time << " seconds" << endl;
            }
        }
    }
}
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;
namespace SortingEnum
{
    // Reference: https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum
    enum SortingType { std_stable, merge_sort };
    static const SortingType All[] = { std_stable, merge_sort };
}

void read_workload(string filename, int workload_arr[])
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

void write_column_name(string filename) {
    ofstream myfile(filename);
    myfile << "Type, K, L, Runtime\n";
    myfile.close();
}

void write_result(string filename, int sorting_type, int k, int l, float proc_time) {
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

void std_stable_sort(int start[], int end[]) {
    stable_sort(start, end);
}

void mergeSort(int array[], const int begin, const int end)
{
    if (begin >= end)
        return; // Returns recursively
 
    const int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);

    const int subArrayOne = mid - begin + 1;
    const int subArrayTwo = end - mid;

    // Create temp arrays
    int *leftArray = new int[subArrayOne], *rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = array[begin + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    int indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0, // Initial index of second sub-array
        indexOfMergedArray = begin; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        } else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

int main() {
    const string OUTPUT_FILE = "./experiment_result.csv";
    write_column_name(OUTPUT_FILE);

    const int len = 1000000;
    int workload[len];

    for (const SortingEnum::SortingType sort_type : SortingEnum::All) {
        for (int k=0; k<=100; k+=10) {
            for (int l=0; l<=100; l+=10) {
                string input_file = "./bods/workloads/createdata_K" + to_string(k) + "_L" + to_string(l) + ".txt";
                read_workload(input_file, workload);
                
                time_t start_time = clock();
                switch (sort_type) {
                    case SortingEnum::std_stable:
                        std_stable_sort(workload, workload + len);
                        break;
                    case SortingEnum::merge_sort:
                        mergeSort(workload, 0, len);
                        break;
                    default:
                        cout << "Cannot find the sort type." << endl;
                        return 0;
                }
                // printArray(workload, len); // DEBUG

                float proc_time = (float)(clock() - start_time) / CLOCKS_PER_SEC;
                write_result(OUTPUT_FILE, sort_type, k, l, proc_time);
                cout << input_file << "[" << sort_type << "]: " << proc_time << " seconds" << endl;
            }
        }
    }
}
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;


void read_workload(string filename, int* workload_arr)
{
    fstream myfile(filename, ios_base::in);

    int i = 0;
    if ( myfile.is_open() ) {
        while ( myfile >> workload_arr[i]) {
            i += 1;
        }
    } else {
        cout << "Failed to open the file." << endl;
    }
}

void std_stable_sort(int* start, int* end) {
    stable_sort(start, end);
}

int main() {
    const int len = 1000000;
    int workload[len];

    read_workload("./bods/workloads/createdata_K100_L100.txt", workload);
    
    time_t start_time = clock();

    const int sort_type = 1;
    switch (sort_type) {
        case 1:
            std_stable_sort(workload, workload + len);
            break;
        default:
            cout << "Cannot find the sort type." << endl;
            return 0;
    }

    float proc_time = (float)(clock() - start_time);
    cout << "sort_type[" << sort_type << "] " << proc_time / CLOCKS_PER_SEC << " seconds" << endl;
}
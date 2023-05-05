// shuffle algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

int main (int argc, char* argv[]) {
    // Reference: https://cplusplus.com/reference/algorithm/shuffle/
    long long SIZE = atoll(argv[1]);
    // Reference: https://cplusplus.com/reference/algorithm/random_shuffle/
    std::vector<long long> myvector;
    for (long long i=1; i <= SIZE; ++i) myvector.push_back(i);
 
    int start = 1;
    std::iota(myvector.begin(), myvector.end(), start);

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (myvector.begin(), myvector.end(), std::default_random_engine(seed));


    for (std::vector<long long>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
        std::cout << *it << '\n';

    return 0;
}
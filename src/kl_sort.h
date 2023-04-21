#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

#include "bin_heap.h"

using namespace std;

// Reference: https://github.com/lin826/KL-sort

bool kl_sort(long long R[], long long OUT[], long long n, long long k, long long l) {
   // Create two binary heaps S, G
   MinHeap S = MinHeap(n), G = MinHeap(n);

   // Insert the first k+l+1 tuples (R[1],...,R[k+l+1]) into S
   for (long long i = 0; i < k+l && i < n; i++) {
      S.insertKey(R[i]);
   }
   long long i_write = 0;

   long long *TMP = new long long[n + 1];
   for (long long i_read = S.getHeapSize(); i_read < n; i_read++) {
      if (S.getHeapSize() <= 0) {
         return false;
      }

      long long last_write = S.extractMin();
      // Write last_write to TMP[i_write]
      TMP[i_write] = last_write;

      i_write += 1;
      if (R[i_read] >= last_write) {
         S.insertKey(R[i_read]);
      } else {
         G.insertKey(R[i_read]);
      }
   }

   // Append all tuples in S to TMP, in sorted order
   while (S.getHeapSize() > 0) {
      TMP[i_write] = S.extractMin();
      i_write += 1;
   }

   i_write = 0;
   for (long long i_read = 0; i_read < n - G.getHeapSize(); i_read++) {
      long long x = G.getMin();
      if (x < 0 || x > TMP[i_read]) {
         // Write TMP[i_read] to OUT[i_write]
         OUT[i_write] = TMP[i_read];
      } else {
         OUT[i_write] = x;
         G.extractMin();
         G.insertKey(TMP[i_read]);
      }
      i_write += 1;
   }

   // Append all tuples in G to OUT, in sorted order
   while (G.getHeapSize() > 0) {
      OUT[i_write] = G.extractMin();
      i_write += 1;
   }
   return true;
}

#include <cstdlib>
#include <iostream>

#include "bin_heap.h"

using namespace std;

// Reference: https://github.com/lin826/KL-sort

bool kl_sort(int R[], int OUT[], int n, int k, int l) {
   // Create two binary heaps S, G
   MinHeap S = MinHeap(n), G = MinHeap(n);

   // Insert the first k+l+1 tuples (R[1],...,R[k+l+1]) into S
   for (int i = 0; i < k+l && i < n; i++) {
      S.insertKey(R[i]);
   }
   int i_write = 0;

   int *TMP = new int[n + 1];
   for (int i_read = S.getHeapSize(); i_read < n; i_read++) {
      if (S.getHeapSize() <= 0) {
         return false;
      }

      int last_write = S.extractMin();
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
   for (int i_read = 0; i_read < n - G.getHeapSize(); i_read++) {
      int x = G.getMin();
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

int calculate_k(int R[], int n) {
   int k = 0;
   for (int i = 0; i<n; i++) {
      if (R[i] != i+1) {
         k++;
      }
   }
   return k;
}

int getMax(int a, int b) {
   return (a > b) ? a : b;
}

int distance(int a, int b) {
   return (a > b) ? (a - b) : (b - a);
}

int calculate_l(int R[], int n) {
   int l = 0;
   for (int i = 0; i<n; i++) {
      l = getMax(l, distance(R[i], i+1) + 1);
   }
   return l;
}

// Precise K with the max L
bool kl_search_first(int R[], int OUT[], int n) {
   int k = calculate_k(R, n/100) * 100; // use the first 1% elements to estimate
   int l = n/2 - k; // half of elements set into heap S
   return kl_sort(R, OUT, n, k, l);
}

// Precise K with the min L = 0, or precise L with the min K = 0
bool kl_search_second(int R[], int OUT[], int n) {
   int k = calculate_k(R, n/100) * 100; // use the first 1% elements to estimate
   int l = calculate_l(R, n);

   if (k > l) {
      return kl_sort(R, OUT, n, 0, l);
   }
   return kl_sort(R, OUT, n, k, 0);
}

pair<int, int> estimate_kl(int R[], int n, int limit) {
   pair<int, int> kl;
   kl.first = 0, kl.second = 0;

   for (int i = 0; i<n; i++) {
      int dist = distance(R[i], i+1) + 1;

      // In the correct position, no need to extend K or L.
      if (dist == 0) {
         continue;
      }

      // Over the limit and increase K directly.
      if (dist > limit) {
         kl.first++;
         continue;
      }

      int l = getMax(kl.second, dist);
      if ((kl.first + l) > (kl.first + 1)) {
         kl.first++;
      } else {
         kl.second = l;
      }
   }
   return kl;
}

// Estimation on between
bool kl_search_third(int R[], int OUT[], int n) {
   pair<int, int> kl = estimate_kl(R, n, n*7/10); // Only allow 70% distance to misplace

   return kl_sort(R, OUT, n, kl.first, kl.second);
}

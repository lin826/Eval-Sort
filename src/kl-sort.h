#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

// Reference: https://github.com/lin826/KL-sort
class BHeap {
    private:
        vector <long long> heap;
        long long l(long long parent);
        long long r(long long parent);
        long long par(long long child);
        void heapifyup(long long index);
        void heapifydown(long long index);
    public:
        BHeap() {}
        void Insert(long long element);
        void DeleteMin();
        long long ExtractMin();
        void showHeap();
        long long Size();
};

bool kl_sort(long long R[], long long OUT[], long long n, long long k, long long l) {
   // Create two binary heaps S, G
   BHeap S, G;

   // Insert the first k+l+1 tuples (R[1],...,R[k+l+1]) into S
   for (long long i = 0; i < k+l && i < n; i++) {
      S.Insert(R[i]);
   }
   long long i_write = 0;

   long long *TMP = new long long[n + 1];
   for (long long i_read = S.Size(); i_read < n; i_read++) {
      if (S.Size() <= 0) {
         return false;
      }

      long long last_write = S.ExtractMin();
      // Write last_write to TMP[i_write]
      TMP[i_write] = last_write;

      S.DeleteMin();
      i_write += 1;
      if (R[i_read] >= last_write) {
         S.Insert(R[i_read]);
      } else {
         G.Insert(R[i_read]);
      }
   }

   // Append all tuples in S to TMP, in sorted order
   while (S.Size() > 0) {
      TMP[i_write] = S.ExtractMin();
      S.DeleteMin();
      i_write += 1;
   }

   i_write = 0;
   for (long long i_read = 0; i_read < n - G.Size(); i_read++) {
      long long x = G.ExtractMin();
      if (x < 0 || x > TMP[i_read]) {
         // Write TMP[i_read] to OUT[i_write]
         OUT[i_write] = TMP[i_read];
      } else {
         OUT[i_write] = x;
         G.DeleteMin();
         G.Insert(TMP[i_read]);
      }
      i_write += 1;
   }

   // Append all tuples in G to OUT, in sorted order
   while (G.Size() > 0) {
      OUT[i_write] = G.ExtractMin();
      G.DeleteMin();
      i_write += 1;
   }
   return true;
}

long long BHeap::Size() {
   return heap.size();
}
void BHeap::Insert(long long ele) {
   heap.push_back(ele);
   heapifyup(heap.size() -1);
}
void BHeap::DeleteMin() {
   if (heap.size() == 0) {
      cout<<"Heap is Empty"<<endl;
      return;
   }
   heap[0] = heap.at(heap.size() - 1);
   heap.pop_back();
   heapifydown(0);
   // cout<<"Element Deleted"<<endl;
}
long long BHeap::ExtractMin() {
   if (heap.size() == 0) {
      return -1;
   }
   else
   return heap.front();
}
void BHeap::showHeap() {
   vector <long long>::iterator pos = heap.begin();
   cout<<"Heap --> ";
   while (pos != heap.end()) {
      cout<<*pos<<" ";
      pos++;
   }
   cout<<endl;
}
long long BHeap::l(long long parent) {
   long long l = 2 * parent + 1;
   if (l < heap.size())
      return l;
   else
      return -1;
}
long long BHeap::r(long long parent) {
   long long r = 2 * parent + 2;
   if (r < heap.size())
      return r;
   else
      return -1;
}
long long BHeap::par(long long child) {
   long long p = (child - 1)/2;
   if (child == 0)
      return -1;
   else
      return p;
}
void BHeap::heapifyup(long long in) {
   if (in >= 0 && par(in) >= 0 && heap[par(in)] > heap[in]) {
      long long temp = heap[in];
      heap[in] = heap[par(in)];
      heap[par(in)] = temp;
      heapifyup(par(in));
   }
}
void BHeap::heapifydown(long long in) {
   long long child = l(in);
   long long child1 = r(in);
   if (child >= 0 && child1 >= 0 && heap[child] > heap[child1]) {
      child = child1;
   }
   if (child > 0 && heap[in] > heap[child]) {
      long long t = heap[in];
      heap[in] = heap[child];
      heap[child] = t;
      heapifydown(child);
   }
}

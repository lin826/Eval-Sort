// A C++ program to demonstrate common Binary Heap Operations
#include<iostream>
#include<climits>
using namespace std;

// Prototype of a utility function to swap two integers
void swap(long long *x, long long *y);

// A class for Min Heap
class MinHeap
{
	long long *harr; // pointer to array of elements in heap
	long long capacity; // maximum possible size of min heap
	long long heap_size; // Current number of elements in min heap
public:
	// Constructor
	MinHeap(long long capacity);

	// to heapify a subtree with the root at given index
	void MinHeapify(long long i);

	long long parent(long long i) { return (i-1)/2; }

	// to get index of left child of node at index i
	long long left(long long i) { return (2*i + 1); }

	// to get index of right child of node at index i
	long long right(long long i) { return (2*i + 2); }

	// to get the size of the current heap
	long long getHeapSize() { return heap_size; };

	// to extract the root which is the minimum element
	long long extractMin();

	// Decreases key value of key at index i to new_val
	void decreaseKey(long long i, long long new_val);

	// Returns the minimum key (key at root) from min heap
	long long getMin() { return harr[0]; }

	// Deletes a key stored at index i
	void deleteKey(long long i);

	// Inserts a new key 'k'
	void insertKey(long long k);
};

// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(long long cap)
{
	heap_size = 0;
	capacity = cap;
	harr = new long long[cap];
}

// Inserts a new key 'k'
void MinHeap::insertKey(long long k)
{
	if (heap_size == capacity)
	{
		cout << "\nOverflow: Could not insertKey\n";
		return;
	}

	// First insert the new key at the end
	heap_size++;
	long long i = heap_size - 1;
	harr[i] = k;

	// Fix the min heap property if it is violated
	while (i != 0 && harr[parent(i)] > harr[i])
	{
	swap(&harr[i], &harr[parent(i)]);
	i = parent(i);
	}
}

// Decreases value of key at index 'i' to new_val. It is assumed that
// new_val is smaller than harr[i].
void MinHeap::decreaseKey(long long i, long long new_val)
{
	harr[i] = new_val;
	while (i != 0 && harr[parent(i)] > harr[i])
	{
	swap(&harr[i], &harr[parent(i)]);
	i = parent(i);
	}
}

// Method to remove minimum element (or root) from min heap
long long MinHeap::extractMin()
{
	if (heap_size <= 0)
		return LLONG_MAX;
	if (heap_size == 1)
	{
		heap_size--;
		return harr[0];
	}

	// Store the minimum value, and remove it from heap
	int root = harr[0];
	harr[0] = harr[heap_size-1];
	heap_size--;
	MinHeapify(0);

	return root;
}


// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(long long i)
{
	decreaseKey(i, LLONG_MAX);
	extractMin();
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(long long i)
{
	long long l = left(i);
	long long r = right(i);
	long long smallest = i;
	if (l < heap_size && harr[l] < harr[i])
		smallest = l;
	if (r < heap_size && harr[r] < harr[smallest])
		smallest = r;
	if (smallest != i)
	{
		swap(&harr[i], &harr[smallest]);
		MinHeapify(smallest);
	}
}

// A utility function to swap two elements
void swap(long long *x, long long *y)
{
	long long temp = *x;
	*x = *y;
	*y = temp;
}

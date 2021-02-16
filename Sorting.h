#ifndef SORTING_H
#define SORTING_H

# include <iostream>
# include <string>
# include <vector>
# include <cstdlib>

using namespace std;

inline int leftChild( int i )
{
	return 2*i+1;
}

//heap sort:
// for deleteMax
// a is the array, i is the position to percolate down from
// n is the logical size of the array
template <class Comparable>  
void percDown( vector<Comparable> & a, int i, int n )
{
	int child;
	Comparable tmp;

	for (tmp=a[i] ; leftChild(i) < n; i = child )
	{
		child = leftChild(i);
		if ( child != n-1 && a[ child  ] < a[ child+1 ] )
			child++;
		if ( a[child ] > tmp )
			a[i] = a[ child ];
		else
			break;
	} 
	a[ i ] = tmp;
}

template <class Comparable>
void heapsort(vector<Comparable> & a)
{
	// buildHeap
	for (int i = a.size()/2; i >=0; i--)
	{
		percDown(a, i, a.size());
	}

	// sort
	for (int j = ((a.size())-1); j >0; j--)
	{
		Swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}



//insertion sort:
template <class Comparable>
void insertionSort (vector <Comparable> & a)
{
	int j;
	// loop over the passes
	for (int p = 1;  p < a.size(); p++)
	{
		Comparable tmp = a[p];
		// loop over the elements
		for (j = p; j > 0 &&  tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}



//merge sort:
/**
* Mergesort algorithm (driver).
*/
template <class Comparable>
void mergeSort( vector<Comparable> & a )
{
	vector<Comparable> tmpArray( a.size( ) );

	mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

/* Internal method that merges two sorted halves of a subarray.
* a is an array of Comparable items.
* tmpArray is an array to place the merged result.
* leftPos is the left-most index of the subarray.
* rightPos is the index of the start of the second half.
* rightEnd is the right-most index of the subarray.
*/
template <class Comparable>
void merge(  vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd )
{
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;
	// Main loop
	while ( leftPos <= leftEnd && rightPos <= rightEnd )
		if ( a[ leftPos ] <= a[ rightPos ] )
			tmpArray[ tmpPos++ ] = a[ leftPos++ ];
		else
			tmpArray[ tmpPos++ ] = a[ rightPos++ ];

	while ( leftPos <= leftEnd )    // Copy rest of first half
		tmpArray[ tmpPos++ ] = a[ leftPos++ ];

	while ( rightPos <= rightEnd )  // Copy rest of right half
		tmpArray[ tmpPos++ ] = a[ rightPos++ ];

	// Copy tmpArray back
	for ( int i = 0; i < numElements; i++, rightEnd-- )
		a[ rightEnd ] = tmpArray[ rightEnd ];
}

/**
* Internal method that makes recursive calls.
* a is an array of Comparable items.
* tmpArray is an array to place the merged result.
* left is the left-most index of the subarray.
* right is the right-most index of the subarray.
*/
template <class Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right )
{
	if ( left < right )
	{
		int center = ( left + right ) / 2;
		mergeSort( a, tmpArray, left, center );
		mergeSort( a, tmpArray, center + 1, right );
		merge( a, tmpArray, left, center + 1, right );
	}
}




//quick sort - median:
/**
* Quicksort algorithm (driver).
*/
template <class Comparable>
void quicksortMedian( vector<Comparable> & a )
{
	quicksortMedian( a, 0, a.size( ) - 1 );
}

/**
* Standard swap
*/
template <class Comparable>
inline void Swap( Comparable & obj1, Comparable & obj2 )
{
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

/** Return median of left, center, and right.
*   Order these and hide the pivot.
*/
template <class Comparable>
const Comparable & median3( vector<Comparable> & a, int left, int right )
{
	int center = ( left + right ) / 2;
	if ( a[ center ] < a[ left ] )
		Swap( a[ left ], a[ center ] );
	if ( a[ right ] < a[ left ] )
		Swap( a[ left ], a[ right ] );
	if ( a[ right ] < a[ center ] )
		Swap( a[ center ], a[ right ] );

	// Place pivot at position right - 1
	Swap( a[ center ], a[ right - 1 ] );
	return a[ right - 1 ];
}

/**
* Internal quicksort method that makes recursive calls.
* Uses median-of-three partitioning and a cutoff of 10.
* a is an array of Comparable items.
* left is the left-most index of the subarray.
* right is the right-most index of the subarray.
*/
template <class Comparable>
void quicksortMedian( vector<Comparable> & a, int left, int right )
{
	if ( left + 10 <= right )
	{
		Comparable pivot = median3( a, left, right );

		// Begin partitioning
		int i = left, j = right - 1;
		for ( ; ; )
		{
			while ( a[ ++i ] < pivot ) { }

			while ( pivot < a[ --j ] ) { }

			if ( i < j )
				Swap( a[ i ], a[ j ] );
			else
				break;
		}
		Swap( a[ i ], a[ right - 1 ] );   // Restore pivot

		quicksortMedian( a, left, i - 1 );       // Sort small elements
		quicksortMedian( a, i + 1, right );    // Sort large elements
	}
	else  // Do an insertion sort on the subarray
		insertionSort( a, left, right );
}

/**
* Internal insertion sort routine for subarrays
* that is used by quicksort.
* a is an array of Comparable items.
* left is the left-most index of the subarray.
* right is the right-most index of the subarray.
*/
template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
	for ( int p = left + 1; p <= right; p++ )
	{
		Comparable tmp = a[ p ];
		int j;

		for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
			a[ j ] = a[ j - 1 ];
		a[ j ] = tmp;
	}
}



////quicksort - first:
template <class Comparable>
void quicksortFirst (vector<Comparable> & a )
{
	quicksortFirst(a, 0, a.size() -1);
}

template<class Comparable>
void quicksortFirst(vector<Comparable> & a, int left, int right)
{
	if (left < right)
	{
		int pivot = partitionFirst( a, left, right );

		quicksortFirst( a, left, pivot - 1);
		quicksortFirst(a, pivot + 1, right);
	}
}

template<class Comparable>
int partitionFirst(vector<Comparable> & a, int left, int right)
{
	Comparable pivotItem = a[left];
	int pivotIndex = left;

	for (int idx = left + 1; idx <= right; idx++)
	{
		if (a[idx] < pivotItem) // < operator overloaded for WordItem class
		{
			Swap(a[pivotIndex + 1], a[idx]);
			Swap(a[pivotIndex], a[pivotIndex + 1]);
			pivotIndex++;
		}
	}
	return pivotIndex;
}

//template<class Comparable>
//int partitionFirst( vector<Comparable> & a, int left, int right)
//{
//	Comparable x = a[left];
//	int i = left;
//	int j;
//	for (j = left + 1 ; j < right ; j++)
//	{
//		if (a[j] <= x)
//		{
//			i += 1;
//			Swap(a[i],a[j]);
//		}
//	}
//	Swap(a[i],a[left]);
//	return i;
//}


//quicksort - random:
template<class Comparable>
void quicksortRandom ( vector<Comparable> & a )
{
	quicksortRandom ( a, 0, a.size() - 1);
}

template<class Comparable>
void quicksortRandom ( vector<Comparable> & a, int left, int right )
{
	if (left < right)
	{
		int pivot = partitionRandom(a,left,right);

		quicksortRandom(a,left,pivot-1);
		quicksortRandom(a,pivot+1,right);
	}
}

template<class Comparable>
int partitionRandom(vector<Comparable> & a, int left, int right)
{
	srand(time(NULL));
	int random = left + rand() % (right - left);

	Swap(a[random], a[left]);
	//after swapping the random idx with the left-most index
	//it is the same partitioning as partitionFirst
	return partitionFirst(a,left,right);
}
# endif
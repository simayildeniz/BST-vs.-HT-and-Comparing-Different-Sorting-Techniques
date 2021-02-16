#ifndef HASHTABLE_H
#define HASHTABLE_H

# include <iostream>
# include <string>

using namespace std;

bool isPrime( int n )
{
	if ( n == 2 || n == 3 )
		return true;

	if ( n == 1 || n % 2 == 0 )
		return false;

	for ( int i = 3; i * i <= n; i += 2 )
		if ( n % i == 0 )
			return false;

	return true;
}

int nextPrime( int n )
{
	if ( n % 2 == 0 )
		n++;

	for ( ; ! isPrime( n ); n += 2 )
		;

	return n;
}

template <class HashedObj>
class HashTable
{
public:
	explicit HashTable( HashedObj & notFound, int size = 53 );

	HashTable( const HashTable & rhs )
		: iTEM_NOT_FOUND( rhs.iTEM_NOT_FOUND ), array( rhs.array ), currentSize( rhs.currentSize ) 
	{ }

	HashedObj & find( HashedObj & x ) ;

	void makeEmpty( );
	void insert( HashedObj & x );
	void remove( const HashedObj & x );

	const HashTable & operator=( const HashTable & rhs );


	enum EntryType { ACTIVE, EMPTY, DELETED };

	int getCurrentSize();
	int getArraySize();

private:
	struct HashEntry
	{
		HashedObj element; //WordItem element
		EntryType info; //enum ACTIVE, DELETED, EMPTY

		HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
			: element( e ), info( i )
		{ }

		//bool operator ==( const HashEntry & rhs)
		//{
		//	if ((this->info == rhs->info) && (this->element == rhs->element)) //worditem == worditem
		//	{
		//		return true;
		//	}
		//	return false;
		//}

		//bool operator !=(const HashEntry & rhs)
		//{
		//	return !(this == rhs);
		//}
	};

	vector<HashEntry> array;
	int currentSize;
	HashedObj iTEM_NOT_FOUND;

	bool isActive( int currentPos ) const;
	int findPos(  HashedObj & x ) const;
	void rehash( );
};


// taken from lecture slides named "8-Hashtables.ppt" and updated
/*
hashing function for WordItem i.e. HashedObj
*/

int Hash( WordItem & key, int tableSize ) 
{
	int hashVal = 0;

	//string KEY = key.access_keyWord();

	for( int i = 0; i < key.access_keyWord().length(); i++ )
		hashVal = 37 * hashVal + key.access_keyWord()[ i ];

	hashVal = hashVal % tableSize;
	if( hashVal < 0 )
		hashVal = hashVal + tableSize;

	return hashVal;
}

template <class HashedObj>
int HashTable<HashedObj>::getArraySize()
{
	return this->array.size();
}

template <class HashedObj>
int HashTable<HashedObj>::getCurrentSize()
{
	return this->currentSize;
}

/**
* Construct the hash table.
*/
template <class HashedObj>
HashTable<HashedObj>::HashTable( HashedObj & notFound, int size )
	: iTEM_NOT_FOUND( notFound ), array( nextPrime( size ) )
{
	makeEmpty( );
}

/**
* Method that performs quadratic probing resolution.
* Return the position where the search for x terminates.
*/
template <class HashedObj>
int HashTable<HashedObj>::findPos(  HashedObj & x ) const
{
	int collisionNum = 0;
	int currentPos = Hash( x, array.size( ) );

	while ( array[ currentPos ].info != EMPTY && 
		!(array[ currentPos ].element == x ))
	{
		currentPos += 2 * ++collisionNum - 1;  // add the difference
		if ( currentPos >= array.size( ) )             // perform the mod
			currentPos = currentPos - array.size( );               // if necessary
	}
	return currentPos;
}

/**
* Return true if currentPos exists and is active.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
	return array[ currentPos ].info == ACTIVE;
}

/**
* Remove item x from the hash table.
*  x has to be in the table
*/
template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		array[ currentPos ].info = DELETED;
}

/**
* Find item x in the hash table.
* Return the matching item, or ITEM_NOT_FOUND, if not found.
*/
template <class HashedObj>
HashedObj & HashTable<HashedObj>::find( HashedObj & x ) 
{
	int currentPos = findPos( x );
	if (isActive( currentPos )) 
		return array[ currentPos ].element; 

	return   iTEM_NOT_FOUND;
}

/**
* Insert item x into the hash table. If the item is
* already present, then do nothing.
*/
template <class HashedObj>
void HashTable<HashedObj>::insert( HashedObj & x )
{
	// Insert x as active
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		return;
	array[ currentPos ] = HashEntry( x, ACTIVE );

	// enlarge the hash table if necessary 
	//currentSize++;

	double loadFactor;////?? 0.25 - 0.9
	loadFactor = currentSize / array.size();
	//(currentSize >= array.size( ) / 2)

	if (++currentSize >= array.size( ) / 1.5)
		rehash( );
}

/**
* Expand the hash table.
*/
template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{
	//cout << "rehashed..." << endl;

	vector<HashEntry> oldArray = array;

	//cout << "previous table size: " << oldArray.size();

	// Create new double-sized, empty table
	array.resize( nextPrime( 2 * oldArray.size( ) ) );
	for ( int j = 0; j < array.size( ); j++ )
		array[ j ].info = EMPTY;

	//cout << ", new table size: " << array.size();

	// Copy table over
	currentSize = 0;
	for ( int i = 0; i < oldArray.size( ); i++ )
		if ( oldArray[ i ].info == ACTIVE )
			insert( oldArray[ i ].element );

	//cout << ", current unique word count " << currentSize; 
	//double loadFactor = double(currentSize) / double(array.size());
	//cout << ", current load factor: " << loadFactor << endl; 
}

/**
* Make the hash table logically empty.
*/
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{
	currentSize = 0;
	for( int i = 0; i < array.size( ); i++ )
		array[ i ].info = EMPTY;
}
#endif
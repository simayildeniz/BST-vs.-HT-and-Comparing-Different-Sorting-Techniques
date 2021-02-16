#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;


template <class Comparable>
class BinarySearchTree;


template <class Comparable>
class BinaryNode
{
	Comparable element;
	BinaryNode  *left;
	BinaryNode  *right;

	BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
		: element( theElement ), left( lt ), right( rt ) 
	{ }

	friend class BinarySearchTree<Comparable>;
};


template <class Comparable>
class BinarySearchTree
{
public:
	explicit BinarySearchTree( const Comparable & notFound );
	BinarySearchTree( const BinarySearchTree & rhs );
	~BinarySearchTree( );

	const Comparable & findMin( );
	const Comparable & findMax( );
	Comparable & find( Comparable & x );
	bool isEmpty( );
	void printTree( );

	void makeEmpty( );
	void insert( Comparable & x );
	void remove( Comparable & x );

	const BinarySearchTree & operator=( const BinarySearchTree & rhs );

private:

	BinaryNode<Comparable> *root;
	Comparable ITEM_NOT_FOUND;

	Comparable & elementAt( BinaryNode<Comparable> *t );

	void insert( Comparable & x, BinaryNode<Comparable> * & t );
	void remove( Comparable & x, BinaryNode<Comparable> * & t );
	BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t );
	BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t );
	BinaryNode<Comparable> * find( Comparable & x, BinaryNode<Comparable> *t );

	void makeEmpty( BinaryNode<Comparable> * & t );
	void printTree( BinaryNode<Comparable> *t );
	BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t );
};

/**
* Construct the tree.
*/
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound ) 
	:ITEM_NOT_FOUND( notFound ), root( NULL )
{}

/**
* Destructor for the tree.
*/
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( )
{
	makeEmpty( );
}

/**
* Make the tree logically empty.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( )
{
	makeEmpty( root );
}

/**
* Internal method to make subtree empty.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t )
{
	if( t != NULL )
	{
		makeEmpty( t->left );
		makeEmpty( t->right );
		delete t;
	}
	t = NULL;
}

/**
* Internal method to get element field in node t.
* Return the element field or ITEM_NOT_FOUND if t is NULL.
*/
template <class Comparable>
Comparable & BinarySearchTree<Comparable>::elementAt( BinaryNode<Comparable> *t )
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

/**
* Find item x in the tree.
* Return the matching item or ITEM_NOT_FOUND if not found.
*/
template <class Comparable>
Comparable & BinarySearchTree<Comparable>::find( Comparable & x )
{
	return elementAt( find( x, root ) );
}

/**
* Internal method to find an item in a subtree.
* x is item to search for.
* t is the node that roots the tree.
* Return node containing the matched item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::find( Comparable & x, BinaryNode<Comparable> * t )
{
	//this_thread::sleep_for(chrono::nanoseconds(x));
	if ( t == NULL )
		return NULL;
	else if( x < t->element )
		return find( x, t->left );
	else if( t->element < x )
		return find( x, t->right );
	else
		return t;    // Match
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin( )
{
	return elementAt( findMin( root ) );
}

/**
* Internal method to find the smallest item in a subtree t.
* Return node containing the smallest item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t ) 
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findMin( t->left );
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax( )
{
	return elementAt( findMax( root ) );
}

/**
* Internal method to find the largest item in a subtree t.
* Return node containing the largest item.
*/
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMax( BinaryNode<Comparable> *t ) 
{
	if( t != NULL )
		while( t->right != NULL )
			t = t->right;
	return t;
}

/**
* Insert x into the tree; duplicates are ignored.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert( Comparable & x )
{
	insert( x, root );
}

/**
* Internal method to insert into a subtree.
* x is the item to insert.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::insert( Comparable & x, BinaryNode<Comparable> * & t ) 
{
	if( t == NULL ) //  create a new node at the right place
		t = new BinaryNode<Comparable>( x, NULL, NULL );
	else if( x < t->element )
		insert( x, t->left );  // insert at the left or 
	else if( t->element < x )
		insert( x, t->right );  // right subtree
	else
		;  // Duplicate; do nothing
}

/**
* Remove x from the tree. Nothing is done if x is not found.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove( Comparable & x )
{
	remove( x, root );
}

/**
* Internal method to remove from a subtree.
* x is the item to remove.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::remove( Comparable & x, BinaryNode<Comparable> * & t ) 
{
	if( t == NULL )
		return;   // Item not found; do nothing
	if( x < t->element )
		remove( x, t->left );
	else if( t->element < x )
		remove( x, t->right );
	else if( t->left != NULL && t->right != NULL ) // Two children
	{
		t->element = findMin( t->right )->element;
		remove( t->element, t->right );
	}
	else // one or no children
	{
		BinaryNode<Comparable> *oldNode = t;
		t = ( t->left != NULL ) ? t->left : t->right;
		delete oldNode;
	}
}

/**
* Print the tree contents in sorted order.
*/
template <class Comparable>
void BinarySearchTree<Comparable>::printTree( )
{
	if( isEmpty( ) )
		cout << "Empty tree" << endl;
	else
		printTree( root );
}

/**
* Copy constructor.
*/
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree( const BinarySearchTree<Comparable> & rhs ) 
	:root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{ 
	*this = rhs;
}
#endif
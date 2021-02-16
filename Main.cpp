# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <thread>
# include <fstream>
# include <sstream>
# include <chrono>
# include <map>
# include "WordItem.h"
# include "BinarySearchTree.h"
# include "HashTable.h"
# include "Sorting.h"

using namespace std;

WordItem item_NOT_found;

BinarySearchTree<WordItem> searchTree(item_NOT_found); //initial root is item not found
HashTable<WordItem> hashMap(item_NOT_found,53); // initially arraysize is 53 which is a prime number
vector<WordItem> myVector;
vector<WordItem> mySortedVector;

vector<WordItem> quickSortMedian;
vector<WordItem> quickSortRandom;
vector<WordItem> quickSortFirst;

vector<WordItem> heapSortVector;
vector<WordItem> insertionSortVector;
vector<WordItem> mergeSortVector;

float BinarySearch_Time;
float BinarySearchTree_Time;
float HashTable_Time;

float quickSortMedian_Time;
float quickSortFirst_Time;
float quickSortRandom_Time;
float mergeSort_Time;
float insertionSort_Time;
float heapSort_Time;

string makeStringValid( string & word)
{
	//convert all letters of the word into lowercase
	for_each(word.begin(), word.end(), [](char & c) 
	{  c = ::tolower(c); });

	// for punctuation, digits and non letter, chars
	for (int i = 0; i < word.length(); i++) 
	{
		if( !((word[i] <= 'z' && word[i] >= 'a') || (word[i] <= 'Z' && word[i] >= 'A')) )  
		{
			word[i] = ' ' ;
		}
	}
	return word;
}

//search in the global variable vector<WordItem> myVector
WordItem & findInVector ( WordItem & item )
{
	string itemsKey = item.access_keyWord();

	for (int i = 0; i < myVector.size(); i++)
	{
		string iKey = myVector[i].access_keyWord();
		if (itemsKey == iKey)
		{
			return myVector[i];
		}
	}

	return item_NOT_found;
}

//used in sorted vectors
//compare item with the one in the middle
//if there is a match return the mid index
//else if items keyWord is greater than mid index, look at the right sub-vector
//else look at the left sub-vector
WordItem & findInVector_BinarySearch ( WordItem & item, vector<WordItem> & arr )
{
	string itemKey = item.access_keyWord();

	int left = 0;
	int right = arr.size() - 1;

	while (left <= right)
	{
		int middle = left + (right - left) / 2;
		string middleKey = arr[middle].access_keyWord();

		//if item is in the middle
		if (middleKey == itemKey)
		{
			return arr[middle];
		}

		//if itemKey is smaller
		if (middleKey < itemKey)
		{
			left = middle + 1;
		}
		//if itemKey is bigger
		else
		{
			right = middle - 1;
		}
	}
	//if not found:
	return item_NOT_found;
}

//reads the file word by word, lowercases and removes punction in every word
void readFile_and_UpdateTree_AND_UpdateHashTable_AND_fillVector(string fileName, BinarySearchTree<WordItem> & searchTree, HashTable<WordItem> & hashMap, vector<WordItem> & myVector)
{
	fstream inFile;
	string word1;

	inFile.open(fileName.c_str());

	while (inFile >> word1)
	{
		word1 = makeStringValid(word1);
		istringstream iss(word1);
		string tempWord;
		while ( iss >> tempWord)
		{
			//cout << tempWord << endl;
			DocumentItem newDocument;
			newDocument.documentName = fileName;
			newDocument.count = 1;

			WordItem newItem = WordItem(tempWord, newDocument);

			////BST INSERT:
			////Also myVector insert
			//I will use the same flow to push_back newItem to myVector or
			//find the already existing index and update its listOfDocuments

			if (searchTree.find(newItem) == item_NOT_found)
			{
				searchTree.insert(newItem);

				// as I insert to tree, I will also insert to myVector
				myVector.push_back(newItem);
			}
			else //node with the given keyword exists in bst:
			{
				WordItem * alreadyExist = &searchTree.find(newItem);
				//This means item already exists, but we are not sure about which file in the listOfDocuments it exists in
				//It may be the current fileName that this function is called
				//Or it may be the previous file I read if there are multiple files
				WordItem * alreadyExistInVector = &findInVector(newItem);

				bool isInCurrentFile = false;
				//if it exists in the current file I am reading:
				vector<DocumentItem> * tempListOfDoc = alreadyExist->access_listOfDocuments();
				vector<DocumentItem> * tempListOfDocFORvector = alreadyExistInVector->access_listOfDocuments();
				for (int i = 0; i < (*tempListOfDoc).size(); i++)
				{
					if ((*tempListOfDoc)[i].documentName == fileName)
					{
						(*tempListOfDoc)[i].count += 1;
						(*tempListOfDocFORvector)[i].count += 1;

						isInCurrentFile = true;
					}
				}

				//if it exist in another file:
				//this file I am reading now does not exist in the listOfDocuments
				//I should push back the current fileName to its listOfDocuments and count in that file will be 1
				if (isInCurrentFile == false)
				{
					DocumentItem tempDocItem;
					tempDocItem.documentName = fileName;
					tempDocItem.count = 1;

					alreadyExist->addTo_listOfDocuments(tempDocItem);
					alreadyExistInVector->addTo_listOfDocuments(tempDocItem);
				}

			}

			////HT INSERT:
			if (hashMap.find(newItem) == item_NOT_found)
			{
				hashMap.insert(newItem);
			}
			else
			{
				WordItem * alreadyExist = &hashMap.find(newItem);
				//This means item already exists, but we are not sure about which file in the listOfDocuments it exists in
				//It may be the current fileName that this function is called
				//Or it may be the previous file


				bool isInCurrentFile = false;
				//if it exists in the current file I am reading:
				vector<DocumentItem> * tempListOfDoc = alreadyExist->access_listOfDocuments();
				for (int i = 0; i < (*tempListOfDoc).size(); i++)
				{
					if ((*tempListOfDoc)[i].documentName == fileName)
					{
						(*tempListOfDoc)[i].count += 1;
						isInCurrentFile = true;
					}
				}

				//if it exist in another file:
				//I should push back the current fileName to its listOfDocuments and count in that file will be 1
				if (isInCurrentFile == false)
				{
					DocumentItem tempDocItem;
					tempDocItem.documentName = fileName;
					tempDocItem.count = 1;

					alreadyExist->addTo_listOfDocuments(tempDocItem);
				}

			}

		}
	}
	inFile.close();
}

void sortVectors()
{
	//deep copy of all elements in myVector to sorting vectors:
	quickSortMedian = myVector;
	quickSortRandom = myVector;
	quickSortFirst = myVector;
	heapSortVector = myVector;
	insertionSortVector = myVector;
	mergeSortVector = myVector;
	mySortedVector = myVector;

	//sort every vector:
	quicksortMedian(mySortedVector);

	clock_t begin_time = clock();
	//auto start = std::chrono::high_resolution_clock::now();
	heapsort(heapSortVector);
	//auto hst = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//heapSort_Time = double(hst.count());
	heapSort_Time = float(clock() - begin_time);

	begin_time = clock();
	//start = std::chrono::high_resolution_clock::now();
	insertionSort(insertionSortVector);
	//auto ist = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//insertionSort_Time = double(ist.count()); 
	insertionSort_Time = float(clock() - begin_time);

	begin_time = clock();
	//start = std::chrono::high_resolution_clock::now();
	mergeSort(mergeSortVector);
	//auto mst = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//mergeSort_Time = double(mst.count());
	mergeSort_Time = float(clock() - begin_time);

	begin_time = clock();
	//start = std::chrono::high_resolution_clock::now();
	quicksortMedian(quickSortMedian);
	//auto qsMt = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//quickSortMedian_Time = double(qsMt.count());
	quickSortMedian_Time = float(clock() - begin_time);

	begin_time = clock();
	//start = std::chrono::high_resolution_clock::now();
	quicksortFirst(quickSortFirst);
	//auto qsFt = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//quickSortFirst_Time = double(qsFt.count());
	quickSortFirst_Time = float(clock() - begin_time);

	begin_time = clock();
	//start = std::chrono::high_resolution_clock::now();
	quicksortRandom(quickSortRandom);
	//auto qsRt = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	//quickSortRandom_Time = double(qsRt.count()); 
	quickSortRandom_Time = float(clock() - begin_time);
}

void printList( vector<string> & toDisplay)
{
	int doesExist = 0;
	for (int i = 0; i < toDisplay.size(); i++)
	{
		if (toDisplay[i] == "No document contains the given query")
		{
			doesExist += 1;
		}
	}
	if (doesExist == toDisplay.size())
	{
		cout << "No document contains the given query" << endl;
		toDisplay.resize(0);
		return;
	}
	else
	{
		for (int x = 0; x < toDisplay.size(); x++)
		{
			if (toDisplay[x] != "No document contains the given query")
			{
				cout << toDisplay[x] << endl;
			}	
		}
		toDisplay.resize(0);
	}
}

void processQuery( BinarySearchTree<WordItem> & searchTree, HashTable<WordItem> & hashMap, string & query, int & numOfFiles, const vector<string> fileNames)
{
	cin.ignore();
	getline(cin,query);
	cout << endl;

	// in order to keep every word inside query, use a vector:
	vector<string> lookFor;

	//make sure query has valid chars, else correct it:
	string TempElement;
	istringstream issTemp(query);


	while (issTemp >> TempElement)
	{
		TempElement = makeStringValid(TempElement);

		string element = TempElement;
		istringstream iss(element);
		while (iss >> element)
		{
			lookFor.push_back(element);
		}	
	}

	////////////////////////////////////////////////////////SEARCHING PART IN TREE:

	int k = 110;

	auto start = std::chrono::high_resolution_clock::now();
	//clock_t begin_time = clock();

	vector<string> displayBST;
	vector<WordItem> resultsVectorBST;
	for (int i = 0; i < k; i++)
	{
		resultsVectorBST.resize(0);
		for (int i = 0; i < lookFor.size(); i++)
		{
			WordItem temp;
			temp.changekeyWord_as(lookFor[i]);
			temp = searchTree.find(temp);

			resultsVectorBST.push_back(temp);
		}
	}

	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	BinarySearchTree_Time = (float(BSTTime.count()) / k);

	//BinarySearchTree_Time = ( float(clock() - begin_time) / k );

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		bool fileExist = true;
		for (int j = 0; j < resultsVectorBST.size(); j++)
		{
			bool wordExist = false;
			vector<DocumentItem> WItempDIlist = resultsVectorBST[j].getCopyof_listOfDocuments();
			for (int k = 0; k < WItempDIlist.size(); k++)
			{
				if (WItempDIlist[k].documentName == fileNames[ii])
				{
					wordExist = true;
					break;
				}
			}
			if (wordExist == false)
			{
				fileExist = false;
				break;
			}

		}
		if (fileExist == true)
		{
			//print
			for (int s = 0; s < resultsVectorBST.size(); s++)
			{
				string toDisplay = "in Document " + fileNames[ii] + ", " + resultsVectorBST[s].access_keyWord() + " found ";
				vector <DocumentItem> myList = resultsVectorBST[s].getCopyof_listOfDocuments();
				for (int ss = 0; ss < myList.size() ; ss++)
				{
					if (myList[ss].documentName == fileNames[ii])
					{
						toDisplay += to_string(myList[ss].count) + " times, ";
					}
				}

				toDisplay[ toDisplay.size() - 2 ] = '.';

				displayBST.push_back(toDisplay);
				//cout <<toDisplay << endl;
			}

		}
		else
		{
			//not found
			string toDisplay = "No document contains the given query";
			//cout <<  "No document contains the given query" << endl;
			displayBST.push_back(toDisplay);
		}


	}
	printList(displayBST);

	//cout << "\nBinary Search Tree Time: " << endBST / k << "\n";

	/////////////////////////////////////////////////////SEARCHING PART IN HASHMAP:

	start = std::chrono::high_resolution_clock::now();
	//begin_time = clock();

	vector<string> displayHT;
	vector<WordItem> resultsVectorHT;
	for (int i = 0; i < k; i++)
	{
		resultsVectorHT.resize(0);
		for (int i = 0; i < lookFor.size(); i++)
		{
			WordItem temp;
			temp.changekeyWord_as(lookFor[i]);
			temp = hashMap.find(temp);

			resultsVectorHT.push_back(temp);
		}
	}

	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	HashTable_Time = (double(HTTime.count()) / (110.0));
	//HashTable_Time = ( float(clock() - begin_time) / k );

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		bool fileExist = true;
		for (int j = 0; j < resultsVectorHT.size(); j++)
		{
			bool wordExist = false;
			vector<DocumentItem> WItempDIlist = resultsVectorHT[j].getCopyof_listOfDocuments();
			for (int k = 0; k < WItempDIlist.size(); k++)
			{
				if (WItempDIlist[k].documentName == fileNames[ii])
				{
					wordExist = true;
					break;
				}
			}
			if (wordExist == false)
			{
				fileExist = false;
				break;
			}

		}
		if (fileExist == true)
		{
			//print
			for (int s = 0; s < resultsVectorHT.size(); s++)
			{
				string toDisplay = "in Document " + fileNames[ii] + ", " + resultsVectorHT[s].access_keyWord() + " found ";
				vector <DocumentItem> myList = resultsVectorHT[s].getCopyof_listOfDocuments();
				for (int ss = 0; ss < myList.size() ; ss++)
				{
					if (myList[ss].documentName == fileNames[ii])
					{
						toDisplay += to_string(myList[ss].count) + " times, ";
					}
				}

				toDisplay[ toDisplay.size() - 2 ] = '.';

				displayHT.push_back(toDisplay);
				//cout <<toDisplay << endl;
			}

		}
		else
		{
			//not found
			string toDisplay = "No document contains the given query";
			//cout <<  "No document contains the given query" << endl;
			displayHT.push_back(toDisplay);
		}
	}

	printList(displayHT);

	//cout << "\nHash Table Time: " << endHT / k << "\n";


	//////////////////////////////////////SEARCHING PART IN VECTORS -BINARY SEARCH-:

	 start = std::chrono::high_resolution_clock::now();
	 //begin_time = clock();

	vector<string>display_Vector;
	vector<WordItem> resultsVector_Vector;
	for (int i = 0; i < k; i++)
	{
		resultsVector_Vector.resize(0);
		for (int i = 0; i < lookFor.size(); i++)
		{
			WordItem temp;
			temp.changekeyWord_as(lookFor[i]);
			temp = findInVector_BinarySearch(temp,mySortedVector); 

			resultsVector_Vector.push_back(temp);
		}
	}

	auto VectorTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
	BinarySearch_Time = ( double(VectorTime.count()) / (110.0) );

	//BinarySearch_Time = ( float(clock() - begin_time) / k );

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		bool fileExist = true;
		for (int j = 0; j < resultsVector_Vector.size(); j++)
		{
			bool wordExist = false;
			vector<DocumentItem> WItempDIlist = resultsVector_Vector[j].getCopyof_listOfDocuments();
			for (int k = 0; k < WItempDIlist.size(); k++)
			{
				if (WItempDIlist[k].documentName == fileNames[ii])
				{
					wordExist = true;
					break;
				}
			}
			if (wordExist == false)
			{
				fileExist = false;
				break;
			}

		}
		if (fileExist == true)
		{
			//print
			for (int s = 0; s < resultsVector_Vector.size(); s++)
			{
				string toDisplay = "in Document " + fileNames[ii] + ", " + resultsVector_Vector[s].access_keyWord() + " found ";
				vector <DocumentItem> myList = resultsVector_Vector[s].getCopyof_listOfDocuments();
				for (int ss = 0; ss < myList.size() ; ss++)
				{
					if (myList[ss].documentName == fileNames[ii])
					{
						toDisplay += to_string(myList[ss].count) + " times, ";
					}
				}

				toDisplay[ toDisplay.size() - 2 ] = '.';

				display_Vector.push_back(toDisplay);
				//cout <<toDisplay << endl;
			}

		}
		else
		{
			//not found
			string toDisplay = "No document contains the given query";
			//cout <<  "No document contains the given query" << endl;
			display_Vector.push_back(toDisplay);
		}
	}

	printList(display_Vector);

	//cout << "\nBinary Search Time: " << endBS / k << "\n";

}

void showTimes()
{
	cout << "Binary Search Tree Time: " << BinarySearchTree_Time << endl;
	cout << "Hash Table Time: " << HashTable_Time << endl;
	cout << "Binary Search Time: " << BinarySearch_Time << endl;

	cout << endl;

	cout << "Quick Sort(median) Time: " << quickSortMedian_Time << endl;
	cout << "Quick Sort(random) Time: " << quickSortRandom_Time << endl;
	cout << "Quick Sort(first) Time: " << quickSortFirst_Time << endl;
	cout << "Merge Sort Time: " << mergeSort_Time << endl;
	cout << "Heap Sort  Time: " << heapSort_Time << endl;
	cout << "Insertion Sort Time: " << insertionSort_Time << endl;
}

void calculateSpeedUps ()
{
	cout << "Speed Up BST/HST: " << BinarySearchTree_Time / HashTable_Time << endl;  //???? bu dogru mu
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << mergeSort_Time / quickSortMedian_Time << endl;
	cout << "Speed Up Heap Sort/Quick Sort(Median): "<< heapSort_Time / quickSortMedian_Time << endl;
	cout << "Speed Up Insertion/Quick Sort(Median): " << insertionSort_Time / quickSortMedian_Time << endl;
	
	cout << endl;

	cout << "Speed Up Binary Search Tree Time/ Binary Search: " << BinarySearchTree_Time / BinarySearch_Time << endl;
	cout << "Speed Up Hash Table Time / Binary Search: " << HashTable_Time / BinarySearch_Time << endl;
}

int main()
{
	int numOfFiles;
	cout << "Enter number of input files: ";
	cin >> numOfFiles;

	vector<string> fileNames;

	for (int i = 0; i < numOfFiles; i++) //call this number of files times and use one tree
	{
		string fName;
		cout << "Enter " << to_string(i+1) << ". file name: ";
		cin >> fName;
		fileNames.push_back(fName);
	}
	cout << endl;

	for (int i = 0; i < fileNames.size(); i++)
	{
		readFile_and_UpdateTree_AND_UpdateHashTable_AND_fillVector(fileNames[i], searchTree, hashMap, myVector);
	}//after this function, I will have been done with the tree construction and hashmap filling

	cout << "After preprocessing, the unique word count is " << hashMap.getCurrentSize() << ". Current load ratio is " << double(hashMap.getCurrentSize()) / double(hashMap.getArraySize()) << endl;

	//I should sort the vectors:
	sortVectors();

	// Query Part
	string query;
	cout << endl << "Enter queried words in one line: ";

	processQuery(searchTree, hashMap, query, numOfFiles, fileNames);

	cout << endl;
	showTimes();

	cout << endl;
	calculateSpeedUps();

	return 0;
}
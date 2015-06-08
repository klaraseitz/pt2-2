#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>

// function template for printing contents of containers to std::cout
template<class T>
void printContainer(T& container)
{
	std::cout << "{";
	for (auto element : container)
		std::cout << element << " ";
	std::cout << "}" << std::endl;
}

// Merge the given lists [leftIt..midIt) and [midIt..rightIt)
template<class T>
void merge(T leftIt, T midIt, T rightIt)
{
	assert(leftIt <= midIt && midIt <= rightIt);
	T startLeftIt  = leftIt;
	T endLeftIt = midIt-1;
	T startRightIt = midIt;
	T endRightIt = rightIt;

	int sizeOfMergedValues = rightIt-leftIt; 
	std::vector
	<typename T::value_type> mergedValues(sizeOfMergedValues+1); // Initialize with size rightIt - leftIt

	int countIndex = 0;


	while(std::distance(leftIt, endLeftIt)>=0 && std::distance(startRightIt, rightIt)>=0){ // both lists not empty
		if(*leftIt < *startRightIt){
			mergedValues[countIndex] = *leftIt;
			leftIt++;
			countIndex++;
		}else{
			mergedValues[countIndex] = *startRightIt;
			startRightIt++;
			countIndex++;
		}
	}
  while(std::distance(leftIt, endLeftIt)>=0){ // Left list is not empty
  	mergedValues[countIndex] = *leftIt;
  	leftIt++;
  	countIndex++;
  }
  while(std::distance(startRightIt, rightIt)>=0){	// Right list is not empty
  	mergedValues[countIndex] = *startRightIt;
  	startRightIt++;
  	countIndex++;
  }

  // replace objects in vector with sorted elements
  countIndex = 0;
  for (startLeftIt; startLeftIt <= endRightIt; ++startLeftIt)
  {
  	*startLeftIt = mergedValues[countIndex];
  	countIndex++;
  }
}

// Sort the given container using merge sort.
template<class T>
void mergeSort(T leftIt, T rightIt)
{
	
	if((std::distance(leftIt,rightIt))>1)
	{
		assert(leftIt < rightIt);
		T midIt = leftIt;
		std::advance(midIt, std::distance(leftIt, rightIt)/2);
		mergeSort(leftIt, midIt-1);
		mergeSort(midIt, rightIt);
		merge(leftIt, midIt, rightIt);
	}else if(std::distance(leftIt, rightIt)==1){
		mergeSort(leftIt, leftIt);
		mergeSort(rightIt, rightIt);
		merge(leftIt, rightIt, rightIt);
	}

}

int main(int argc, char** argv)
{
	// define sample data
	std::vector<int> sampleDataInt = { 10, 1, 12, 33, 24, 5, 6, -7, -2, 19 };
	std::vector<std::string> sampleDataString = { "Die", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung" };

	// test for integer vector
	printContainer(sampleDataInt);
	mergeSort(sampleDataInt.begin(), sampleDataInt.end()-1);
	printContainer(sampleDataInt);


	// test for string vector
	printContainer(sampleDataString);
	mergeSort(sampleDataString.begin(), sampleDataString.end()-1);
	printContainer(sampleDataString);

	return 0;
}

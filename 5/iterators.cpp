#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <map>

// function template for printing contents of containers to std::cout
template<class T>
void printContainer(T& container)
{
    std::cout << "{"; 
    for(auto element : container)
        std::cout << element << " ";
    std::cout << "}" << std::endl; 
}


// ToDo 5.1a - Merge front-back pairings of elements of inContainer into outContainer. Do *not* use the []-operator.
template<class T>
void front_back_pairing(T& inContainer, T& outContainer)
{
	//T& containerCopy = inContainer;
	bool first = 1; // if 1 then take first element, else take last element
	auto begin = inContainer.begin();
	auto end = inContainer.end();
	end--;
	while(begin != end){
		if(first){	// first element will be moved into new Container
			outContainer.push_back(*begin);
			begin++;
			first = 0;
		}else{		// last element will be moved into new Container
			outContainer.push_back(*end);
			end--;
			first = 1;
		}
	}
	outContainer.push_back(*begin);

}

// ToDo 5.1b - Remove all duplicates from the given container. Do *not* use the []-operator.
template<class T>
void remove_duplicates(T& container)
{
	auto containerIt = container.begin();
	T&& newContainer { *containerIt };	// first element will always be in resulting container
	containerIt++;

	auto duplicateTestItStart = newContainer.begin();
	auto duplicateTestItEnd = newContainer.end();

	bool first = 1;
	for(containerIt; containerIt < container.end(); containerIt++){

		for(duplicateTestItStart; duplicateTestItStart < duplicateTestItEnd; duplicateTestItStart++){
			if(*duplicateTestItStart == *containerIt){ // element has already been there
				first = 0;
				break;
			}
		}
		if(first){
			newContainer.push_back(*containerIt);
			duplicateTestItStart = newContainer.begin();
			duplicateTestItEnd = newContainer.end();
		}
		first = 1;
		
	}

	container = newContainer;
}

void calculate_differences(int& c1, int& c2, int& c3){
	// c1 -> c1 − c2 
	// c2 -> c2 
	// c3 -> c3 − c2
	c1 -= c2;
	c3 -= c2;
	
}

// ToDo 5.1c - Expand the given container by inserting the numerical differences of each element to its neighbors. Do *not* use the []-operator.
template<class T>
void insert_differences(T& container)
{
	auto itBegin = container.begin();
	auto itEnd = container.end();
	int c1 = *(itEnd-1);
	int c2 = *itBegin;
	int c3 = *itBegin+1;
	calculate_differences(c1, c2, c3);
	T&& newContainer {c1, c2, c3};
	itBegin++;
	for(itBegin; itBegin < itEnd; itBegin++){ // start with second element	
		if((itBegin+1) == itEnd){
			c1 = *(itBegin-1);
			c2 = *itBegin;
			c3 = *container.begin();
			calculate_differences(c1, c2, c3);
			newContainer.push_back(c1);
			newContainer.push_back(c2);
			newContainer.push_back(c3);
		}else{
			c1 = *(itBegin-1);
			c2 = *itBegin;
			c3 = *(itBegin+1);
			calculate_differences(c1, c2, c3);
			newContainer.push_back(c1);
			newContainer.push_back(c2);
			newContainer.push_back(c3);
		}
	}
	container = newContainer;

}

void testFrontBackPairingFunctionality()
{
    // define sample data
    std::vector<int> sampleDataInt = { 10,11,12,13,14,15,16,17,18,19 };
    std::list<std::string> sampleDataString = { "Die", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung" };
    
    // test for integer vector
    std::vector<int> helpIntVector;
    front_back_pairing(sampleDataInt, helpIntVector);
    printContainer(sampleDataInt);
    printContainer(helpIntVector);
    
    // test for string list
    std::list<std::string> helpStringList;
    front_back_pairing(sampleDataString, helpStringList);
    printContainer(sampleDataString);
    printContainer(helpStringList);
}

void testRemoveDuplicateFunctionality()
{
    // define sample data
    std::vector<int> sampleDataInt = { 10,11,10,13,14,15,15,15,18,19,12,11,17 };
    std::vector<std::string> sampleDataString = { "Die", "___", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung", "Die", "ist", "blabla" };
    
    // test for integer vector
    printContainer(sampleDataInt);
    //remove_duplicates(sampleDataInt, std::less<int>() );
    remove_duplicates(sampleDataInt);
    printContainer(sampleDataInt);
    
    // test for string vector
    printContainer(sampleDataString);
    remove_duplicates(sampleDataString);
    printContainer(sampleDataString);
}

void testAddDifferenceFunctionality()
{
    // define sample data
    std::vector<int> sampleDataInt = { 10,11,14,16,1,18 };
    
    // test for integer vector
    printContainer(sampleDataInt);
    //add_differences(sampleDataInt); probably meant the following:
    insert_differences(sampleDataInt);
    printContainer(sampleDataInt);
}

int main(int argc, char** argv)
{
    testFrontBackPairingFunctionality();
    testRemoveDuplicateFunctionality();
    testAddDifferenceFunctionality();

    return 0;
}

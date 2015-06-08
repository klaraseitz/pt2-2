#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

struct Route
{
	int airlineId;
	int sourceId;
	int destinationId;
};

bool operator<(const Route& r1, const Route& r2) {
	return r1.destinationId < r2.destinationId;
}

void importRoutesData(char* path, std::vector<Route>& routes)
{
	std::cout << "Importing routes data.." << std::endl;
	std::ifstream file(path);
	std::string field, line;
	
	while (std::getline(file, line))
	{
		std::istringstream linestream;
		linestream.str(line);
		int fieldNum = 0;
		Route route;
		route.airlineId = route.sourceId = route.destinationId = -1;

		while (std::getline(linestream, field, ';'))
		{
			try
			{
				switch (fieldNum)
				{
					case 1: // airline id
						route.airlineId = std::stoi(field);
						break;
					case 3: // source id
						route.sourceId = std::stoi(field);
						break;
					case 5: // dest id
						route.destinationId = std::stoi(field);
						break;
					default:
						break;
				}
			}
			catch (const std::invalid_argument&)
			{
				//std::cout << "Couldn't convert field " << currentLineNum << " correctly (invalid argument)!" << std::endl;
				//std::cout << field << std::endl;
			}
			catch (const std::out_of_range&)
			{
				//std::cout << "Couldn't convert field " << currentLineNum << " correctly (out of range)!" << std::endl;
				//std::cout << field << std::endl;
			}
			fieldNum++;
		}
		if (route.airlineId > -1 && route.sourceId > -1 && route.destinationId > -1)
			routes.push_back(route);
	}
}

// Return the number of routes for the given destination id based on a linear search. Count the number of lookups.
int linearSearch(int destID, std::vector<Route>& routes, long long& numLookups)
{
	int numRoutes = 0;

	for(const Route &it: routes)
	{
		numLookups++;
		if(it.destinationId == destID){
			numRoutes++;
		}
	}
	return numRoutes;
}

// Evaluate the linearSearch function by calling it for every possible destination id (1..9541). 
// Return the number of lookups and the processing time as a pair of long longs.
// Use std::chrono for time measurement.
std::pair<long long, long long> evaluateLinearSearch(std::vector<Route>& routes)
{
	long long numLookups = 0;
	long long duration = 0;

	auto start = std::chrono::system_clock::now();

	for (int i = 1; i < 9542; i++)
	{
		linearSearch(i, routes, numLookups); 
	}
	
	auto end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	duration = diff.count();

	return std::make_pair(numLookups, duration);
}

// Return the number of routes for the given destination id based on a binary search. Count the number of lookups.
// The vector should have been sorted before calling this function.
int binarySearch(int destID, std::vector<Route>& routes, long long& numLookups)
{
	long long numRoutes = 0;
    long long mitte;

    std::vector<Route>::iterator leftIt = routes.begin();
    std::vector<Route>::iterator middleIt; // = routes.begin();
    std::vector<Route>::iterator rightIt = routes.end()-1;
    bool nothingFound = 1;

    // Solange noch kein entsprechendes destID gefunden wurde liste halbieren, in passender Hälfte weitersuchen:
    while (leftIt <= rightIt && nothingFound)   
    {
    	numLookups++;
    	mitte = std::distance(leftIt, rightIt) / 2; // Bereiche halbieren

    	middleIt = leftIt;	// mitte auf Anfang setzen, damit advance geht
    	std::advance(middleIt, mitte);
 
        if (middleIt->destinationId == destID)  // wenn entsprechendes element gefunden wurde, dann ...
            {
            	numRoutes++;
            	nothingFound = 0;	// ... while-Schleife abbrechen lassen
            }else{
            	if (middleIt->destinationId > destID) { // im linken Abschnitt weitersuchen
            		rightIt = middleIt - 1;      
              	}else{ 
                	leftIt = middleIt + 1;      //im rechten Abschnitt weitersuchen
                }
        	}
    }

    if(nothingFound == false){	// es wurde ein Element gefunden...
    	for(std::vector<Route>::iterator it = middleIt+1; it < routes.end(); it++){ // rechts vom Element nach weiteren Vorkommen suchen
    		numLookups++;
    		if(it->destinationId == destID){
    			numRoutes++;
    		}else{
    			break;
    		}
    	}
    	for(std::vector<Route>::iterator it = middleIt-1;it >= routes.begin(); it--){ // links vom Element nach weiteren Vorkommen suchen
    		numLookups++;
    		if(it->destinationId == destID){
    			numRoutes++;
    		}else{
    			break;
    		}
    	}
    }
	return numRoutes;
}

// Evaluate the binarySearch function by calling it for every possible destination id (1..9541). 
// Return the number of lookups and the processing time as a pair of long longs.
// Use std::chrono for time measurement.
// Attention: sorting is *not* part of the evaluation and should be conducted beforehand.
std::pair<long long, long long> evaluateBinarySearch(std::vector<Route>& routes)
{
	long long numLookups = 0;
	long long duration = 0;

	std::sort(routes.begin(), routes.end(), 
		[](Route elem1, Route elem2){
			return elem1.destinationId < elem2.destinationId;
		}
	);

	auto start = std::chrono::system_clock::now();

	for(int i = 0; i < 9542 ; i++){
		binarySearch(i, routes, numLookups);
	}
	
	auto end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	duration = diff.count();
	return std::make_pair(numLookups, duration);
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		std::cout << "not enough arguments - USAGE: sort [ROUTE DATASET]" << std::endl;
		return -1;	// invalid number of parameters
	}

	static std::vector<Route> routes;

	std::cout << "Given path to routes.csv: " << argv[1] << std::endl;

	importRoutesData(argv[1], routes);

	auto result = evaluateLinearSearch(routes);
	std::cout << result.first << " - " << result.second << std::endl;
	result = evaluateBinarySearch(routes);	
	std::cout << result.first << " - " << result.second << std::endl;
	
	return 0;
}

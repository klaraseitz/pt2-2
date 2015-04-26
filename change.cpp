
#include <cstdlib>
#include <limits.h>
#include <iostream>
#include <string.h>
#include <fstream>

int moneyValues[12] = {5000,2000,1000,500,200,100,50,20,10,5,2,1}; // list of coin-sizes
int changeCount[12] = {0}; // array to save the amount of returned coins
int sizeOfChangeCount =sizeof(changeCount) / sizeof(changeCount[0]); // number of array-elements of changeCount

void change(const int due, const int paid)	
{
	int changeValue = paid - due;
	int sizeOfMoneyValues = sizeof(moneyValues) / sizeof(moneyValues[0]); // number of array-elements of moneyValues

	while(changeValue){	// as long as changeValue is not 0 calculate coin to add to changeCount
		for (int i = 0; i < sizeOfMoneyValues; i++)
		{
			if((changeValue - moneyValues[i]) >= 0 ){
				changeValue -= moneyValues[i];
				changeCount[i]++;
				break;
			}
		}
	}
}


void writeToFile(const int due, const int paid, char *filename){	// prints values to file
	std::ofstream file(std::string (filename) + std::string(".txt"));
	file << "coin, num\n";
	for (int i = 0; i < sizeOfChangeCount; i++)
			{
				if(changeCount[i] != 0){
				file << moneyValues[i] << ", " << changeCount[i] << "\n";
				}
			}

}



int main(int argc, char * argv[])
{
	if(argc < 3 || argc == 4 || argc > 5)
		{
		//std::cout << "Invalid number of parameters. \n"; //exercise sheet requests no output :(
		return 1;
	}
	const int due  = std::atoi(argv[1]); 
	const int paid = std::atoi(argv[2]); 

	if(paid < 0 || paid < due || paid > INT_MAX || due < 0 || due > INT_MAX ){
		//std::cout << "Invalid values. \n";
		return 1;
	}
	if(argc	== 5 && argv[3] != std::string("-o")){
		//std::cout << "Invalid optional parameter. Try '-o [filename]' for output to a file. \n";
		return 1;
	}


	change(due, paid);

	if(argc == 5 && argv[3] == std::string("-o")){	// option for output file  selected
		char *fileName = argv[4];
		writeToFile(due, paid, fileName);
	}
	else{
		std::cout << "coin, num \n";
		

			for (int i = 0; i < sizeOfChangeCount; i++)	// printing change values , no file option chosen
			{
				if(changeCount[i] != 0){
				std::cout << moneyValues[i] << ", " << changeCount[i] << std::endl;
				}
			}
	}
	return 0;
}

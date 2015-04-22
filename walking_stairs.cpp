
#include <cstdlib>
#include <iostream>


// ToDo: Exercise 2.d - adapt and implement combinations

unsigned long combinations(int number)
{
	if(number <= 0)
		return 0;
	if(number == 1){
		return 1;
	}


	unsigned long fibSum1=1;
	unsigned long fibSum2=1;
	unsigned long fibSum = 0;
	for(int i = 0;i < number-1;i++){
		fibSum = fibSum1 + fibSum2;
		fibSum1 = fibSum2;
		fibSum2 = fibSum;
	}
	return fibSum;
}


int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]); 

	std::cout << combinations(n) << "\n";

	return 0;
}

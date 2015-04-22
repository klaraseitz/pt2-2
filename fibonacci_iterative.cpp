
#include <cstdlib>
#include <iostream>

int sumCounter = 0;

unsigned long fibonacci(int number)
{
	// ToDo: Exercise 2.b - return 0 on bad arguments (large arguments are missing)	
	
	if(number <= 0)
		return 0;
	if(number == 1 || number == 2){
		return 1;
	}


	unsigned long fibSum1=1;
	unsigned long fibSum2=1;
	unsigned long fibSum = 0;
	for(int i = 1;i < number-1;i++){
		sumCounter++;
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

	// ToDo: Exercise 2.c - print calculation steps

	std::cout << n << " : " << fibonacci(n);
	std::cout << " : #" << sumCounter << "\n";

	return 0;
}

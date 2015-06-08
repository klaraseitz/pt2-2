
#include <cstdlib>
#include <iostream>

int sumCounter = 0;

unsigned long fibonacci(int number)
{
	if(number <= 0)	// check for negative input
		return 0;
	if(number == 1 || number == 2){	// starting values
		return 1;
	}


	unsigned long fibSum1=1;
	unsigned long fibSum2=1;
	unsigned long fibSum = 0;
	for(int i = 1;i < number-1;i++){	// computes fibonacci numbers
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

	if(n>93)	// more numbers are not necessary for this exercise but one could use the Moivre Binet Formulae to calculate n in fib(n) < ULONG_MAX
		return 1; // invalid input

	std::cout << n << " : " << fibonacci(n);	// print solution of fibonacci
	std::cout << " : #" << sumCounter << "\n";	// print number of summations

	return 0;
}

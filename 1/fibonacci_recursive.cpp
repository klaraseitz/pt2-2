
#include <cstdlib>
#include <iostream>

int sumcounter;

int fibonacci(int number)
{
	if(number <= 2){	// exit condition
		sumcounter++;
		return 1;
	}
	if(number<=0){	// check for negative numbers (and 0)
		return 0;
	}
	else{	// calculate fibonacci numbers
		sumcounter++;
		return fibonacci(number-1)+fibonacci(number-2);
	}

	return 0;
}

int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]); 

	if(n>46)	// more numbers take too long to calculate and with integer on our system 46 is the last possible number to be calculated
				// found out by using the Moivre Binet Formulae to calculate n in fib(n) < INT_MAX (here: 2147483647)
		return 1; // input too large for integer

	std::cout << n << " : " << fibonacci(n); // ((insight: muss in extra Zeile stehen, damit Sumcounter berechnet wi2147483647rd vor der Ausgabe))
	std::cout << " : #" << sumcounter << "\n";

	return 0;
}

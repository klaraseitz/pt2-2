
#include <cstdlib>
#include <iostream>

int sumcounter;

int fibonacci(int number)
{
	// ToDo: Exercise 2.b - return 0 on bad arguments (too large arguments missing)


	if(number <= 2){
		sumcounter++;
		return 1;
	}
	if(number<=0){
		return 0;
	}
	else{
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

	// ToDo: Exercise 2.c - print calculation steps

	std::cout << n << " : " << fibonacci(n); // muss in extra Zeile stehen, damit Sumcounter berechnet wird vor der Ausgabe
	std::cout << " : #" << sumcounter << "\n";

	return 0;
}

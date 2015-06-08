#include <functional>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: m_fibonacci(false)
		, m_centeredTriangular(false)
		, m_powSum(false)
	{
		for (int i = 1; i < argc; i++)
		{
			if (!strcmp(argv[i], "-fib"))
			{
				m_fibonacci = true;
			}
			else if (!strcmp(argv[i], "-ct"))
			{
				m_centeredTriangular = true;
			}
			else if (!strcmp(argv[i], "-pow"))
			{
				m_powSum = true;
			}
		}
	}

	bool m_fibonacci;
	bool m_centeredTriangular;
	bool m_powSum;
};

// ToDo 4.2a - Calculate and return the fibonacci number for the given input value
int fibonacci(int number)
{
	if (number == 0) { 
        return 0;
    }
    if (number == 1) {
        return 1;
    }
    return fibonacci(number - 1) + fibonacci(number - 2);
}

// ToDo 4.2a - Calculate and return the centered triangular number for the given input value
int centeredTriangular(int number)
{
	return (3*pow(number,2)+3*number+2)/2;
}

// ToDo 4.2a - Calculate and return the sum of i^exp for all i in [1..number]
int powSum(int number, int exp)
{
	int result = 0;
	for (int i=1; i<=number; i++){
		result += pow(i, exp);
	}
	return result;
}

// ToDo 4.2c - Call each function inside the functions vector. Print the results via std::cout
void applyFuntions(std::vector<std::function<int(int)>> & functions, int number)
{
	for (auto & element : functions) {
    	std::cout << element(number) << std::endl;
	}

}

int main(int argc, char * argv[])
{
	std::vector<std::function<int(int)>> functions;
	CommandLineParameter cmd(argc, argv);
	
	if (cmd.m_fibonacci)
	{
		// ToDo 4.2b - Add the fibonacci function directly to the functions vector
		std::function<int(int)> fib = &fibonacci;
		functions.push_back(fib);
	}
	if (cmd.m_centeredTriangular)
	{
		// ToDo 4.2b - Add a lambda expression that calls the centeredTriangular function to the functions vector
		auto callCenteredTriangular = [] (int number) {return centeredTriangular(number); };
		functions.push_back(callCenteredTriangular);
	}
	if (cmd.m_powSum)
	{
		// ToDo 4.2b - Add the powSum function using a std::bind expression to the functions vector. Use a constant value (e.g., 2) for the exp argument 
		functions.push_back(std::bind(powSum,std::placeholders::_1,2));
	}

	for (int i = 1; i < 45; i++)
	{
		applyFuntions(functions, i);
	}
}

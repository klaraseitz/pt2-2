#include <limits.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long maximum;

int triangular(int number)
{
	/* n(n+1) / 2 = n/2 * (n+1) <= INT_MAX
	n berechnen, dann number testen
	Berechnung:
	n/2 * (n+1) <= INT_MAX
	n*(n+1)<=2*INT_MAX
	n^2+n <=2*INT_MAX
	p-q-Formel anwenden:
	n1 = -1/2 + sqrt(1/4 + 2*INT_MAX) -> gesuchtes Maximum
	n2 = -1/2 - sqrt(1/4 + 2*INT_MAX)
	*/
	
	maximum = -0.5 + sqrt(0.25 + 2* (long)INT_MAX);	//Maximum ist meist: 65535


	if(number <= maximum && number > -1){
		double result = 0.5*number * (number+1);
		return (int)result;}
	else
		{return -1;}
	
}

void pretty_print(int decimal)
{
	if(decimal == 0){
	std::cout << 0;}

	// Länge der Zahl herausfinden:
	int number = decimal;
	int counter = 0;

	while (number) { 		// loop till there's nothing left
		counter++;
		number /= 10; }		// "right shift" the number

	// Zahl in Array umwandeln, dabei umdrehen (rückwärts speichern):
	int i = 0; // the array index
	char numberArray[counter]; 

	while (decimal) { 		// loop till there's nothing left
		numberArray[i++] = decimal % 10; 	// assign the last digit
		decimal /= 10; 		// "right shift" the number
	}

	// Kleine Zahl (braucht keine Punkte) ausgeben:
	if(sizeof(numberArray) <=3){
		for (int i =sizeof(numberArray)-1; i>=0; i--) 
    	std::cout << (int)numberArray[i];
	}
	// Große Zahl mit Punkten ausgeben:
	else{
		for (int i =sizeof(numberArray)-1; i>=0; i--) 
			{std::cout << (int)numberArray[i];
				if(i%3==0&&i!=0){
					std::cout <<".";}}
	}
	

}

int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]); 
	
	//Get Triangular Number:
	int triangularNr = triangular(n);
	
	//Check for error value (-1):
	if(triangularNr == -1){
		std::cout << "domain = [0;" << maximum << "],codomain = [0;" ;
		pretty_print(triangular(maximum));
		std::cout << "]\n";
	}
	//Print answer:
	else{
		std::cout << n << "-te Dreieckszahl: ";
		pretty_print(triangularNr);
		std::cout << "\n";
 	}

	return 0;
}

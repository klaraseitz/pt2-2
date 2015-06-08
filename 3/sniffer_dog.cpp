#include <climits>
#include <iostream>
#include <vector>
#include <algorithm>

// todo 3.3: Calculate minimum time needed for both dogs to return 
int minTimeDogTraining(int searchValueX, int searchValueY, const std::vector<int> &boxes)
{
	// find both elements in Array and save their index:
	int dog1Left=-1;
	int dog2Left=-1;
	int boxAmount = (int)boxes.size();
	int halfBoxAmout = (int)((boxAmount+1)/2);
	for (int i = 0; i < halfBoxAmout; i++)
	{
		if(boxes[i]==searchValueX && dog1Left==-1){
			dog1Left = i;
		}else if(boxes[i]==searchValueY && dog2Left==-1){
			dog2Left = i;
		}
	}
	int dog1Right =-1;
	int dog2Right =-1;
	for (int i = boxAmount-1; i >= halfBoxAmout; i--)
	{
		if(boxes[i]==searchValueX){
			dog1Right = i;
		}else if(boxes[i]==searchValueY){
			dog2Right = i;
		}
	}	

	if( (dog1Left == -1 && dog1Right == -1) || (dog2Right == -1 && dog2Left == -1) ){
		return -1;
	}else{
		// Länge von rechts und links für beide Hunde berechnen, und Minimum finden:
		int minDog1;
		if(dog1Left==-1){
			minDog1 = boxAmount - dog1Right;
		}else if(dog1Right == -1){
			minDog1 = dog1Left + 1;
		}else{
			minDog1 = std::min( (boxAmount - dog1Right), (dog1Left + 1) );
		}
		int minDog2;
		if(dog2Left==-1){
			minDog2 = boxAmount - dog2Right;
		}else if(dog2Right == -1){
			minDog2 = dog2Left + 1;
		}else{
			minDog2 = std::min( (boxAmount - dog2Right), (dog2Left + 1) );
		}		
		// Maximum der beiden Zeiten berechnen und mindestwartezeit zu finden:
		return std::max(minDog1, minDog2);
	}



}

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "Missing parameters. Usage: <searchValue1> <searchValue2> [box1] [box2] [...]" << std::endl;
		return 0;
	}

	const int searchValueX = std::atoi(argv[1]);
	const int searchValueY = std::atoi(argv[2]);
	if(searchValueX < 1 || searchValueY < 1)
	{
		std::cerr << "Search values must be greater than zero!";
	}
	
	if (searchValueX == searchValueY)
	{
		std::cerr << "Search values must be different!";
		return 1;
	}

	std::vector<int> boxes;
	for (int i = 3; i < argc; i++)
	{
		int value = std::atoi(argv[i]);
		if (value < 1)
		{
			std::cerr << "Box value must be greater than zero!";
			return 2;
		}

		boxes.push_back(std::atoi(argv[i]));
	}

	int minTime = minTimeDogTraining(searchValueX, searchValueY, boxes);
	if (minTime == -1)
	{
		std::cout << "There exists no two boxes with the given search values!" << std::endl;
	}
	else
	{
		std::cout << "The coach will have to wait at least " << minTime << " seconds!" << std::endl;
	}
	
	return 0;
}
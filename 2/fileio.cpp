#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

// Split strings by given delimiter
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool isValueCorrect(const std::string &teststring, const int &column)
{
	std::regex regExp;
	
	switch (column)
	{
		//regExp for AirportID
        case 0:
            regExp = "[0-9]+";
            break;
		//regExp for Name
        /*case 1:
            //regExp = "\" [A-Z][a-z]+(([A-Z][a-z]+_?-? ?)|[a-z]+_?-? ?)+\"";
            regExp = "\"[A-Z][a-z]+-?.?'?( ?[A-Z]?[a-z]+-?.?'?)*\"";
            break;
        //regExp for City
        case 2:
            regExp = "\"[A-Z][a-z]+-?.?'?( ?[A-Z]?[a-z]+-?.?'?)*\"";
            break;
		//regExp for Country
        case 3:
            regExp = "\"[A-Z][a-z]+-?.?'?( ?[A-Z]?[a-z]+-?.?'?)*\"";
            break;*/
		//regExp for IATA/FAA
        case 4:
            regExp = "\"([A-Z]{3}|)\"";
            break;   
        //regExp for ICAO
        case 5:
            regExp = "\"([A-Z]{4}|)\"";
            break;
		//regExp for Latitude
        case 6:
            regExp = "-?[0-9]{1,3}(.[0-9]{1,6})?";
            break; 
        //regExp for Longitude
        case 7:
            regExp = "-?[0-9]{1,3}(.[0-9]{1,6})?";
            break;
        //regExp for Altitude 
        case 8:
            regExp = "[0-9]+"; // sinvoller Maximalwert fehlt
            break;
        //regExp for Timezone
        case 9:
            regExp = "-?((1[0-2])|[0-9])(.[0-9])?";
            break;
        //regExp for DST
        case 10:
            regExp = "\"(E|A|S|O|Z|N|U)\"";
            break;
        //regExp for Database time zone
        case 11:
            regExp = "\"[A-Z][a-z]+/(([A-Z][a-z]+_?-?)|[a-z]+_?-?)+\""; 
            break;     
		default:
			regExp = ".*";
			break;
	}
	
	return std::regex_match(teststring, regExp);
}



void readTokensAndLines(char* path)
{
	std::ifstream file(path);
	std::string parsed, line;
	std::ofstream outputFile(std::string ("fileio") + std::string(".log"));
	
	while (std::getline(file, line)) {
		std::istringstream linestream;
		linestream.str(line);
		std::vector<std::string> lineItems = split(line, ',');

        // Test all values for errors and then log them in the fileio.log
		for (int i = 0; i < 12; i++)
		{
			if(isValueCorrect(lineItems[i],i)==0){ 
				outputFile << "Wrong entry in value " << 0 << ": " << lineItems[i] << std::endl;
				outputFile << line << std::endl;
			} 
		}
		std::cout << lineItems[1] << " - " << lineItems[11] << std::endl;
	}
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		std::cout << "not enough arguments - USAGE: fileio [DATASET]" << std::endl;
		return -1;	// invalid number of parameters
	}
	
	std::cout << "Given path to airports.dat: " << argv[1] << std::endl;

	readTokensAndLines(argv[1]);

	return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cfloat>
#include <iomanip>
#include <vector>
#include <stdexcept> //because it said logic_error not a member of std...

std::ofstream outputFile(std::string ("exceptions") + std::string(".log"));

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

// transforms a string to a date. Throws a logic_error if year is *not* between 2005 and 2015
std::tm stringToTime(std::string date)
{
	std::tm t;
#if defined(__GNUC__) && (__GNUC__ < 5)
	strptime(date.c_str(), "%d.%m.%Y", &t);
#else
	std::istringstream ss(date);
	ss >> std::get_time(&t, "%d.%m.%Y");
#endif

	if(t.tm_year < 105 || t.tm_year > 115)
		throw std::logic_error("Year should be between 2005 and 2015");

	return t;
}

struct FormatException
{
	int m_actLine;
	std::string m_actFields;
};

void parseLine(std::string line, int lineNum)
{
	const std::string fieldNames[3] = { "Date", "Temperature", "Rainfall" };

	std::vector<std::string> lineItems = split(line, ';');

    // 3.2b: parse a given line, check dates by calling stringToTime, check temperature/rainfall by calling std::stof. 
	try{
		std::cout << lineItems[0] << std::endl;
		stringToTime(lineItems[0]);
		
	}
	catch(std::exception &e){
		std::cerr << "Wrong date.\n";
		FormatException exception;
		exception.m_actLine = lineNum;
		exception.m_actFields = "Date";
		throw exception;
	}
	try{
		std::stof(lineItems[1]);
		std::cout << lineItems[1] << std::endl;
	}
	catch(const std::invalid_argument& ia){
		std::cerr << "Invalid temperature argument.\n";

		FormatException exception;
		exception.m_actLine = lineNum;
		exception.m_actFields = "Temperature";
		throw exception;
	}
	catch(const std::out_of_range& oor){
		std::cerr << "Temperature value out of range.\n";
		FormatException exception;
		exception.m_actLine = lineNum;
		exception.m_actFields = "Temperature";
		throw exception;
	}
	try{
		std::stof(lineItems[2]);
		std::cout << lineItems[2] << std::endl;
	}
	catch(const std::invalid_argument& ia){
		std::cerr << "Invalid rainfall argument.\n";
		//std::string exceptionMsg= lineNum + "; Temperature;" + lineItems[1];
		FormatException exception;
		exception.m_actLine = lineNum;
		exception.m_actFields = "Rainfall";
		throw exception;
	}
	catch(const std::out_of_range& oor){
		std::cerr << "Rainfall value out of range.\n";
		FormatException exception;
		exception.m_actLine = lineNum;
		exception.m_actFields = "Rainfall";
		throw exception;
	}

    // Catch all exceptions thrown by these methods. If there have been any exceptions, 
    // aggregate all necessary information into an instance of FormatException and throw that instance.
}

// todo 3.2c.
void writeOutFormatException(const FormatException & e)
{
	try{
	outputFile << "In Zeile " << e.m_actLine << " ist das Feld '" << e.m_actFields << "' falsch formatiert" << std::endl; 
	}catch (std::ios_base::failure f){
		std::cerr << "Exception opening/reading/closing file\n";
	}	
    // todo 3.2d: export information (i.e., line number + invalid data fields) about exception to a logfile.
    // todo 3.2d: catch ios_base::failure
}

void checkData(std::string path)
{
	int validLines = 0;
	int invalidLines = 0;
	int countLines=1;
	try {
		std::ifstream file(path);
		std::string line; 
		while (std::getline(file, line))
		{
			if(countLines ==1){
				countLines++;
				continue;
			}
			try{
				parseLine(line, countLines);
			}catch (FormatException fe){
				writeOutFormatException(fe);
				invalidLines++;
			}
			countLines++;   
		}
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file\n";
	}
	
	validLines = countLines - invalidLines;

    // todo 3.2a: open file + read each line + call parseLine function (catch ifstream::failure)
    // todo 3.2c: read each line + call parseLine function (catch FormatException) + count valid + invalid lines

	std::cout << "valid lines: " << validLines << " - invalid lines: " << invalidLines << std::endl;
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		std::cout << "Invalid number of arguments - USAGE: exceptions [DATASET]" << std::endl;
		return -1;
	}

	checkData(argv[1]);

	return 0;
}

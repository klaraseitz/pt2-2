#include <iostream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <array>
#include <fstream>
#include <sstream>
#include <iterator>

#if defined _WIN32
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

struct fileInfo {
	int size;
	int count;
	std::vector<std::string> adresses;
};
std::map<std::string, int> flattenNames;
std::map<std::string, int>::iterator mapIt2;
std::map<std::string,fileInfo> fileInfoMap;
std::map<std::string,fileInfo>::iterator mapIt;
int pathlength;

struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: fsize(false)
		 , fnum(false)
		 , files(false)
		 , flattening(false)
		 , oSet(false)
		 , oName("")
		 , pathName("")
	{
		for (int i = 1; i < argc; i ++ )
		{
			if (!strcmp(argv[i], "-fsize"))
			{
				fsize=true;
			}
			else if (!strcmp(argv[i], "-fnum"))
			{
				fnum=true;
			}
			else if (!strcmp(argv[i], "-files"))
			{
				files=true;
			}
			else if (!strcmp(argv[i], "-o"))
			{
				if(i < argc-1){
					oSet=true;
					oName = argv[i+1];
				}
				else
				{
					std::cerr << "Missing value for " << argv[i] << std::endl;
				}
				i++;

			}
			else if(!strcmp(argv[i], "-flattening"))
			{
				flattening=true;
			}
			else
			{
				pathName = argv[i];
			}
		}
	}

bool fsize;
bool fnum;
bool files;
bool flattening;
bool oSet;
std::string oName;
std::string pathName;
};


// Beware: this function only works when the directory is empty..
void removeDirectory(std::string dir)
{
	#if defined _WIN32
    	_rmdir(dir.data());
	#elif defined __GNUC__
 	   rmdir(dir.data());
	#endif
}

void createDirectory(std::string dir)
{
	#if defined _WIN32
  	  _mkdir(dir.data());
	#elif defined __GNUC__
 	   mkdir(dir.data(), 0777);
	#endif
}

// Splits a given string on every appearance of a given char
std::vector<std::string> split( std::string str, char trenner )
{
    // Rückgabevektor definieren
    std::vector<std::string> result;
	while ( true )
    {
        int pos = str.find_first_of( trenner );
        // den Teil des Strings vom Anfang bis zur ermittelten Pos holen
        std::string cur = str.substr( 0, pos );
        result.push_back( cur );
 		// npos = End of string, Trennzeichen war also nicht mehr enthalten
        if ( pos == std::string::npos )
            break;
        // Stirng anpassen und ersten Teil entfernen
        str = str.substr( pos + 1 );
    }
    return result;
}

// Takes a path (as string) and returns a new string where every '/' is a '_'
std::string underscoredName(std::string str){
	std::string result="";
	while( true )
	{
		int pos = str.find_first_of( '/' );
		std::string neuerTeilString = str.substr( 0, pos );
		result = result + "_" + neuerTeilString;
		if ( pos == std::string::npos ){
            break;
        }
        str = str.substr( pos + 1 );
	}
	result = result.substr( 2 );
	return result;
}

// Prints a vector to the console
void printVector(std::vector<std::string> vec){
	for( std::vector<std::string>::const_iterator i=vec.begin(); i!=vec.end(); ++i )
		std::cout <<*i<<std::endl;
}

// Copies a file from one path to another
void copyFile(std::string from, std::string to) {
    
    std::ifstream source(from, std::ios::binary);
    std::ofstream dest(to, std::ios::binary);

    std::istreambuf_iterator<char> begin_source(source);
    std::istreambuf_iterator<char> end_source;
    std::ostreambuf_iterator<char> begin_dest(dest); 
    copy(begin_source, end_source, begin_dest);

    source.close();
    dest.close();

    return;
}

// Implements the flattening function of the programm
void flattenDirectory(std::string path, std::string targetDir)
{
	DIR *dir;
	struct dirent *ent;
	dir = opendir(path.c_str());
	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string name = ent->d_name;
			if (ent->d_type == DT_DIR)
			{
				if(name== "." || name=="..") continue;
				std::string newpath = path + "/" + name;
				flattenDirectory(newpath, targetDir);			
			}
			else
			{
				struct stat fileinfo;
				std::string fullpath = path + "/" + name;
				if (!stat(fullpath.c_str(), &fileinfo))
				{
					// determines a new path and a new name for the found file
					std::string relPath = fullpath.erase(1, pathlength);
					std::string fileName_underscored = underscoredName(relPath);		
					// checks if the filename was used before and creates a new name
					if (flattenNames.find(fileName_underscored) == flattenNames.end() )
					{
						flattenNames.insert(std::pair<std::string,int>(fileName_underscored,1));
					}
					else
					{
						mapIt2 = flattenNames.find(fileName_underscored);
						mapIt2->second +=1;
						std::string count = std::to_string(mapIt2->second);
						std::vector<std::string> nameParts = split(fileName_underscored, '.');
						fileName_underscored = nameParts.front() + count + "." + nameParts.back();
					}
					copyFile(fullpath, targetDir + "/" + fileName_underscored);
				}
				else
				{
					std::cout << "(stat() failed for this file)\n" << std::endl;
				}
			}
		}
		closedir(dir);
	}
	else
	{
		std::cout << "Failed to read directory" << std::endl;
	}

}

void traverseDirectory(std::string path)
{
	DIR *dir;
	struct dirent *ent;
	dir = opendir(path.c_str());
	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string name = ent->d_name;
			if (ent->d_type == DT_DIR)
			{
				if(name== "." || name=="..") continue;
				// std::cout << "Dir: " << name << std::endl;
				// uses recursion to get every subfolder
				std::string newpath = path + "/" + name;
				traverseDirectory(newpath);			
			}
			else
			{
				// std::cout << "File: " << name << std::endl;
				struct stat fileinfo;
				std::string fullpath = path + "/" + name;
				// std::cout << "Fullpath: " << fullpath << std::endl;
				if (!stat(fullpath.c_str(), &fileinfo))
				{
					// std::cout << (unsigned int)fileinfo.st_size << "bytes" << std::endl;
					int filesize = (int)fileinfo.st_size;
					// sorts the files in categories depending on the file extension
					std::vector<std::string> nameGetrennt = split(name, '.');
					std::string dateiendung = nameGetrennt.back();
					// shortens the path to its realtive part
					std::string relPath = fullpath.erase(1, pathlength);
					// Searches for the found file extension and updates the map
					if (fileInfoMap.find(dateiendung) == fileInfoMap.end() )
					{
						struct fileInfo start;
						start.size = filesize;
						start.count = 1;
						start.adresses.push_back(relPath);
						fileInfoMap.insert(std::pair<std::string,fileInfo>(dateiendung,start));
					}
					else
					{
						mapIt = fileInfoMap.find(dateiendung);
						mapIt->second.size +=filesize;
						mapIt->second.count +=1;
						mapIt->second.adresses.push_back(relPath);
					}
				}
				else
				{
					std::cout << "(stat() failed for this file)\n" << std::endl;
				}
			}
		}
		closedir(dir);
	}
	else
	{
		std::cout << "Failed to read directory" << std::endl;
	}
}

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		std::cout << "not enough arguments - USAGE: dirinfo [DIR] -o [OUTPUT.csv] COLUMNS" << std::endl;
		std::cout << "possible COLUMNS are:" << std::endl;
		std::cout << "-fsize\t\t\tSummarized file size for files with same extension will be exported" << std::endl;
        std::cout << "-fnum\t\t\tSummarized number of files with same extension will be exported" << std::endl;
		std::cout << "-files\t\t\tRelative paths of files with same extension will be exported" << std::endl;
		// Wir treffen folgende Annahme: Die Ausgabe des flattenings erfolgt in den Ordner dessen Pfad angegeben wird.
		// Die Aufgabenstellung spezifiziert dies nicht genau, diese Lösung scheint uns am sinnvollsten.
		// Alternativ müsste man mit einem weiteren Argument den gewünschten Zielpfad abfragen und diesen
		// (nach ggf. erstellen) als targetDir an die Funktion flatteningDirectory übergeben.
		std::cout << "-flattening\t\tCopies every file of the subfolders in the [DIR]-folder" << std::endl;
		
		return -1;	// invalid number of parameters
	}
	// createDirectory("foo");

    CommandLineParameter cmd(argc, argv);
    
    // Calculates the length of the given path (used to shorten the paths to the relative part)
    std::string path = cmd.pathName;
    pathlength = path.size();

	traverseDirectory(cmd.pathName);
	
	// creates a outputfile if requested
	if(cmd.oSet){
		std::ofstream outputFile(cmd.oName);
		
		// writes the collected information in the file if flag is true
		for(mapIt = fileInfoMap.begin(); mapIt != fileInfoMap.end(); ++mapIt)
		{
			outputFile << mapIt->first << ";";
			if(cmd.fsize)
			{
				outputFile << mapIt->second.size << ";";
			}
			if(cmd.fnum)
			{
				outputFile << mapIt->second.count << ";";
			}
			if(cmd.files)
			{
				for( std::vector<std::string>::const_iterator i=mapIt->second.adresses.begin(); i!=mapIt->second.adresses.end(); ++i )
					outputFile << *i << ";";
			}
			outputFile << std::endl;
		}
	}
	// if the flattening flag is set the directory will be flatened
	if(cmd.flattening)
	{
		flattenDirectory(cmd.pathName, cmd.pathName);
	}
	
	// getchar();
}


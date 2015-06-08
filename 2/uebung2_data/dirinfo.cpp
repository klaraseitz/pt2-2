#include <iostream>
#include <dirent.h>
#include <string>

#if defined _WIN32
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

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
				std::cout << "Dir: " << name << std::endl;
				//ToDo recursive
			}
			else
			{
				std::cout << "File: " << name << std::endl;
				struct stat fileinfo;
				std::string fullpath = path + "/" + name;
				std::cout << "Fullpath: " << fullpath << std::endl;
				if (!stat(fullpath.c_str(), &fileinfo))
				{
					std::cout << (unsigned int)fileinfo.st_size << "bytes" << std::endl;
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


		return -1;	// invalid number of parameters
	}

    createDirectory("foo");
	traverseDirectory(argv[1]);


	getchar();
}
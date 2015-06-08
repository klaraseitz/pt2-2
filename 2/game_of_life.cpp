#include <iostream>
#include <ctime>
#include <cstring>
#include <stdlib.h>
#include "bitmap_image.hpp"

struct Raster {
	
	Raster(Raster && old){
//rasterCopy.data = nullptr;
	}

	Raster(int w, int h) : width(w), height(h)
	{
		data = new int[width*height];
	}

	Raster(int w, int h, float seedProbability) : width(w), height(h)
	{
		int randomNr;
		data = new int[width*height];
		srand (time(NULL));
		for (int n=0; n<width; n++)
		{
    		for (int m=0; m<height; m++)
    			{
					randomNr = rand() % 10;
					if(randomNr < (seedProbability*10)){
						data[m*width+n] = 1;
					}else{
						data[m*width+n] = 0;
					}
				}
		}
	
	}

	Raster(const std::string &filename)
	{
		bitmap_image image(filename);


		if (!image)
		{
			std::cerr << "Could not open bitmap!" << std::endl;
		}

		height = image.height();
		width = image.width();
		unsigned char red;
   		unsigned char green;
   		unsigned char blue;


		data = new int[width*height]; // pseudo multidimensional array

		for (int n=0; n<width; n++)
		{
    		for (int m=0; m<height; m++)
    			{
    				image.get_pixel(n,m,red,green,blue);
    				if((int)red == 0 && (int)green == 0 && (int)blue ==0){ // Pixel is black 
    					data[m*width+n]=1;
    				}else{
    					data[m*width+n]=0;
    				}
    			}
		}
	}

	void save(const std::string &filename)
	{
		bitmap_image outputImage(width,height);
		for (int n=0; n<width; n++)
		{
    		for (int m=0; m<height; m++)
    			{
    				if(data[m*width+n]==1){ // value is 1 
    					outputImage.set_pixel(n,m,0,0,0);	// set pixel to black
    				}else{ // value is 0
    					outputImage.set_pixel(n,m,255,255,255); // set pixel to white
    				}
    			}
		}
		outputImage.save_image(filename);	
	}

	~Raster()
	{


		delete[] data;
	}

	int width;
	int height;
	int* data;
};

// This struct parses all necessary command line parameters. It is already complete and doesn't have to be modified. However - feel free to add support for additional arguments if you like.
struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: width(0)
		, height(0)
		, invasionFactor(0)
		, isTorus(false)
		, maxIterations(20)
	{
		if (argc % 2 == 0)
		{
			std::cerr << "Missing value for " << argv[argc - 1] << std::endl;
			argc--;
		}

		for (int i = 1; i < argc; i += 2)
		{
			if (!strcmp(argv[i], "-w"))
			{
				width = atoi(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-h"))
			{
				height = atoi(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-s"))
			{
				seedProbability = atof(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-p"))
			{
				patternFilename = argv[i + 1];
			}
			else if (!strcmp(argv[i], "-o"))
			{
				outputDirectory = argv[i + 1];
			}
			else if (!strcmp(argv[i], "-iv"))
			{
				invasionFactor = atof(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-t"))
			{
				isTorus = strcmp(argv[i + 1], "0") != 0;
			}
			else if (!strcmp(argv[i], "-i"))
			{
				maxIterations = atoi(argv[i + 1]);
			}
		}

		if ((width != 0 || height != 0) && !patternFilename.empty())
		{
			std::cout << "Width and height are ignored, because pattern is defined." << std::endl;
		}

		if (width < 0 || height < 0)
		{
			std::cerr << "Width or height has a invalid value." << std::endl;
			width = 0;
			height = 0;
		}
	}

	int width;
	int height;
	float seedProbability;
	std::string patternFilename;
	std::string outputDirectory;
	float invasionFactor;
	bool isTorus;
	int maxIterations;
};

int neighborValue(const Raster &raster, int x, int y, bool isTorus)
{
	int valueCounter = 0;
	for (int yy=-1; yy <= 1; yy++)
    {
        for (int xx=-1; xx <= 1; xx++)
        {
        	if( (x+xx >=raster.width || x+xx < 0 ||y+yy >=raster.height || y+yy < 0) && isTorus ==0){
        		continue;
        	}
        	if((x+xx >=raster.width || x+xx < 0 ||y+yy >=raster.height || y+yy < 0) && isTorus ==1){
        		if(raster.data[((y+yy)%raster.height)*raster.width+((x+xx)%raster.width)]==1){
        			valueCounter++;
        		}
        	}else if(raster.data[(y+yy)*raster.width+(x+xx)]==1 && (xx!=0 || yy!=0)){
        		
        		valueCounter++;
        	}
        }
    }	
    return valueCounter;
}

void simulateInvasion(Raster &raster, float invasionFactor)
{
	if (invasionFactor <= 0)
	{
		return;
	}
	int randomNr;
	srand(time(NULL));
	for (int n=0; n<raster.width; n++)
	{
    	for (int m=0; m<raster.height; m++)
    	{
    		randomNr = std::rand()%1000;
			if(randomNr < (invasionFactor*1000)){
				if(raster.data[m*raster.width+n] == 0){
					raster.data[m*raster.width+n] = 1;
				}else{
					raster.data[m*raster.width+n] = 0;
				}
			}
		}
	}

}

void simulateNextState(Raster &raster, bool isTorus)
{
	Raster rasterCopy = Raster(raster.width, raster.height);
	for (int n=0; n<raster.width; n++)
	{
		for (int m=0; m<raster.height; m++)
			{	
				
				if(raster.data[m*raster.width+n]==0){ // Pixel ist tot
					if(neighborValue(raster, n, m, isTorus)==3){
						rasterCopy.data[m*raster.width+n]=1; 	// Pixel wird geboren
						
					}else{
						rasterCopy.data[m*raster.width+n]=0;	// Pixel bleibt tot

					}
				}else{ // Pixel lebt
					
					if(neighborValue(raster, n, m, isTorus)==2||neighborValue(raster, n, m, isTorus)==3)
					{
						rasterCopy.data[m*raster.width+n]=1;	// Pixel bleibt am Leben
					}else{
						rasterCopy.data[m*raster.width+n]=0;	// Pixel stirbt
					}
				}
			}
	}
	for (int n=0; n<raster.width; n++)
	{
		for (int m=0; m<raster.height; m++)
			{
				raster.data[m*raster.width+n] = rasterCopy.data[m*rasterCopy.width+n];
			 }
	}
}

int main(int argc, char* argv[])
{
	Raster* raster = nullptr;

	CommandLineParameter cmd(argc, argv);
	if (!cmd.patternFilename.empty())
	{
		raster = new Raster(cmd.patternFilename);
	}
	else
	{
		raster = new Raster(cmd.width, cmd.height, cmd.seedProbability);
	}

	for (int iteration = 0; iteration <= cmd.maxIterations; iteration++)
	{
		raster->save(cmd.outputDirectory + "game_of_life_" + std::to_string(iteration) + ".bmp");
		simulateInvasion(*raster, cmd.invasionFactor);
		simulateNextState(*raster, cmd.isTorus);
	}

	delete raster;
	return 0;
}
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
//#include "Mesh.h"
//#include "Clock.h"
//#include "util.h"
//#include "Properties.h"
#include "Harris3D.h"



int main(int argc, char *argv[])
{
    std::string nomFichier = "/Users/lemaitrecedric/Documents/Ergocycle/data/DepthImg/Cyclist1.bin";
		Harris3D hd(nomFichier);
	
	return EXIT_SUCCESS;
}

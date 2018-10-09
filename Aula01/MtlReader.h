

#pragma once
#ifndef mtl_reader_H
#define mtl_reader_H

#include <vector>
#include <string>


#include <GL/glew.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <fstream>

#include "Material.h"

using namespace std;

class MtlReader
{
public:
	MtlReader();
	~MtlReader();
	
	unordered_map<string, Material*> readMaterialsFile(string filename);
};


#endif
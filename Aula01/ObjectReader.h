#pragma once


#include <fstream>
#include <sstream>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "Mesh.h"

using namespace std;

class ObjectReader
{
public:
	ObjectReader();
	~ObjectReader();
	std::vector<string> explode(string delimiter, string source, int limit = numeric_limits<int>::max());
	Mesh* read(std::string filename);
};


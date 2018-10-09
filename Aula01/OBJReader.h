#pragma once

#include <string>
#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>

class OBJReader
{

private:
	std::string materials_file;

public:
	OBJReader();
	~OBJReader();
	Mesh *read(std::string filename);
	void readVertex(std::stringstream& line, Mesh& mesh);
	void readMapping(std::stringstream& sline, Mesh& mesh);
	void readNormal(std::stringstream& sline, Mesh& mesh);
	void readFace(std::stringstream& sline, Group& group);
	int readNextInt(std::stringstream& stoken);
};


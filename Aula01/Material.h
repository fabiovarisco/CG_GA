#pragma once

#ifndef mtl_file_H
#define mtl_file_H


#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>

typedef struct Material
{
	std::string mtlName;
	float ns;
	float ni;
	float d;
	glm::vec3 tf;
	float illum;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ke;
	std::string map_kd;
	GLuint textureId;
} Material;


#endif
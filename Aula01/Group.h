
#include <string>
#include <vector>
#define GLEW_STATIC
// External Libs
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "Face.h"
class Mesh;

#pragma once

class Group
{
private: 
	std::string name, material;
	std::vector<Face*> faces;
public:
	Group();
	~Group();
	std::string &getName();
	std::string &getMaterial();
	std::vector<Face*> &getFaces();
	void setName(std::string i_name);
	void setMaterial(std::string i_material);
	void setFaces(std::vector<Face*> i_faces);
	GLuint vEBO, nEBO, tEBO;
	int nVertices, nNormals, nTexts;
	void setup(Mesh* mesh, GLuint vVBO, GLuint nVBO, GLuint tVBO);
	void draw(GLuint vVBO, GLuint nVBO, GLuint tVBO);


};


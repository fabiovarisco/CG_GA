#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Group.h"

class Mesh
{
private: 
	std::vector<glm::vec2> mappings;
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> normals;
	std::vector<Group*> groups;

public:
	Mesh();
	~Mesh();
	std::string materialsFile;
	
	std::vector<glm::vec2> &getMappings();
	std::vector<glm::vec3> &getVertexes();
	std::vector<glm::vec3> &getNormals();
	std::vector<Group*> &getGroups();
	void setMappings(std::vector<glm::vec2> *i_mappings);
	void setVertexes(std::vector<glm::vec3> *i_vertexes);
	void setNormals(std::vector<glm::vec3> *i_normals);
	void setGroups(std::vector<Group*> *i_groups);
};


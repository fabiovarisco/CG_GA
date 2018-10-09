#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


std::vector<glm::vec2> &Mesh::getMappings() {
	return mappings;
}

std::vector<glm::vec3> &Mesh::getVertexes() {
	return vertexes;
}
std::vector<glm::vec3> &Mesh::getNormals() {
	return normals;
}

std::vector<Group*> &Mesh::getGroups() {
	return groups;
}

void Mesh::setMappings(std::vector<glm::vec2> *i_mappings) {
	mappings = *i_mappings;
}

void Mesh::setVertexes(std::vector<glm::vec3> *i_vertexes) {
	vertexes = *i_vertexes;
}
void Mesh::setNormals(std::vector<glm::vec3> *i_normals) {
	normals = *i_normals;
}

void Mesh::setGroups(std::vector<Group*> *i_groups) {
	groups = *i_groups;
}
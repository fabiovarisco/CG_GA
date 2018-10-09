#include "Group.h"
#include "Mesh.h"

#include <iostream>

Group::Group()
{
}

Group::~Group()
{
}

std::string &Group::getName() {
	return name;
}

std::string &Group::getMaterial() {
	return material;
}

std::vector<Face*> &Group::getFaces() {
	return faces;
}

void Group::setName(std::string i_name) {
	name = i_name;
}

void Group::setMaterial(std::string i_material) {
	material = i_material;
} 

void Group::setFaces(std::vector<Face*> i_faces) {
	faces = i_faces;
}

void Group::setup(Mesh* mesh, GLuint vVBO, GLuint nVBO, GLuint tVBO) {
	//std::vector<GLuint> vertices, normals, texts;
	std::vector<glm::vec3> verticesD, normalsD;
	std::vector<glm::vec2> textsD;
	glm::vec3 aux;
	for (Face* f : faces) {
		for (unsigned int vert : f->getVerts()) {
			//vertices.push_back(vert);
			verticesD.push_back(mesh->getVertexes().at(vert));
		}
		for (unsigned int norm : f->getNorms()) {
			//normals.push_back(norm);
			normalsD.push_back(mesh->getNormals().at(norm));
		}
		for (unsigned int text : f->getTexts()) {
			//texts.push_back(text);
			textsD.push_back(mesh->getMappings().at(text));
		} 
	}

	this->nVertices = verticesD.size();
	this->nNormals = normalsD.size();
	this->nTexts = textsD.size();

	vEBO = 0;
	glGenBuffers(1, &vEBO);
	glBindBuffer(GL_ARRAY_BUFFER, vEBO);
	glBufferData(GL_ARRAY_BUFFER, verticesD.size() * sizeof(glm::vec3), verticesD.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	nEBO = 0;
	glGenBuffers(1, &nEBO);
	glBindBuffer(GL_ARRAY_BUFFER, nEBO);
	glBufferData(GL_ARRAY_BUFFER, normalsD.size() * sizeof(glm::vec3), normalsD.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	tEBO = 0;
	glGenBuffers(1, &tEBO);
	glBindBuffer(GL_ARRAY_BUFFER, tEBO);
	glBufferData(GL_ARRAY_BUFFER, textsD.size() * sizeof(glm::vec2), textsD.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,23, GL_FLOAT, GL_FALSE, 0, (void*)0);
/*
	this->nVertices = vertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	vEBO = 0;
	glGenBuffers(1, &vEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	nNormals = normals.size();
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	nEBO = 0;
	glGenBuffers(1, &nEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, normals.size() * sizeof(GLuint), normals.data(), GL_STATIC_DRAW);
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	nTexts = texts.size();
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	tEBO = 0;
	glGenBuffers(1, &tEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, texts.size() * sizeof(GLuint), texts.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	*/
}

void Group::draw(GLuint vVBO, GLuint nVBO, GLuint tVBO) {


	glBindBuffer(GL_ARRAY_BUFFER, vEBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, nEBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, tEBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, nVertices);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nEBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawElements(GL_TRIANGLES, nNormals, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawElements(GL_TRIANGLES, nTexts, GL_UNSIGNED_INT, (void*)0);*/
}
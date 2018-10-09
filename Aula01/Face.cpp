#include "Face.h"

Face::Face()
{
	
}

void Face::setVerts(std::vector<unsigned int> *i_verts) {
	verts = *i_verts;
}

void Face::setNorms(std::vector<unsigned int> *i_norms) {
	norms = *i_norms;
}

void Face::setTexts(std::vector<unsigned int> *i_texts) {
	texts = *i_texts;
}

std::vector<unsigned int> &Face::getVerts() {
	return verts;
}

std::vector<unsigned int> &Face::getNorms() {
	return norms;
}

std::vector<unsigned int> &Face::getTexts() {
	return texts;
}
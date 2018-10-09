#include <vector>


#pragma once
class Face
{
	std::vector<unsigned int> verts, norms, texts;

public:
	Face();
	std::vector<unsigned int> &getVerts();
	std::vector<unsigned int> &getNorms();
	std::vector<unsigned int> &getTexts();
	void setVerts(std::vector<unsigned int> *i_verts);
	void setNorms(std::vector<unsigned int> *i_norms);
	void setTexts(std::vector<unsigned int> *i_texts);
};


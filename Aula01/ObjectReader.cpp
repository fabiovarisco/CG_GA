#include "ObjectReader.h"



ObjectReader::ObjectReader()
{
}


ObjectReader::~ObjectReader()
{
}



vector<string> ObjectReader::explode(string delimiter, string source, int limit) {
	// PHP returns FALSE in this case, but C++ doesn't handle variant types well, and a
	// reasonable facsimile of FALSE is tricky with std::vector<>, so we'll just throw.
	if (delimiter.empty())
		throw std::invalid_argument("Empty delimiter in explode()");

	// Sanitize the limit to something meaningful
	if (limit == 0)
		limit = 1;

	std::vector<std::string> result;

	// Handle the simple case of zero delimiter matches
	if (source.find(delimiter) == std::string::npos) {
		// PHP returns array() if limit is negative, array(source) if limit is positive
		if (limit > 0)
			result.push_back(source);

		return result;
	}

	std::vector<std::string>::size_type begin = 0;
	std::vector<std::string>::size_type end = 0;

	// Explode up to any positive limit, exclusive
	while (end != std::string::npos) {
		if (limit > 0 && result.size() == limit - 1) {
			// PHP populates the last element (ie. result[limit - 1]) with the remaining 
			// string when limit is positive and less than the number of total tokens.
			result.push_back(source.substr(begin));
			break;
		}

		/*
		Conventional C++ manual string tokenization.
		*/
		std::vector<std::string>::size_type substr_len = std::string::npos;
		std::vector<std::string>::size_type next_tok = std::string::npos;

		end = source.find_first_of(delimiter, begin);

		if (end != std::string::npos) {
			substr_len = end - begin;
			next_tok = end + delimiter.size();
		}

		result.push_back(source.substr(begin, substr_len));
		begin = next_tok;
	}

	// Negative limits trim that amount from the right side of the result. It's
	// easier to do this after the explode loop because we don't need to retain
	// unexploded parts of the string like with a positive limit.
	if (limit < 0) {
		limit = abs(limit);

		if (limit < result.size())
			result.resize(result.size() - limit);
		else {
			// PHP returns array() if all items or more would be trimed
			result.clear();
		}
	}

	return result;
}

Mesh* ObjectReader::read(string filename) {
	Mesh* mesh = new Mesh();

	ifstream infile(filename);
	string line;


	while (getline(infile, line))
	{
		if (!line.empty()) {
			if (line.at(0) != '#') { //ignorar comentarios

				int pos = line.find(" ");
				string firstWord = line.substr(0, pos);

				if (firstWord.compare("mtllib") == 0) {
					mesh->materialsFile = line.substr(pos + 1, line.size());
				}
				else {
					vector<string> temp = explode(" ", line.substr(pos + 1, line.size()));
					if (firstWord.compare("v") == 0)
						mesh->getVertexes().push_back(glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str())));
					else if (firstWord.compare("vn") == 0)
						mesh->getNormals().push_back(glm::vec3(atof(temp.at(0).c_str()), atof(temp.at(1).c_str()), atof(temp.at(2).c_str())));
					else if (firstWord.compare("vt") == 0)
						mesh->getMappings().push_back(glm::vec2(atof(temp.at(0).c_str()), atof(temp.at(1).c_str())));
					//else if (firstWord.compare("g") == 0)
						//objFile.groups.push_back({ temp.at(0) });
					else if (firstWord.compare("usemtl") == 0)
					{
						Group* g = new Group();
						g->setName(temp.at(0));
						mesh->getGroups().push_back(g);
						
					}
					//objFile.groups.back().mtlName = temp.at(0);
					else if (firstWord.compare("f") == 0) {

						//caso nenhum grupo foi setado, seta um como padrão
						if (mesh->getGroups().empty()) {
							Group* g = new Group();
							g->setName("default");
							mesh->getGroups().push_back(g);
						}
						Face* f = new Face();
						for (int i = 0; i < temp.size(); i++) {
							int finded = temp.at(i).find("//");
							if (finded != string::npos) { //encontrou //
								vector<string> vn = explode("//", temp.at(i), 2);
								f->getVerts().push_back(atoi(vn.at(0).c_str()) - 1);
								f->getNorms().push_back(atoi(vn.at(1).c_str()) - 1);
							}
							else { //não encontrou
								finded = temp.at(i).find("/");
								if (finded != string::npos) { //encontrou /
									vector<string> vn = explode("/", temp.at(i), 3);
									f->getVerts().push_back(atoi(vn.at(0).c_str()) - 1);
									f->getTexts().push_back(atoi(vn.at(1).c_str()) - 1);
									if (vn.size() == 3) f->getNorms().push_back(atoi(vn.at(2).c_str()) - 1);
								}
								else f->getVerts().push_back(atoi(temp.at(i).c_str()) - 1);
							}
						}
						mesh->getGroups().back()->getFaces().push_back(f);
					}
				}
			}
		}
	}
	
	return mesh;
}